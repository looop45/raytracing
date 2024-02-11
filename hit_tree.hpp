#ifndef TREE_H
#define TREE_H

#include "color.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "vec3.hpp"

#include <memory>
#include <vector>

using namespace std;

const double infinity = numeric_limits<double>::infinity();

struct hit_node 
{
    hit_record rec;
    ray r;
    hit_node* reflection_ray;
    hit_node* transmission_ray;
    hit_node* parent;

    hit_node(hit_record rec, ray r);
    hit_node() {}

    hit_node* get_reflection_ray(int depth, hittable& world);
    hit_node* get_transmission_ray(int depth, hittable& world);
};

class hit_tree
{
    public:
        hit_tree(ray r, hit_record rec, hittable& world, vector<shared_ptr<light> > lights, color bgColor) : world(world), lights(lights)
        {
            const int DEPTH = 4;

            root.r = r;
            root.rec = rec;
            this->world = world;
            this->lights = lights;
            this->bgColor = bgColor;

            construct_tree(DEPTH, root);
        }

        ~hit_tree() {}

        color evaluate();
        color node_solve(hit_node* node);

    private:
        hit_node root;
        hittable& world;
        vector<shared_ptr<light> > lights;
        color bgColor;

        hit_node* construct_tree(int depth, hit_node &root);
};

color hit_tree::evaluate()
{
    //DFS evaluation of color at each hit record, combined to final pixel color
    color sample_color = node_solve(&root);
    return sample_color;

}

color hit_tree::node_solve(hit_node* node)
{
    color out = color(0,0,0);
    color refl_color = color(0,0,0);
    color trans_color = color(0,0,0);

    //evaluate reflection component
    if (node->reflection_ray != nullptr)
    {
        //refl_color = node->reflection_ray->r.direction();
        refl_color = node_solve(node->reflection_ray);
        //refl_color = color(0,1,1);
    }
    else
    {
        refl_color = bgColor;
    }
    refl_color = refl_color * node->rec.material.refl;

    //evaluate transmission component
    if (node->transmission_ray != nullptr)
    {
        trans_color = node_solve(node->transmission_ray);
    }
    else
    {
        trans_color = bgColor;
    }
    trans_color = trans_color * node->rec.material.Kt;

    //diffuse spec component
    for (const auto& light : lights)
        {
            //get the light's direction
            vec3 light_direction = unit_vector(light->get_light_direction(node->rec.p));

            point3 p = node->rec.p;
            ray shadow_ray(p, light_direction);
            hit_record shadow_hit;

            if(!world.hit(shadow_ray, 0.001, infinity, shadow_hit))
            {
                color diffuse_lobe = node->rec.material.compute_diffuse(node->rec.normal, node->r.direction(), light_direction, light->col);
                color spec_lobe = node->rec.material.compute_spec(node->rec.normal, node->r.direction(), light_direction, light->col);
                out += light->intensity(p) * (diffuse_lobe + spec_lobe);

            }
        }
    color ambient_term = node->rec.material.compute_ambient();
    //delete node;
    
    return clamp(out + refl_color + ambient_term + trans_color);
}

hit_node* hit_tree::construct_tree(int depth, hit_node& node)
{
    node.reflection_ray = nullptr;
    node.transmission_ray = nullptr;
    //reflections
    if(node.rec.material.is_reflective())
    {
        hit_node* reflection_node = node.get_reflection_ray(depth, world);
        node.reflection_ray = reflection_node;
    }
    //refraction
    if (node.rec.material.is_transmissive())
    {
        hit_node* transmission_node = node.get_transmission_ray(depth, world);
        node.transmission_ray = transmission_node;
    }
    //hit_node* refraction_node = node.get

    //node.refraction_ray = construct_tree(depth-1, reflection_node);

}

hit_node* hit_node::get_transmission_ray(int depth, hittable& world)
{
    if (depth <= 0 || !this->rec.material.is_transmissive())
    {
        return nullptr;
    }

    double n2 = this->rec.material.IOR;
    double n1 = 1;
    if (this->parent->rec.material.is_transmissive())
    {
        n1 = this->parent->rec.material.IOR;
        n2 = 1;
    }

    double nR = n1/n2;
    vec3 N = this->rec.normal;
    vec3 I = -this->r.direction();

    vec3 T_dir = (nR * (dot(N, I)) - sqrt(1-pow(nR, 2) * (1 - pow(dot(N,I),2)))) * N - nR * I;

    ray T = ray(this->rec.p, T_dir);

    hit_record rec;

    if (world.hit(T, 0.001, infinity, rec))
    {
        hit_node* trans_node = new hit_node();
        trans_node->parent = this;
        trans_node->r = T;
        trans_node->rec = rec;

        trans_node->transmission_ray = trans_node->get_transmission_ray(depth - 1, world);
        if (rec.material.is_reflective())
        {
            trans_node->reflection_ray = trans_node->get_reflection_ray(depth-1, world);
        }
        return trans_node;
    }
    else
    {
        return nullptr;
    }
}

hit_node* hit_node::get_reflection_ray(int depth, hittable& world)
{
    //base case, depth reached, none-reflective
    if (depth <= 0 || !this->rec.material.is_reflective())
    {
        return nullptr;
    }

    vec3 d = this->r.direction();
    vec3 n = this->rec.normal;
    vec3 r = d - 2 * n * (dot(d, n));
    //r = this->rec.p - point3(0.1,0,0.4);
    ray reflect_ray(this->rec.p, r);

    hit_record rec;

    if (world.hit(reflect_ray, 0.001, infinity, rec))
    {
        hit_node* refl_node = new hit_node();
        refl_node->parent = this;
        refl_node->r = reflect_ray;
        refl_node->rec = rec;

        refl_node->reflection_ray = refl_node->get_reflection_ray(depth - 1, world);
        if (rec.material.is_transmissive())
        {
            refl_node->transmission_ray = refl_node->get_transmission_ray(depth-1, world);
        }
        return refl_node;
    }
    else
    {
        return nullptr;
    }
}

#endif