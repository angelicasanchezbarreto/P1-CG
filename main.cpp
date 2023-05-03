#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Camera.h"
#include "Object.h"

void moveFireFlies(Camera cam, int size,int spheres){
    for(int i=size; i<size+spheres; i++){
        auto *sp = dynamic_cast<Sphere*>(cam.objects[i]);
        sp->center = sp->center.shiftValues(0.5,2);
    }
}

void scene1(Camera cam){
    float kd = (float) rand()/RAND_MAX;
    float ks = (float) rand()/RAND_MAX;
    float ke = (float) rand()/RAND_MAX;

    cam.setObj(new Plane(vec3(0, 1, 0), 1, vec3(0.545, 0.271, 0.075)));
    cam.objects[cam.numObjs()-1]->setConst(kd,ks);

    vec3 cyl_pa = vec3(0,0,0);
    vec3 cyl_pb = vec3(0,15,0);
    float ra_ = 10;
    //body
    cam.setObj(new Cylinder(cyl_pa,
                            cyl_pb,ra_,
                            vec3(0.529,0.81,0.98)));
    cam.objects[1]->setConst(kd,ks,0,8);
    cam.objects[1]->isClear(true,1.3);

    Light light1(vec3(0,10,0), vec3(1,1,0));
    cam.lights.push_back(&light1);

    float r = 0.5;
    int spheres = 5;
    int size = cam.numObjs();
    for(int i=0; i<spheres; i++) {
        vec3 luz_col(0.2, 0.2, 0);
        vec3 cen = cyl_pa.getCenter(cyl_pb);
        vec3 new_pos = cen.shiftValues(3,6);
        cam.setObj(new Sphere(new_pos, r,
                              luz_col,
                              kd));
        cam.objects[cam.numObjs()-1]->setConst(kd,ks);
        cam.objects[cam.numObjs()-1]->is_light=true;

        /*Light light1(new_pos, luz_col);
        cam.lights.push_back(&light1);*/
    }

    //cubes
    cam.setObj(new Box(vec3(14,0,12),
                       vec3(16,2,14),
                       vec3(1,0,0),kd));

    cam.setObj(new Box(vec3(13,2,12),
                       vec3(15,4,14),
                       vec3(1,1,0),kd));

    cam.setObj(new Box(vec3(14,4,12),
                       vec3(16,6,14),
                       vec3(1,0,1),kd));

    cam.setObj(new Box(vec3(12,0,12),
                       vec3(14,2,14),
                       vec3(0,1,1),kd));

    cam.setObj(new Box(vec3(16,0,12),
                       vec3(18,2,14),
                       vec3(0.8,1,0),kd));

    cam.setObj(new Box(vec3(15,2,12),
                       vec3(17,4,14),
                       vec3(0.3,0.5,0.8),kd));

    //spheres
    cam.setObj(new Sphere(vec3(-5,2,22),2,
                          vec3(0.3,0.3,0.9),kd));
    cam.objects[cam.numObjs()-1]->setConst(kd,ks,ke);
    cam.setObj(new Sphere(vec3(-8,3,18),3,
                          vec3(0.2,0.6,0.4),kd));
    cam.objects[cam.numObjs()-1]->setConst(kd,ks,ke);

    //cylinders
    cam.setObj(new Cylinder(vec3(0,0,-15),
                            vec3(0,7,-15),1.5,
                            vec3(0.6,0,0.4)));
    cam.setObj(new Cylinder(vec3(5,1.5,-15),
                            vec3(10,1.5,-10),1.5,
                            vec3(0.6,0.8,1)));

    for (int x=0,y=0,z=80,n=1; x<80,y<80,z>0; x+=1,y+=1,z-=1, n++){
        cam.configure(3,60,1200,800,
                      vec3(x,y,z),
                      vec3(0,0,0),
                      vec3(0,1,0));
        cam.render(n);
        moveFireFlies(cam,size,spheres);
    }
}

int main() {
    srand((unsigned) time(NULL));
    Camera cam;
    scene1(cam);

    return 0;
}



