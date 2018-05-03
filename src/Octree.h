#pragma once
#include "ofMain.h"
#include "ray.h"
#include "box.h"
#include <map>

class TreeNode {
public:
	bool shouldDraw = false;
	bool last = false;
	Box* box = nullptr;
	ofMesh* mesh = nullptr;
	vector<int> verts;
	TreeNode* children[8] = { nullptr };
	int depth = 0;
	map<int, ofColor> colors;

	TreeNode(Box* newBox, int newDepth, ofMesh* newMesh, bool checkAll, vector<int> possibleVerts);
	bool checkIntersection(Ray ray);
	bool isLeaf();
	void draw();
	void undraw();
	void drawBox();
	ofColor getColor(int d);
};

class Octree {
	TreeNode* head;
	Octree(int depth) { }
};