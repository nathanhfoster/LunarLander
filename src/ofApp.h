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
    void loadVbo();

    bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
    
    ofEasyCam cam;
    ofxAssimpModelLoader mars, lander;
    ofMesh marsMesh;
    
    // nathan
    Particle spaceship;
    ParticleEmitter exhaust;
    ParticleSystem sys;
    ThrusterForce thruster;
    ofSoundPlayer thrust_sound;
    
    // chris kang
    void setupCam();
    void setupBools();
    void setupEnv();
    void setupLander();
    void setupSceneLights();
    void toggleCam(int option);
    void loadVbo();
    
    void checkCollision();
    
    void createOctree(const ofMesh & mesh);
    float getAGL();
    
    ofLight light, keyLight, rimLight, fillLight;
    
    bool showOctree, collided;
    ofVec3f closestPoint;
    int level, camOption = 1;
    Octree *octree;
    OctreePoint *octreePoints;
    
    ImpulseForce collisionForce;
    ofImage spacefield;
    
    bool bAltKeyDown = false;
    bool bCtrlKeyDown = false;
    bool bWireframe = false;
    bool bDisplayPoints = false;
    bool bPointSelected;
    
    bool bRoverLoaded = false;
    bool bTerrainSelected = true;
    
    ofVec3f selectedPoint;
    ofVec3f intersectPoint;
    ofVec3f sideCamPosition;
    ofVec3f downView;
    
    //samnang's shader
    ofShader shader;
    ofVbo vbo;
    
    // textures
    //
    ofTexture  particleTex;
    
    const float selectionRange = 4.0;
};
