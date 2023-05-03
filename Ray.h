//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_RAY_H
#define CG2023_RAY_H

#include "vec3.h"
class Ray {
public:
    vec3 ori, dir, invDir;
    Ray(){
        invDir.x = 1/dir.x;
        invDir.y = 1/dir.y;
        invDir.z = 1/dir.z;
    }
    Ray(vec3 _ori){ ori = _ori; }
    Ray(vec3 _ori, vec3 _dir){
        ori = _ori;
        dir = _dir;
    }
};


#endif //CG2023_RAY_H
