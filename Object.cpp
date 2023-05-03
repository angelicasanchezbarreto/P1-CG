//
// Created by hgallegos on 30/03/2023.
//

#include "Object.h"
bool Sphere::intersect(Ray ray, float &t, vec3 &normal) {
    auto _a = ray.dir.punto(ray.dir);
    auto _b = 2*ray.dir.punto(ray.ori-center);
    auto _c = (ray.ori-center).punto(ray.ori-center)-radio*radio;
    auto D = _b*_b-4*_a*_c;
    if(D <= 0) {return false;}
    float t1 = (-_b + sqrt(D))/2*_a;
    float t2 = (-_b - sqrt(D))/2*_a;
    t = std::min(t1, t2);
    if(t <= 0) {return false;}
    vec3 pi = ray.ori + ray.dir * t;
    normal = pi - center;
    normal.normalize();
    return true;
}

bool Plane::intersect(Ray ray, float &t, vec3 &normal) {
    float denom = normal_plane.punto(ray.dir);
    if (denom != 0) {
        t = (normal_plane*d - ray.ori).punto(normal_plane) / denom;
        if (t < 0) {
            return false;
        }
        normal = normal_plane;
        return true;
    }
    return false;
}

bool Box::intersect(Ray ray, float &t, vec3 &normal) {
    /* t = ratio of the distance from the ray origin */
    /* X */
    float tMinX = (min_bound.x - ray.ori.x)/ray.dir.x;
    float tMaxX = (max_bound.x - ray.ori.x)/ray.dir.x;
    //if(tMaxX < tMinX) swap(tMinX,tMaxX);
    /* Y */
    float tMinY = (min_bound.y - ray.ori.y)/ray.dir.y;
    float tMaxY = (max_bound.y - ray.ori.y)/ray.dir.y;
    //if(tMaxY < tMinY) swap(tMinY,tMaxY);
    /* Z */
    float tMinZ = (min_bound.z - ray.ori.z)/ray.dir.z;
    float tMaxZ = (max_bound.z - ray.ori.z)/ray.dir.z;
    //if(tMaxZ < tMinZ) swap(tMinZ,tMaxZ);

    //Get greatest min
    float tMin = max(max(min(tMinX,tMaxX),min(tMinY,tMaxY)),min(tMinZ,tMaxZ));
    //Get smallest max
    float tMax = min(min(max(tMinX,tMaxX),max(tMinY,tMaxY)),max(tMinZ,tMaxZ));
    //if the ray origin is inside de cube tMin<=0
    if(tMax<0) return false; //box is behind ray
    if(tMin>tMax) return false; //box is missed
    /*if(tMin<0) t = tMax;
    else */t = tMin;
    vec3 hit_pt = ray.ori + ray.dir * t;
    vec3 box_hit = hit_pt - min_bound.getCenter(max_bound);
    vec3 box_normal = box_hit / max(max(abs(box_hit.x), abs(box_hit.y)), abs(box_hit.z));
    normal.x = clamp(box_normal.x, 0, 1);
    normal.y = clamp(box_normal.y, 0, 1);
    normal.z = clamp(box_normal.z, 0, 1);
    normal.normalize();
    return true;
}

bool Cylinder::intersect(Ray rayo, float &t, vec3 &normal) {
    vec3 ro = rayo.ori;
    vec3 rd = rayo.dir;
    vec3 ca = pb-pa;
    vec3 oc = ro-pa;
    float caca = ca.punto(ca);
    float card = ca.punto(rd);
    float caoc = ca.punto(oc);
    float a = caca - card*card;
    float b = caca * oc.punto(rd) - caoc*card;
    float c = caca * oc.punto(oc) - caoc*caoc - ra*ra*caca;
    float h = b*b - a*c;
    if( h < 0.0 ) return false; //no intersection
    h = sqrt(h);
    t = (-b-h)/a;
    //if (t <= 0) return false;
    // body
    float y = caoc + t*card;
    if ( y > 0.0 && y < caca && t > 0) {
        normal = (oc + t*rd - ca*y/caca)/ra;
        normal.normalize();
        return true;
    }
    // caps
    t = (((y<0.0)?0.0:caca) - caoc)/card;
    if (t <= 0) return false;
    if( abs(b+a*t)<h ) {
        normal =  ca * sgn(y) / caca;
        normal.normalize();
        return true;
    }
    return false; //no intersection
}
