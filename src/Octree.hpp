//
//  Octree.hpp
//  my3DTerrain
//
//  Octree Implementation done by brandonpelfrey
//  https://github.com/brandonpelfrey/SimpleOctree
//
//  Added Box to implementation, along with draw and hilight intersection
//  Created by Christopher Kang on 4/18/18.
//

#ifndef Octree_hpp
#define Octree_hpp

#include <stdio.h>
#include "ofMain.h"
#include "OctreePoint.hpp"

class Octree {
    ofVec3f origin;
    ofVec3f halfDimension;
    Box box;
    
    // tree has up to 8 children to a point
    Octree *children[8];
    OctreePoint *data;
    /* // CHANGE TO PROPER PATTERN OCTANT NUMBERING //
     Children follow a predictable pattern to make accesses simple.
     Here, - means less than 'origin' in that dimension, + means greater than.
     child:    0 1 2 3 4 5 6 7
     x:        - - - - + + + +
     y:        - - + + - - + +
     z:        - + - + - + - +
     */
    
public:
    Octree(const ofVec3f& origin, const ofVec3f& halfDimension, const Box& box) : origin(origin), halfDimension(halfDimension), box(box), data(NULL) {
        for (int i=0; i<8; ++i)
            children[i] = NULL;
    }
    
    Octree(const Octree& copy) : origin(copy.origin), halfDimension(copy.halfDimension), box(box), data(copy.data) {}
    
    ~Octree() {
        for (int i=0; i<8; ++i)
            delete children[i];
    }
    
    // determine octant of tree that would contain the point
    /*
     
     */
    int getOctantContainingPoint(const ofVec3f& point) const {
        int oct = 0;
        if (point.x >= origin.x) oct |= 4;
        if (point.y >= origin.y) oct |= 2;
        if (point.z >= origin.z) oct |= 1;
        return oct;
    }
    
    bool isLeafNode() const {
        for (int i=0; i<8; ++i) {
            if (children[i] != NULL)
                return false;
        }
        return true;
//        return children[0] == NULL;
    }
    
    void insert(OctreePoint* point) {
        if (isLeafNode()) { // if this node doesnt have children, it is a leaf, can insert
            if (data == NULL) { // if no current data, insert data
                data = point;
                return;
            } else { // if current data exists, create 8 new nodes, and replace old data
                OctreePoint *oldPoint = data;
                data = NULL;
                
                for (int i=0; i< 8; ++i) {
                    ofVec3f newOrigin = origin;
                    newOrigin.x += halfDimension.x * (i&4 ? .5f : -.5f);
                    newOrigin.y += halfDimension.y * (i&2 ? .5f : -.5f);
                    newOrigin.z += halfDimension.z * (i&1 ? .5f : -.5f);
                    
                    Box newBox = Box(
                        ofVec3f(
                            newOrigin.x - halfDimension.x*.5f,
                            newOrigin.y - halfDimension.y*.5f,
                            newOrigin.z - halfDimension.z*.5f
                        ),
                        ofVec3f(
                            newOrigin.x + halfDimension.x*.5f,
                            newOrigin.y + halfDimension.y*.5f,
                            newOrigin.z + halfDimension.z*.5f
                        )
                    );
                    children[i] = new Octree(newOrigin, halfDimension*.5f, newBox);
                }
                
                children[getOctantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
                children[getOctantContainingPoint(point->getPosition())]->insert(point);
            }
        } else { // if not a leaf, get correct child to insert into
            int octant = getOctantContainingPoint(point->getPosition());
            children[octant]->insert(point);
        }
    }
    
    void getPointsInsideBox(const ofVec3f bmin, const ofVec3f bmax, std::vector<OctreePoint*>& results) {
        if (isLeafNode()) {
            if (data != NULL) {
                const ofVec3f p = data->getPosition();
                if(p.x > bmax.x || p.y > bmax.y || p.z > bmax.z) return;
                if(p.x < bmin.x || p.y < bmin.y || p.z < bmin.z) return;
                results.push_back(data);
            }
        } else {
            for (int i=0; i<8; ++i) {
                ofVec3f cmax = children[i]->origin + children[i]->halfDimension;
                ofVec3f cmin = children[i]->origin - children[i]->halfDimension;
                
                if(cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z) continue;
                if(cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z) continue;

                children[i]->getPointsInsideBox(bmin,bmax,results);
            }
        }
    }
    
    void findRayIntersection(const Ray& ray, std::vector<ofVec3f>& results) {
        if (isLeafNode()) { // If Leaf Node
            if (data != NULL) {
                if (box.intersect(ray, -100, 100)) {
//                    b = Box(box.min(), box.max());
                    results.push_back(data->getPosition());
//                    cout << "intersect with ray at: " << data->getPosition() << endl;
                }
            }
        } else { // Interior Node, check for leaf nodes
            for (int i=0; i<8; ++i) {
                children[i]->findRayIntersection(ray, results);
            }
        }
    }
    
    void findPointIntersection(const ofVec3f& p, float epsilon, std::vector<ofVec3f>& results) {
        
        if (isLeafNode()) {
            if (data != NULL) {
                if (ofDist(origin.x, origin.y, origin.z, p.x, p.y, p.z) < epsilon) {
                    results.push_back(data->getPosition());
                } else {
                    if(p.x > box.max().x || p.y > box.max().y || p.z > box.max().z) return;
                    if(p.x < box.min().x || p.y < box.min().y || p.z < box.min().z) return;
                    results.push_back(data->getPosition());
                }
            }
        } else {
            for (int i=0; i<8; i++) {
                children[i]->findPointIntersection(p, epsilon, results);
            }
        }
    }
    
    void drawOctree() {
        if (isLeafNode()) { // If Leaf Node
            if (data != NULL) {
                ofVec3f size = box.parameters[1] - box.parameters[0];
                ofDrawBox(origin, size.x, size.y, size.z);
            }
        } else { // Interior Node, check for leaf nodes
            for (int i=0; i<8; ++i) {
                children[i]->drawOctree();
            }
        }
    }
    
    void drawOctree(int level) {
        if (this == NULL) return;
        if (level == 1) {
            ofVec3f size = box.parameters[1] - box.parameters[0];
            ofDrawBox(origin, size.x, size.y, size.z);
        }
        else if (level > 1) {
            for (int i=0; i<8; ++i) {
                children[i]->drawOctree(level - 1);
            }
        }
    }
    
};

#endif /* Octree_hpp */
