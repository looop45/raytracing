#include <iostream>
#include <fstream>

#include "vec3.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "polygon.hpp"
#include "shadingModel.hpp"
#include "scene.hpp"
#include <string>

using namespace std;

int main()
{
    const int WIDTH = 512;
    const int HEIGHT = 512;


    //*********SCENE 1**********
    //camera
    point3 camera_org(0,0,1);
    vec3 camera_dir(0,0,1);
    float fov = 150.0f;

    camera cam(camera_org, camera_dir, fov, WIDTH, HEIGHT);

    //sphere(s)
    vec3 lightDir(0,1,0);

    color lightColor(1,1,1);
    color ambientColor(0.1, 0.1, 0.1); 
    color bgColor(0.2, 0.2, 0.2);
    

    hittable_list objects;
    //planes
    vector<point3> points;
    points.push_back(point3(0,-0.7,-0.5));
    points.push_back(point3(1,0.4,-1.0));
    points.push_back(point3(0,-0.7,-1.5));

    shadingModel mat0(lightColor, 0.9, 1.0, 0.1, color(0,0,1), ambientColor, color(1.0,1.0,1.0), 4.0, 0, lightDir);

    objects.add(make_shared<polygon>(points, mat0));

    points.clear();
    points.push_back(point3(0,-0.7,-0.5));
    points.push_back(point3(0,-0.7,-1.5));
    points.push_back(point3(-1,0.4,-1));

    shadingModel mat2(lightColor, 0.9, 1.0, 0.1, color(1,1,0), ambientColor, color(1.0,1.0,1.0), 4.0, 0, lightDir);

    objects.add(make_shared<polygon>(points, mat2));
  
    shadingModel mat3(lightColor, 0.1, 0.1, 0.1, color(0.75,0.75,0.75), ambientColor, color(1.0,1.0,1.0), 10.0, 1, lightDir);

    objects.add(make_shared<sphere >(point3(0,0.3,-1), 0.25, mat3));
    
    scene scene1(cam, bgColor, lightDir, objects, WIDTH, HEIGHT);
    scene1.traceScene("scene1");

    //********SCWENE 2*********
    lightDir = vec3(1,0,0);

    //spheres
    objects.clear();

    shadingModel white_Sphere(lightColor, 0.8, 0.1, 0.3, color(1.0, 1.0, 1.0), ambientColor, color(1.0, 1.0, 1.0), 4, 0, lightDir);
    objects.add(make_shared<sphere>(point3(0.5, 0.0, -0.15), 0.05, white_Sphere));
    
    shadingModel red_sphere(lightColor, 0.8, 0.8, 0.1, color(1.0, 0, 0), ambientColor, color(0.5, 1.0, 0.5), 32, 0, lightDir);
    objects.add(make_shared<sphere>(point3(0.3, 0.0, -0.1), 0.08, red_sphere));

    shadingModel green_sphere(lightColor, 0.7, 0.5, 0.1, color(0, 1, 0), ambientColor, color(0.5, 1.0, 0.5), 64, 0, lightDir);
    objects.add(make_shared<sphere>(point3(-0.6, 0.0, 0.0), 0.3, green_sphere));

    shadingModel reflective_sphere(lightColor, 0.0, 0.1, 0.1, color(.75, .75, .75), ambientColor, color(1, 1, 1), 10, 0.9, lightDir);
    objects.add(make_shared<sphere>(point3(0.1, -0.55, 0.25), 0.3, reflective_sphere));
    
    shadingModel blue_triangle(lightColor, 0.9, 0.9, 0.1, color(0, 0, 1), ambientColor, color(1, 1, 1), 32, 0, lightDir);
    points.clear();
    points.push_back(point3(0.3,-0.3,-0.4));
    points.push_back(point3(0,0.3,-0.1));
    points.push_back(point3(-0.3,-0.3,0.2));
    objects.add(make_shared<polygon>(points, blue_triangle));

    shadingModel yellow_triangle(lightColor, 0.9, 0.5, 0.1, color(1, 1, 0), ambientColor, color(1, 1, 1), 4, 0, lightDir);
    points.clear();
    points.push_back(point3(-0.2,0.1,0.1));
    points.push_back(point3(-0.2,-0.5,0.2));
    points.push_back(point3(-0.2,0.1,-0.3));
    objects.add(make_shared<polygon>(points, yellow_triangle));

    scene scene2(cam, bgColor, lightDir, objects, WIDTH, HEIGHT);
    scene2.traceScene("scene2");

    //************SCENE 3************
    objects.clear();
    lightDir = vec3(-1,0.3,1);
    red_sphere = shadingModel(lightColor, 0.8, 0.8, 0.1, color(1.0, 0, 0), ambientColor, color(0.5, 1.0, 0.5), 32, 0, lightDir);
    reflective_sphere = shadingModel(lightColor, 0.0, 0.1, 0.1, color(.75, .75, .75), ambientColor, color(1, 1, 1), 10, 0.9, lightDir);
    shadingModel blue_polygon(lightColor, 0.8, 0.8, 0.1, color(0, 0, 1), ambientColor, color(1, 1.0, 1), 64, 0, lightDir);
    shadingModel green_polygon(lightColor, 0.8, 0.8, 0.1, color(0, 1, 0.05), ambientColor, color(1, 1.0, 1), 4, 0, lightDir);
    shadingModel yellow(lightColor, 0.8, 0.8, 0.1, color(1, 1, 0.05), ambientColor, color(1, 1.0, 1), 4, 0, lightDir);




    for (int i = 0; i < 6; i++)
    {
        objects.add(make_shared<sphere>(vec3(i*0.1, i * 0.1 - 0.5, -i*0.1), i * 0.05, red_sphere));
        objects.add(make_shared<sphere>(vec3(-i*0.1, i * 0.1 - 0.5, -i*0.05), i * 0.05, reflective_sphere));
        points.clear();
        points.push_back(point3(-0.5,3 - 0.3*i,-3 + 0.2*i));
        points.push_back(point3(0,2 - 0.3*i,-2+ 0.2*i));
        points.push_back(point3(0.5,3 - 0.3*i,-3 + 0.2*i));
        objects.add(make_shared<polygon>(points, green_polygon));
    }

    points.clear();
    points.push_back(point3(-10,-0.5,-3));
    points.push_back(point3(0,-0.2,10));
    points.push_back(point3(10,-0.5,-3));
    //objects.add(make_shared<polygon>(points, blue_polygon));

    objects.add(make_shared<sphere>(vec3(0,-0.5,0.2), 0.1, yellow));


    scene scene3(cam, bgColor, lightDir, objects, WIDTH, HEIGHT);
    scene3.traceScene("scene3");

    return 0;
}