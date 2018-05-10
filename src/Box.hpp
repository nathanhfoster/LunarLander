#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Ray.hpp"

/*
 * Axis-aligned bounding box class, for use with the optimized ray-box
 * intersection test described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 Vector3 class replaced by ofVec3f from open frameworks
 
 Christopher Kang
 
 */

class Box {
  public:
    Box() { }
    Box(const ofVec3f &min, const ofVec3f &max) {
        // assert(min < max);
        parameters[0] = min;
        parameters[1] = max;
    }
    Box(const Box& copy) {
        parameters[0] = copy.parameters[0];
        parameters[1] = copy.parameters[1];
    }
    // (t0, t1) is the interval for valid hits
    bool intersect(const Ray &r, float t0, float t1) const;
    
    // corners
    ofVec3f parameters[2];
    ofVec3f min() { return parameters[0]; }
    ofVec3f max() { return parameters[1]; }

//    static void subDivideBox8(const Box* box, vector<Box*> & boxList) {
//        Vector3 min = box->parameters[0];
//        Vector3 max = box->parameters[1];
//        Vector3 size = max - min;
//        Vector3 center = size / 2 + min;
//        float xdist = (max.x() - min.x()) / 2;
//        float ydist = (max.y() - min.y()) / 2;
//        float zdist = (max.z() - min.z()) / 2;
//        Vector3 h = Vector3(0, ydist, 0);
//
//        //  generate ground floor
//        //
//        Box* b[8];
//        b[0] = new Box(min, center);
//        b[1] = new Box(b[0]->min() + Vector3(xdist, 0, 0), b[0]->max() + Vector3(xdist, 0, 0));
//        b[2] = new Box(b[1]->min() + Vector3(0, 0, zdist), b[1]->max() + Vector3(0, 0, zdist));
//        b[3] = new Box(b[2]->min() + Vector3(-xdist, 0, 0), b[2]->max() + Vector3(-xdist, 0, 0));
//
//        //boxList->clear();
//        for (int i = 0; i < 4; i++)
//            boxList.push_back(b[i]);
//
//        // generate second story
//        //
//        for (int i = 4; i < 8; i++) {
//            b[i] = new Box(b[i - 4]->min() + h, b[i - 4]->max() + h);
//            boxList.push_back(b[i]);
//        }
//    }
//
//    static vector<int> vertsInside(Box* b, ofMesh* mesh, vector<int> verts) {
//
//        // Points inside
//        vector<int> vect;
//
//        if (verts.size() > 0) {
//            // Use passed verts
//            for (int i : verts) {
//                ofVec3f v = mesh->getVertex(i);
//                if (b->pointInside(v)) vect.push_back(i);
//            }
//        }
//        else {
//            // Use all verts
//            for (int i = 0; i < mesh->getNumVertices(); i++) {
//                ofVec3f v = mesh->getVertex(i);
//                if (b->pointInside(v)) vect.push_back(i);
//            }
//        }
//
//        return vect;
//    }

	bool pointInside(ofVec3f v) {
		float minX = min().x, maxX = max().x;
		float minY = min().y, maxY = max().y;
		float minZ = min().z, maxZ = max().z;

		return (v.x > minX && v.y > minY && v.z > minZ &&
			v.x < maxX && v.y < maxY && v.z < maxZ);
	}
};

#endif /* Box_hpp */
