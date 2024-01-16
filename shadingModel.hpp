#ifndef SMODEL_H
#define SMODEL_H

#include "vec3.hpp"
#include <math.h>

class shadingModel
{
    public:
        shadingModel() {}
        
        shadingModel(color light, double Kd, double Ks, double Ka, color Od, color Oa, color Os, double kgls, double refl, vec3 L)
        {
            Ip = light;
            this->L = unit_vector(L);

            //diffuse
            this->Kd = Kd;
            this->Od = Od;

            //ambient
            this->Ka = Ka;
            this->Oa = Oa;

            //specular
            this->Ks = Ks;
            this->kgls = kgls;
            this->Os = Os;

            //reflect
            this->refl = refl;
        }

        color compute(vec3 normal, vec3 V, double in_shadow, color refl_color)
        {
            //diffuse
            color Id = Kd * Ip * Od * max((double)0, dot(normal, L));
            
            //ambient
            color Ia = Ka * Oa * Od;

            //specular
            vec3 R = unit_vector(2 * (dot(normal, L)) * normal - L);
            color Is = Ks * Ip * Os * pow(max((double)0, dot(V, R)), kgls);

            //reflective
            return clamp(in_shadow * Is + Ia + in_shadow * Id + refl * refl_color);
        }

        color compute(vec3 normal, vec3 V, double in_shadow)
        {
            //diffuse
            color Id = Kd * Ip * Od * max((double)0, dot(normal, L));
            
            //ambient
            color Ia = Ka * Oa * Od;

            //specular
            vec3 R = unit_vector(2 * (dot(normal, L)) * normal - L);
            color Is = Ks * Ip * Os * pow(max((double)0, dot(V, R)), kgls);

            //reflective

            return clamp(Is * in_shadow + Ia + Id * in_shadow);
        }

    private:
        //light
        vec3 L;
        color Ip;

        //diffuse
        double Kd;
        color Od;

        //ambient
        double Ka;
        color Oa;

        //specular
        double Ks;
        double kgls;
        color Os;

        //reflect
        double refl;

        color clamp(color c)
        {
            color out = c;
            if (c.x() > 1)
            {
                out[0] = 1;
            }
            if (c.y() > 1)
            {
                out[1] = 1;
            }
            if (c.z() > 1)
            {
                out[2] = 1;
            }

            return out;
        }
};

#endif
