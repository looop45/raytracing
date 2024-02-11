#ifndef SMODEL_H
#define SMODEL_H

#include "vec3.hpp"
#include <math.h>

class shadingModel
{
    public:
        shadingModel() {}
        
        shadingModel(double Kd, double Ks, double Ka, color Od, color Oa, color Os, double kgls, double refl, double Kt, double IOR)
        {
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

            //transmission
            this->IOR = IOR;
            this->Kt = Kt;
        }

        bool is_reflective()
        {
            return this->refl;
        }

        bool is_transmissive()
        {
            return this->Kt;
        }

        color compute_diffuse(vec3 normal, vec3 V, vec3 L, color Ip)
        {
            //diffuse
            color Id = Kd * Ip * Od * max((double)0, dot(normal, L));
            
            return clamp(Id);
        }

        color compute_spec(vec3 normal, vec3 V, vec3 L, color Ip)
        {
            //specular
            vec3 R = unit_vector(2 * (dot(normal, -L)) * normal - (-L));
            color Is = Ks * Ip * Os * pow(max((double)0, dot(V, R)), kgls);

            return clamp(Is);
        }

        color compute_ambient()
        {
            //ambient
            color Ia = Ka * Oa * Od;
            return clamp(Ia);
        }

        color compute(vec3 normal, vec3 V, vec3 L, color Ip)
        {
            //diffuse
            color Id = Kd * Ip * Od * max((double)0, dot(normal, L));

            //specular
            vec3 R = unit_vector(2 * (dot(normal, L)) * normal - L);
            color Is = Ks * Ip * Os * pow(max((double)0, dot(V, R)), kgls);

            //reflective
                        //ambient
            color Ia = Ka * Oa * Od;
            

            return clamp(Is + Ia + Id);
        }

        //reflect
        double refl;

        double IOR;        //transmission
        double Kt;


    private:

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


};

#endif
