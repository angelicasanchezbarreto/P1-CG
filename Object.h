//
// Created by hgallegos on 30/03/2023.
//
#pragma once

#ifndef CG2023_OBJECT_H
#define CG2023_OBJECT_H
#include "vec3.h"
#include "Ray.h"
#include <cmath>

using namespace std;

class Object {
public:
    vec3 color;
    float kd,ks,ke,n; //difusa, specular, espejo
    bool is_clear, is_light;
    float refraction_index;
    Object(vec3 col,float kd=1): color{col}, kd{kd}{
        ke=0;
        is_clear=false;
        refraction_index=1;
        is_light=false;
    }
    void setConst(float kd=1,float ks=1,float ke=0,float n=8){
        this->kd=kd; this->ks=ks; this->ke=ke; this->n=n;}
    void isClear(bool is_clear,float ior){
        this->is_clear=is_clear; refraction_index=ior;}
    virtual bool intersect(Ray ray,float &t,vec3 &normal)=0;
};

class Sphere : public Object {
public:
    vec3 center;
    float radio;

    Sphere(vec3 cen,float r,vec3 col,float kd=1): center{cen}, radio{r}, Object(col, kd) {}
    bool intersect(Ray ray, float &t, vec3 &normal);
};

class Plane : public Object {
public:
    vec3 normal_plane;
    float d;
    Plane(vec3 normal,float dist,vec3 col): normal_plane(normal), Object(col) {
        normal_plane.normalize();
    }
    bool intersect(Ray ray, float &t, vec3 &normal);
};

class Box : public Object {
public:
    vec3 min_bound; //left bottom
    vec3 max_bound; //right top

    Box(vec3 min,vec3 max,vec3 col,float kd_): min_bound{min}, max_bound{max}, Object(col, kd_) {}
    vec3 getDistance(vec3 hit_pt){
        vec3 d;
        d.x = abs(hit_pt.x-((max_bound.x-min_bound.x)/2));
        d.y = abs(hit_pt.y-((max_bound.y-min_bound.y)/2));
        d.z = abs(hit_pt.z-((max_bound.z-min_bound.z)/2));
        return d;
    }
    float clamp(float value, float min, float max){
        if (value < min)
            return min;
        else if (value > max)
            return max;
        return value;
    }
    bool intersect(Ray ray, float &t, vec3 &normal);
};

class Cylinder: public Object{
public:
    vec3 pa, pb;
    float ra;
    Cylinder(vec3 _pa, vec3 _pb, float _ra, vec3 _color):
            pa{_pa}, pb{_pb}, ra{_ra}, Object(_color){}
    bool intersect(Ray rayo, float &t, vec3 &normal);
};
#endif //CG2023_OBJECT_H
