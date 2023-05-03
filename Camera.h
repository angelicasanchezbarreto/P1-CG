//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_CAMERA_H
#define CG2023_CAMERA_H

#include "Ray.h"
#include "vec3.h"
#include "CImg.h"
#include "Object.h"
#include "Light.h"
#include <vector>
using namespace std;
using namespace cimg_library;
typedef unsigned char BYTE;

class Camera {
    vec3 eye, xe, ye, ze;
    float f, a, b, w, h;
    CImg<BYTE> *pImg;
    int prof_max;
public:
    vector<Object*> objects;
    vector<Light*> lights;
    int numObjs() const{
        return objects.size();
    }
    Camera(){ prof_max = 7; }
    void setObj(Object* obj);
    void configure(float _near,float fov,int ancho,int alto,
                   vec3 pos_eye,vec3 center,vec3 up);
    void render(int num=1);
    vec3 calculateColor(Ray ray,int prof);
    vec3 refract(vec3 &I, vec3 &N, float &ior);
    void fresnel(vec3 &I, vec3 &N, float &ior, float &kr);
};


#endif //CG2023_CAMERA_H
