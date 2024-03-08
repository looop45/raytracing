#ifndef POLY_H
#define POLY_H

#include "vec3.hpp"
#include "vertex.hpp"
#include "vec2.hpp"
#include "ray.hpp"
#include <math.h>
#include "shadingModel.hpp"
#include "hittable.hpp"
#include <vector>
#include "vertex.hpp"

using namespace std;

class polygon : public hittable
{
    public:
        polygon(vector<shared_ptr<vertex>> vertices, shadingModel* material) 
        {
            this->material = material;
            this->normal = compute_normal(vertices);
            this->distance = compute_dist(vertices.at(0), normal);
            this->vertices = vertices;
            calculate_extents();
        }

        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) override;

    private:
        shadingModel* material;
        point3 normal;
        double distance;
        vector<shared_ptr<vertex>> vertices;
        void calculate_extents() override;

        vec3 compute_normal(vector<shared_ptr<vertex>> vertices)
        {
            vec3 v1 = *vertices.at(1) - *vertices.at(0);
            vec3 v2 = *vertices.at(2) - *vertices.at(0);

            return unit_vector(cross(v1, v2));
        }

        double compute_dist(shared_ptr<vertex> point, vec3 normal)
        {
            double d = -(point->x() * normal.x()+ point->y() * normal.y() + point->z() * normal.z());
            return d;
        }

        vector<point2> project2d(vector<shared_ptr<vertex>> vertices)
        {
            //find which axis to project onto
            int dom_offset;

            vector<point2> vertices2d;
            if (maximize(normal) == normal.x())
            {
                dom_offset = 0;
            }
            else if (maximize(normal) == normal.y())
            {
                dom_offset = 1;
            }
            else
            {
                dom_offset = 2;
            }

            for (int i = 0; i < vertices.size(); i++)
            {
                vertex point3d = *vertices.at(i);
                point2 new_point;
                if (dom_offset == 0)
                {
                    new_point = point2(point3d.y(), point3d.z());
                }
                else if (dom_offset == 1)
                {
                    new_point = point2(point3d.x(), point3d.z());
                }
                else
                {
                    new_point = point2(point3d.x(), point3d.y());
                }
                vertices2d.push_back(new_point);
            }
            return vertices2d;
        }

        point2 project2d(point3 intersect)
        {

            point2 new_point;
            if (maximize(normal) == normal.x())
            {
                new_point = point2(intersect.y(), intersect.z());
            }
            else if (maximize(normal) == normal.y())
            {
                new_point = point2(intersect.x(), intersect.z());
            }
            else
            {
                new_point = point2(intersect.x(), intersect.y());
            }

            return new_point;
        }

        bool test_crossings(point3 intersection)
        {
            //convert vertices
            vector<point2> points2d = project2d(vertices);

            point2 intersect2d = project2d(intersection);

            //translate
            for (int i = 0; i < points2d.size(); i++)
            {
                points2d.at(i) = points2d.at(i) - intersect2d;
            }

            int num_crossings = 0;
            int sign_holder;

            if (points2d.at(0).v() < 0)
            {
                sign_holder = -1;
            }
            else{
                sign_holder = 1;
            }

            //loop
            for (int i = 0; i < points2d.size(); i++)
            {
                int next_signholder;
                //wrap around
                int next = i + 1;
                if (next == points2d.size())
                {
                    next = 0;
                }

                if (points2d.at(next).v() < 0)
                {
                    next_signholder  = -1;
                }
                else
                {
                    next_signholder = 1;
                }

                //check signs
                if (sign_holder != next_signholder)
                {
                    if (points2d.at(i).u() > 0 && points2d.at(next).u() > 0)
                    {
                        num_crossings++;
                    }
                    else if (points2d.at(i).u() > 0 || points2d.at(next).u() > 0)
                    {
                        auto ucross = points2d.at(i).u() - points2d.at(i).v() * (points2d.at(next).u() - points2d.at(i).u()) / (points2d.at(next).v() - points2d.at(i).v());\

                        if (ucross > 0)
                        {
                            num_crossings++;
                        }
                    }
                    sign_holder = next_signholder;
                }
            }
            
            if (num_crossings % 2 == 0)
            {
                return false;
            }

            return true;
        }

};

bool polygon::hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec)
{
    auto discriminant = dot(normal, cam_ray.direction());
    
    /*if (discriminant >= 0) //double sided or single sided
    {
        return false;
    }
    else
    {*/
        double t = (-(dot(normal, cam_ray.origin()) + distance))/(discriminant);

        if (t < t_min || t > t_max)
        {
            return false;
        }

        point3 intersection = cam_ray.at(t);

        if (!test_crossings(intersection))
        {
            return false;
        }

        rec.set_face_normal(cam_ray, normal);
        rec.d = cam_ray.direction();
        rec.t = t;
        rec.p = intersection;
        rec.material = material;

        return true;
    //}
}



void polygon::calculate_extents()
{
    double max_x = 0;
    double max_y = 0;
    double max_z = 0;

    double min_x = 0;
    double min_y = 0;
    double min_z = 0;

    for (shared_ptr<point3> point : vertices)
    {
        //x component
        if (point->x() > max_x)
        {
            max_x = point->x();
        }
        else if(point->x() < min_x)
        {
            min_x = point->x();
        }

        //y component
        if (point->y() > max_y)
        {
            max_y = point->y();
        }
        else if(point->y() < min_y)
        {
            min_y = point->y();
        }

        //z component
        if (point->z() > max_z)
        {
            max_z = point->z();
        }
        else if(point->z() < min_z)
        {
            min_z = point->z();
        }
    } 

    this->lower_extent = point3(min_x, min_y, min_z);
    this->upper_extent = point3(max_x, max_y, max_z);
}

#endif