#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.hpp"
#include "Box.hpp"
#include "Ray.hpp"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool  doPointSelection();
		void drawBox(const Box &box);
		Box meshBounds(const ofMesh &);
		void subDivideBox8(const Box &b, vector<Box>& boxList);

		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);

		ofEasyCam cam;
		ofxAssimpModelLoader mars, lander;

		Particle spaceship;
		ParticleEmitter exhaust;
		ParticleSystem sys;
		ThrusterForce thruster;
		ofSoundPlayer thrust_sound;

		ofMesh marsMesh;
        vector<vector<Box>> levels;
    
    // chris kang
    void setupCam();
    void setupBools();
    void setupEnv();
    void setupLander();
    void setupSceneLights();
    
    void checkCollision();
    
    void createOctree(const ofMesh & mesh);
    float getAGL();

    ofLight light, keyLight, rimLight, fillLight;
    
    bool showOctree;
    ofVec3f closestPoint;
    int level;
    Octree *octree;
    OctreePoint *octreePoints;
    
    ImpulseForce collisionForce;

    
		bool bAltKeyDown = false;
		bool bCtrlKeyDown = false;
		bool bWireframe = false;
		bool bDisplayPoints = false;
		bool bPointSelected;
		
		bool bRoverLoaded = false;
		bool bTerrainSelected = true;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;


		const float selectionRange = 4.0;

//        TreeNode* octree;
};
