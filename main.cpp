#include <iostream>
#include <fstream>

#include "vec3.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "polygon.hpp"
#include "shadingModel.hpp"
#include "scene.hpp"
#include "distant_light.hpp"
#include "point_light.hpp"
#include <string>

using namespace std;

int main()
{
    const int WIDTH = 512;
    const int HEIGHT = 512;

    //camera
    point3 camera_org(0,0,1);
    vec3 camera_dir(0,0,1);
    float fov = 150.0f;

    color lightColor(0,1,1);

    camera cam(camera_org, camera_dir, fov, WIDTH, HEIGHT);

    color bgColor = color(0.2,0.2,0.2);


    hittable_list objects;
    vec3 lightDir = vec3(-1,-1,-1);

    vector<shared_ptr<light>> lights;
//
    color ambientColor(1, 1, 1); 

    shadingModel white_Sphere(0.8, 0.2, 0.25, color(1.0, 1.0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 32, 0, 0, 0);
    shadingModel red_Sphere(0.8, 0.2, 0.25, color(1.0, 0, 0), ambientColor, color(1.0, 1.0, 1.0), 6, 0, 0, 0);
    shadingModel green_Sphere(0.8, 0.2, 0.25, color(0, 1.0, 0), ambientColor, color(1.0, 1.0, 1.0), 6, 0, 0, 0);
    shadingModel blue_Sphere(0.8, 0.2, 0.25, color(0, 0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 6, 0, 0, 0);
    shadingModel refl_Sphere(0, 1.0, 0.15, color(1.0, 1.0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 128, 0.8, 0, 0);
    shadingModel mat0(0.9, 1.0, 0.1, color(0,0,1), ambientColor, color(1.0,1.0,1.0), 4.0, 0, 0, 0);
    shadingModel mat2(0.9, 1.0, 0.1, color(1,1,0), ambientColor, color(1.0,1.0,1.0), 4.0, 0, 0, 0);


    for (int i = 0; i < 48; i++)
    {
        shadingModel trans_Sphere(00.2, 1.0, 0, color(1.0, 1.0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 128, 0.3, 1, 1 + i*0.02);

        objects.add(make_shared<sphere>(point3(0.0, 0, 0), 0.2, trans_Sphere));
        objects.add(make_shared<sphere>(point3(0.0,0,-0.5), 0.1, red_Sphere));

        objects.add(make_shared<sphere>(point3(0.0, -4, -3), 3, white_Sphere));

        //planes
        vector<point3> points;
        points.push_back(point3(0,-0.7,-0.5));
        points.push_back(point3(1,0.4,-1.0));
        points.push_back(point3(0,-0.7,-1.5));


        objects.add(make_shared<polygon>(points, mat0));

        points.clear();
        points.push_back(point3(0,-0.7,-0.5));
        points.push_back(point3(0,-0.7,-1.5));
        points.push_back(point3(-1,0.4,-1));

        objects.add(make_shared<polygon>(points, mat2));


        float rad = 0.1309;
        float light_r = 1;
        double sphere_r = 0.75;
        double lightx = sin(rad * -i * 2) * light_r;
        double lighty = cos(rad * -i * 2) * light_r;
        double spherex = sin(rad*(i+24)) * sphere_r;
        double spherey = cos(rad * (i+24)) * sphere_r;
        lights.push_back(make_shared<point_light>(point3(lightx,1,lighty), color(1,1,0.6), 1.5));
        lights.push_back(make_shared<distant_light>(lightDir, color(1,1,1), 0.6));
        objects.add(make_shared<sphere>(point3(spherex,0.25, spherey), 0.2, refl_Sphere));

        scene scene1 = scene(cam, bgColor, objects, lights, 8, WIDTH, HEIGHT);
        scene1.traceScene("scene_" + to_string(i));

        lights.clear();
        objects.clear();
    }

    
    


    

    return 0;
}