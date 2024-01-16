#ifndef SCENE_H
#define SCENE_H

#include "vec3.hpp"
#include <vector>
#include "sphere.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

const double infinity = numeric_limits<double>::infinity();


class scene
{
    public:
        scene(camera cam, color bgColor, vec3 lightDir, hittable_list surfaces, int width, int height)
        {
            this->cam = cam;
            this->bgColor = bgColor;
            this->surfaces = surfaces;
            this->WIDTH = width;
            this->HEIGHT = height;
            this->lightDir = lightDir;
        }

        void traceScene(string name)
        {
            //write out file
            ofstream outFile;
            outFile.open(name + ".ppm");

            outFile << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

            for (int j = HEIGHT - 1; j >= 0; j--)
            {
                //cout << "\rSCANLINES REMAINING: " << j << ' ' << flush;
                for (int i = 0; i < WIDTH; i++)
                {
                    ray r = cam.getRay(i,j);
                    color pixel_color = ray_color(r,surfaces);
                    write_color(outFile, pixel_color);
                }
            }

            cout << "\nBEEP BOOP BEEP BOOP DONE!" << endl;

            outFile.close();
        }

    private:
        int WIDTH;
        int HEIGHT;

        camera cam;

        vec3 lightDir;
        color lightColor;
        color ambientColor;
        color bgColor;

        hittable_list surfaces;

        //returns the color derived from the shading model or the BG
        color ray_color(const ray& r, hittable& world)
        {
            hit_record rec;
            if (world.hit(r, 0, infinity, rec))
            {
                color out;
                double in_shadow = check_shadow(rec.p, surfaces);

                color rcol = refl_color(rec.normal, r.direction(), rec.p, surfaces, 1);
                out = rec.material.compute(rec.normal, -r.direction(), in_shadow, rcol);
                return out;
            }
            //no hit, do gradient bg
            return bgColor;
        }

        double check_shadow(point3 intersect, hittable& world)
        {
            vec3 unit = unit_vector(lightDir);
            point3 p = intersect;
            ray shadow_ray(p, unit);
            hit_record temp;

            if (world.hit(shadow_ray, 0.001, infinity, temp))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }

        color refl_color(vec3 n, vec3 d, point3 p, hittable& world, int depth)
        {
            vec3 x = lightDir;
            vec3 r = d - 2 * n * (dot(d, n));
            hit_record rec;

            ray reflect_ray(p,r);
            if (world.hit(reflect_ray, 0.001, infinity, rec))
            {
                double in_shadow = check_shadow(rec.p, surfaces);
                if (depth > -1)
                {
                    color rcol = refl_color(rec.normal, reflect_ray.direction(), rec.p, surfaces, depth - 1);
                    return rec.material.compute(rec.normal, reflect_ray.direction(), in_shadow, rcol);\
                }
                return rec.material.compute(rec.normal, reflect_ray.direction(), in_shadow);

            }
            return bgColor;
        }

};

#endif