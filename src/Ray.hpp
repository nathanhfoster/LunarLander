#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "ofMain.h"

/*
 * Ray class, for use with the optimized ray-box intersection test
 * described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 Vector3 class replaced by ofVec3f from open frameworks
 
 Christopher Kang
 
 */

class Ray {
public:
    Ray() { }
    Ray(ofVec3f o, ofVec3f d) {
        origin = o;
        direction = d;
        inv_direction = ofVec3f(1/d.x, 1/d.y, 1/d.z);
        sign[0] = (inv_direction.x < 0);
        sign[1] = (inv_direction.y < 0);
        sign[2] = (inv_direction.z < 0);
    }
    Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
        inv_direction = r.inv_direction;
        sign[0] = r.sign[0]; sign[1] = r.sign[1]; sign[2] = r.sign[2];
    }
    
    ofVec3f origin;
    ofVec3f direction;
    ofVec3f inv_direction;
    int sign[3];
};

#endif /* Ray_hpp */
