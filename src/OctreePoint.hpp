//
//  OctreePoint.hpp
//  my3DTerrain
//
//  OctreePoint Implementation done by brandonpelfrey
//  https://github.com/brandonpelfrey/SimpleOctree
//
//  Created by Christopher Kang on 4/18/18.
//

#ifndef OctreePoint_hpp
#define OctreePoint_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Box.hpp"
#include "Ray.hpp"

class OctreePoint {
    ofVec3f position;
public:
    OctreePoint() {}
    OctreePoint(const ofVec3f& position) : position(position) { }
    const ofVec3f& getPosition() { return position; }
    void setPosition(const ofVec3f& p) { position = p; }

};

#endif /* OctreePoint_hpp */
