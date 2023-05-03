//
// Created by hgallegos on 30/03/2023.
//

#include "vec3.h"

vec3 operator*(float f, vec3 v) {
    return vec3(v.x * f, v.y * f, v.z*f); }

vec3 vec3::shiftValues(float offset, int range){
    vec3 new_vec = *this;
    for(int i=0; i<3; i++){
        float ran = (rand()%range)-offset;
        if(i==0) new_vec.x += ran;
        if(i==1) new_vec.y += ran;
        if(i==2) new_vec.z += ran;
    }
    return new_vec;
}

float clamp(float menor, float mayor, float valor){
    if (valor < menor)
        return menor;
    if ( valor > mayor)
        return mayor;
    return valor;
}

vec3 vec3::getCenter(vec3 bound){
    float cenX = (this->x + bound.x)/2;
    float cenY = (this->y + bound.y)/2;
    float cenZ = (this->z + bound.z)/2;
    return {cenX,cenY,cenZ};
}
