#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.hpp"
#include "ray.hpp"
#include <math.h>
#include "shadingModel.hpp"
#include "hittable.hpp"

class sphere : public hittable
{
    public:
        sphere(vec3 c, double r, shadingModel material) 
        {
            s_center = c;
            s_radius = r;
            this->material = material;
        }

        point3 center()
        {
            return s_center;
        }

        double radius()
        {
            return s_radius;
        }

        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) override;

    private:
        point3 s_center;
        double s_radius;
        shadingModel material;


};

bool sphere::hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec)
{
    vec3 oc = cam_ray.origin() - s_center;
    auto a = cam_ray.direction().length_squared();
    auto half_b = dot(oc, cam_ray.direction());
    auto c = oc.length_squared() - s_radius*s_radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);


    auto root = (-half_b - sqrtd) / a;
    if (root <= t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = cam_ray.at(rec.t);
    rec.d = cam_ray.direction();
    vec3 normal = (rec.p - s_center) / s_radius;
    rec.set_face_normal(cam_ray, normal);
    rec.material = material;

    return true;
}

#endif