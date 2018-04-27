#include "Octree.h"

TreeNode::TreeNode(Box* newBox, int newDepth, ofMesh* newMesh, bool checkAll, vector<int> availableVerts) : box(newBox), mesh(newMesh), depth(newDepth) {
	colors = { 
		{0, ofColor::blue}, 
		{1, ofColor::crimson}, 
		{2, ofColor::cyan},
		{3, ofColor::red}, 
		{5, ofColor::bisque},
		{6, ofColor::darkSeaGreen}, 
		{7, ofColor::yellow},
		{8, ofColor::darkCyan}, 
		{9, ofColor::burlyWood},
		{10, ofColor::red} 
	};

	verts = Box::vertsInside(box, mesh, availableVerts);


	if (verts.size() > 1 && depth > 0) {
		// Use the subdivide function to divide a box into children
		vector<Box*> childBoxes;
		Box::subDivideBox8(box, childBoxes);

		for (int i = 0; i < 8; i++) {
			children[i] = new TreeNode(childBoxes.at(i), depth - 1, mesh, false, verts);
		}
	}
	else if (verts.size() == 1 || depth == 0) {
		last = true;
	}
}

void TreeNode::checkIntersection(Ray ray) {
	if (this->box->intersect(ray, -100, 100)) {
		shouldDraw = true;
		if (!isLeaf()) {
			for (TreeNode* c : children) {
				c->checkIntersection(ray);
			}
		}
	}
}

bool TreeNode::isLeaf() { return children[0] == nullptr; }

void TreeNode::draw() {
	if (!shouldDraw) return;

	drawBox();

	if (!isLeaf()) {
		for (TreeNode* child : children) {
			child->draw();
		}
	}
}

void TreeNode::undraw() {
	if (shouldDraw) {
		shouldDraw = false;
		last = false;
		if (!isLeaf()) {
			for (TreeNode* child : children) {
				child->undraw();
			}
		}
	}
}

void TreeNode::drawBox() {
	if (box) {
		Vector3 min = box->parameters[0];
		Vector3 max = box->parameters[1];
		Vector3 size = max - min;
		Vector3 center = size / 2 + min;
		ofVec3f clickPoint = ofVec3f(center.x(), center.y(), center.z());
		float X = size.x();
		float Y = size.y();
		float Z = size.z();
		ofSetColor(TreeNode::getColor(depth));
		ofDrawBox(clickPoint, X, Y, Z);
		if (last) {
			ofSetColor(ofColor::darkMagenta);
			ofDrawSphere(center.x(), center.y(), center.z(), 0.05);
		}
	}
	else {
		cout << "Box was missing!" << endl;
	}
}

ofColor TreeNode::getColor(int d) {
	return colors[d];
}