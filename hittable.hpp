#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.hpp"

struct hit_record 
{
    point3 p;
    vec3 normal;
    double t;
    shadingModel material;
    bool front_face;
    vec3 d;
    int id;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) 
    {
        
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
    
};

class hittable {
    public:
        virtual bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) = 0;
};

#endif
