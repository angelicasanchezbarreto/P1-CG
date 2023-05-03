//
// Created by hgallegos on 5/04/2023.
//

#ifndef CG2023_LIGHT_H
#define CG2023_LIGHT_H
#include "vec3.h"

class Light {
public:
    vec3 pos, color;
    Light(vec3 _pos, vec3 _color): pos(_pos), color(_color){};
};


#endif //CG2023_LIGHT_H
