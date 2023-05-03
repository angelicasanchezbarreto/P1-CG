//
// Created by hgallegos on 30/03/2023.
//

#include "Camera.h"
#include <iostream>

using namespace std;

void Camera::setObj(Object* obj) {
    objects.push_back(obj);
}

void Camera::configure(float _near,float fov,int ancho,int alto,
                       vec3 pos_eye,vec3 center,vec3 up){
    f = _near;
    w = ancho;
    h = alto;
    a = 2 * f * tan(fov * M_PI/360);
    b = w / h * a;
    eye = pos_eye;
    ze = eye - center;
    ze.normalize();
    xe = up.cruz(ze);
    xe.normalize();
    ye = ze.cruz(xe);
}

void Camera::render(int num) {
    pImg = new CImg<BYTE>(w, h, 1, 10);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camera Pinhole");

    vec3 color(0,0,0), dir;
    for(int x=0;  x < w; x++) {
        for (int y = 0; y < h; y++) {
            int samples=4;
            for(int i=0; i<samples; i++){
                for(int j=0; j<samples; j++){
                    float new_x = (x + (i + 0.5) / 2);
                    float new_y = (y + (j + 0.5) / 2);
                    dir = ze * (-f) + ye * a * (new_y / h - 0.5) + xe * b * (new_x / w - 0.5);
                    dir.normalize();
                    Ray ray(eye);
                    ray.dir = dir;
                    color = color + calculateColor(ray,1);
                }
            }
            color = color / (samples*samples);

            (*pImg)(x,h-1-y,0) = (BYTE)(color.x * 255);
            (*pImg)(x,h-1-y,1) = (BYTE)(color.y * 255);
            (*pImg)(x,h-1-y,2) = (BYTE)(color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    string filename = "image" + to_string(num) + ".bmp";
    pImg->save(filename.c_str());
    /*while (!dis_img.is_closed()) {
        dis_img.wait();
    }*/

}

vec3 Camera::calculateColor(Ray ray, int prof) {
    vec3 color(0,0,0);
    vec3 normal, normal_tmp;
    Object *pObject;
    bool is_intersection = false;
    float t_tmp, t = 1000000000;
    for(auto pObj : objects) {
        if ( pObj->intersect(ray, t_tmp, normal_tmp)) {
            is_intersection = true;
            if (t_tmp < t) {
                t = t_tmp;
                normal = normal_tmp;
                pObject = pObj;
            }
        }
    }

    if (is_intersection and pObject->is_light)
        color = pObject->color;
    else if (is_intersection){
        vec3 pi = ray.ori + ray.dir * t;
        vec3 V = -ray.dir;
        //TRANSPARENCIA
        if (pObject->is_clear){
            // kr kt
            vec3 refraction_color(0,0,0);
            // compute fresnel
            float kr;
            fresnel(ray.dir, normal, pObject->refraction_index, kr);
            bool outside = ray.dir.punto(normal) < 0;
            vec3 bias = 0.0005 * normal;
            // compute refraction if it is not a case of total internal reflection
            if (kr < 1) {
                vec3 refraction_dir = refract(ray.dir, normal, pObject->refraction_index);
                refraction_dir.normalize();
                vec3 refraction_ray_orig = outside ? pi - bias : pi + bias;
                Ray refraction_ray(refraction_ray_orig, refraction_dir);
                refraction_color = calculateColor(refraction_ray,prof + 1);
            }

            vec3 reflection_dir = 2 * (V.punto(normal)) * normal - V;// reflect(dir, normal).normalize();
            reflection_dir.normalize();
            vec3 reflection_ray_orig = outside ? pi + bias : pi - bias;
            Ray reflection_ray(reflection_ray_orig, reflection_dir);
            vec3 reflection_color = calculateColor(reflection_ray,prof + 1);

            // mix the two
            color = reflection_color * kr + refraction_color * (1 - kr);
        } else {
            if (pObject->ke > 0 and  prof + 1 <= prof_max) {
                // rayos reflexivos
                Ray reflective_ray;
                reflective_ray.ori = pi + 0.0005 * normal;
                reflective_ray.dir = 2 * (V.punto(normal)) * normal - V;
                reflective_ray.dir.normalize();
                vec3 color_reflexivo = calculateColor(reflective_ray,prof + 1);
                color = pObject->ke * color_reflexivo;
            }
        }

        vec3 ambient_light = vec3(1, 1, 1) * 0.2;
        vec3 diffuse_light = vec3(0, 0, 0);
        vec3 specular_light = vec3(0, 0, 0);

        for(auto light:lights) {
            vec3 L = light->pos - pi;
            float dist = L.modulo();
            L.normalize();
            // evaluar si hay sombra
            bool is_shadow = false;
            Ray shadow_ray(pi + 0.0005 * normal, L);
            for (auto pObj: objects) {
                if ((not pObj->is_light) and (not pObj->is_clear) and
                    pObj->intersect(shadow_ray, t_tmp, normal_tmp) and t_tmp < dist) {
                    is_shadow = true;
                }
            }

            if (!is_shadow) {
                //LUZ DIFUSA
                float diffuse_coef = normal.punto(L);
                if (diffuse_coef > 0)
                    diffuse_light = light->color * pObject->kd * diffuse_coef;

                //LUZ ESPECULAR
                vec3 R = 2 * (L.punto(normal)) * normal - L;
                float specular_coef = R.punto(V);
                if (specular_coef > 0)
                    specular_light = light->color * pObject->ks * pow(specular_coef, pObject->n);

                color = color + pObject->color * (ambient_light + diffuse_light + specular_light);
                color.max_to_one();
            } else {
                color = color + pObject->color * (ambient_light);
                color.max_to_one();
            }
        }
    }
    return color;
}

vec3 Camera::refract(vec3 &I, vec3 &N, float &ior){
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    vec3 n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3(0,0,0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Camera::fresnel(vec3 &I, vec3 &N, float &ior, float &kr){
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
}
