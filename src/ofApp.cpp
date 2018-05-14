//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
//
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like
//  the input interface to be the same for each student's
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//
//  Student Name:  Christopher Kang
//  Date: 5/8/2018


#include "ofApp.h"
#include "Util.h"


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
    setupBools();
    setupCam();
    setupEnv();
    setupLander();
    setupSceneLights();

    if (soundPlayer.load("sounds/engine.wav")) {
		soundFileLoaded = true;
		soundPlayer.setMultiPlay(true);
	}
    
    cam.setTarget(ofVec3f(spaceship.position.x, spaceship.position.y/2, spaceship.position.z));
    
    
    exhaust.visible = false;
    
    exhaust.setMass(1);
    exhaust.setRandomLife(false);
    exhaust.setOneShot(true);
    exhaust.radius = 0.5;
    exhaust.setParticleRadius(.05);
   
    exhaust.setEmitterType(DirectionalEmitter);
    exhaust.setGroupSize(100);
    
    // texture loading
    //
    ofDisableArbTex();     // disable rectangular textures

    // load textures
    //
    if (!ofLoadImage(particleTex, "images/dot.png")) {
        cout << "Particle Texture File: images/dot.png not found" << endl;
        ofExit();
    }
    
    // load the shader
    //
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
    
    spacefield.loadImage("images/spacefield.jpg");
    
    mars.loadModel("geo/moon-houdini.obj");
    mars.setScaleNormalization(false);
    marsMesh = mars.getMesh(0);
    
    createOctree(mars.getMesh(0));
    level = 1;
    showOctree = false;
    
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    //    cout << "Cam Position: " << cam.getPosition() << endl;
    ofVec3f landerVelocity = sys.particles[0].velocity;
    ofVec3f landerPosition = sys.particles[0].position;
    
    // lander
    sys.update();
    exhaust.update();
    lander.setPosition(landerPosition.x, landerPosition.y, landerPosition.z);
    exhaust.setPosition(landerPosition);
    
    // lights
    keyLight.lookAt(landerPosition);
    rimLight.lookAt(landerPosition);
    fillLight.lookAt(landerPosition);
    
    // AGL
    if (!collided) cout << "distance to ground: " << getAGL() << endl << endl;
    
    // collisions
    float particleVel = landerVelocity.length() / ofGetFrameRate();
    cout << "particle vel: " << particleVel << endl;
    vector<ofVec3f> collisionPoints;
    octree->findPointIntersection(landerPosition, particleVel, collisionPoints);
    if (collisionPoints.size() > 0 && !collided) {
        ofVec3f landingForce = -landerVelocity * ofGetFrameRate();
        collisionForce.apply(landingForce);
        cout << "collided" << endl;
        collided = true;
    }
    
    //camera
    toggleCam(camOption);
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own backgroujnd
    //    ofBackground(ofColor::black);
    //    cout << ofGetFrameRate() << endl;
    
    loadVbo();
    
    ofDisableDepthTest();

    spacefield.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    ofEnableDepthTest();
    

    cam.begin();
    
    ofPushMatrix();
    if (bWireframe) {                    // wireframe mode  (include axis)
        ofDisableLighting();
        ofSetColor(ofColor::slateGray);
        mars.drawWireframe();
        if (bRoverLoaded) {
            lander.drawWireframe();
            if (!bTerrainSelected) drawAxis(lander.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    else {
        ofEnableLighting();              // shaded mode
        mars.drawFaces();
        
        if (bRoverLoaded) {
            lander.drawFaces();
            if (!bTerrainSelected) drawAxis(lander.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    
    
    if (bDisplayPoints) {                // display points as an option
        glPointSize(3);
        ofSetColor(ofColor::green);
        mars.drawVertices();
    }
    
    // highlight selected point (draw sphere around selected point)
    //
    if (bPointSelected) {
        ofSetColor(ofColor::blue);
        ofDrawSphere(selectedPoint, .1);
    }
    
    ofNoFill();
//    ofSetColor(255,255,255);
    
    
    
    //    keyLight.draw();
    //    fillLight.draw();
    //    rimLight.draw();
    //
    //    switch (level) {
    //        case 1:
    //            ofSetColor(255, 255, 255);
    //            break;
    //        case 2:
    //            ofSetColor(0, 255, 0);
    //            break;
    //        case 3:
    //            ofSetColor(0, 0, 255);
    //            break;
    //        case 4:
    //            ofSetColor(255, 255, 0);
    //            break;
    //        case 5:
    //            ofSetColor(0, 255, 255);
    //            break;
    //        case 6:
    //            ofSetColor(255, 0, 0);
    //            break;
    //        case 7:
    //            ofSetColor(100, 255, 125);
    //            break;
    //        case 8:
    //            ofSetColor(67, 100, 125);
    //            break;
    //        case 9:
    //            ofSetColor(125, 0, 200);
    //            break;
    //    }
    //    if (showOctree) octree->drawOctree(level);
    ofDrawSphere(closestPoint, 1);
    
//    exhaust.draw();

    ofPopMatrix();
    cam.end();


    shader.begin();
    cam.begin();
    ofSetColor(255, 255, 255);
    
    // this makes everything look glowy :)
    //
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();

    particleTex.bind();
    vbo.draw(GL_POINTS, 0, (int)exhaust.sys->particles.size());
    particleTex.unbind();

    ofDisablePointSprites();
    ofDisableBlendMode();
    ofEnableAlphaBlending();
    //    sys.draw();
    cam.end();
    shader.end();
}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
    if (exhaust.sys->particles.size() < 1) return;
    
    vector<ofVec3f> sizes;
    vector<ofVec3f> points;
    for (int i = 0; i < exhaust.sys->particles.size(); i++) {
        points.push_back(exhaust.sys->particles[i].position);
        sizes.push_back(ofVec3f(10,10,10));
    }
    // upload the data to the vbo
    //
    int total = (int)points.size();
    vbo.clear();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}


// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {
    
    ofPushMatrix();
    ofTranslate(location);
    
    ofSetLineWidth(1.0);
    
    // X Axis
    ofSetColor(ofColor(255, 0, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
    
    
    // Y Axis
    ofSetColor(ofColor(0, 255, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
    
    // Z Axis
    ofSetColor(ofColor(0, 0, 255));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
    
    ofPopMatrix();
}


void ofApp::keyPressed(int key) {
    switch (key) {
        case '1':
            camOption = 1;
            break;
        case '2':
            camOption = 2;
            break;
        case '3':
            camOption = 3;
            break;
        case '4':
            camOption = 4;
            break;
        case '5':
            camOption = 5;
            break;
        case '6':
            camOption = 6;
            break;
        case '7':
            camOption = 7;
            break;
        case '-':
            level -= 1;
            break;
        case '=':
            level += 1;
            break;
        case '0':
            showOctree = !showOctree;
            break;
        case 'O':
        case 'o':
			playSound();
            thruster.add(ofVec3f(0, 1, 0));
            exhaust.setVelocity(ofVec3f(0, -10, 0));
            exhaust.start();
            break;
        case 'L':
        case 'l':
			playSound();
            thruster.add(ofVec3f(0, -1, 0));
            exhaust.setVelocity(ofVec3f(0, 10, 0));
//            exhaust.start();
            break;
        case 'K':
        case 'k':
			playSound();
            thruster.add(ofVec3f(-1, 0, 0));
            exhaust.setVelocity(ofVec3f(10, 0, 0));
//            exhaust.start();
            break;
        case ';':
			playSound();
            thruster.add(ofVec3f(1, 0, 0));
            exhaust.setVelocity(ofVec3f(-10, 0, 0));
//            exhaust.start();
            break;
        case 'U':
        case 'u':
			playSound();
            thruster.add(ofVec3f(0, 0, 1));
            exhaust.setVelocity(ofVec3f(0, 0, -10));
//            exhaust.start();
            break;
        case 'I':
        case 'i':
        	playSound();
            thruster.add(ofVec3f(0, 0, -1));
            exhaust.setVelocity(ofVec3f(0, 0, 10));
//            exhaust.start();
            break;
            
        case 'C':
        case 'c':
            if (cam.getMouseInputEnabled()) cam.disableMouseInput();
            else cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            break;
        case 'r':
            cam.reset();
            break;
        case 's':
            savePicture();
            break;
        case 't':
            setCameraTarget();
            break;
        case 'v':
            togglePointsDisplay();
            break;
        case 'V':
            break;
        case 'w':
            toggleWireframeMode();
            break;
        case OF_KEY_ALT:
            cam.enableMouseInput();
            bAltKeyDown = true;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_DEL:
            break;
        case GLFW_KEY_SPACE:
            break;
        default:
            break;
    }
}

void ofApp::toggleWireframeMode() {
    bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
    bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
    bDisplayPoints = !bDisplayPoints;
}

void ofApp::toggleCam(int option) {
    ofVec3f landerPosition = sys.particles[0].position;
    switch (option) {
        case 1: {
            cam.enableMouseInput();
            cam.setPosition(ofVec3f(10, 100, 100));
            cam.lookAt(lander.getPosition());
            break;
        }
        case 2: {
            cam.disableMouseInput();
            cam.setPosition(landerPosition + ofVec3f(-43,90,31));
            cam.lookAt(closestPoint);
            break;
        }
        case 3: {
            cam.disableMouseInput();
            cam.setPosition(ofVec3f(85, 146, 133));
            cam.lookAt(landerPosition);
            break;
        }
        case 4: {
            cam.disableMouseInput();
            cam.setPosition(285, 32, -93);
            cam.lookAt(landerPosition);
            break;
        }
        case 5: {
            cam.disableMouseInput();
            cam.setPosition(landerPosition + ofVec3f(60,0,60));
            cam.lookAt(landerPosition);
            break;
        }
        case 6: {
            cam.disableMouseInput();
            cam.setPosition(lander.getPosition().x, lander.getPosition().y+3, lander.getPosition().z);
            downView = ofVec3f(lander.getPosition().x ,mars.getPosition().y, lander.getPosition().z);
            cam.setTarget(downView);
            break;
        }
        case 7: {
            cam.disableMouseInput();
            cam.setPosition(lander.getPosition().x, lander.getPosition().y+0.5, lander.getPosition().z);
            sideCamPosition = ofVec3f(lander.getPosition().x, lander.getPosition().y+0.5, lander.getPosition().z - 1);
            cam.setTarget(sideCamPosition);
        }
        default:
            break;
    }
}

void ofApp::keyReleased(int key) {
    exhaust.stop();
    stopSound();
    thruster.set(ofVec3f(0, 0, 0));
    switch (key) {
            
        case OF_KEY_ALT:
            cam.disableMouseInput();
            bAltKeyDown = false;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_SHIFT:
            break;
        default:
            break;
            
    }
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    if (bAltKeyDown) return;
}


//draw a box from a "Box" class
//
void ofApp::drawBox(const Box &box) {
    ofVec3f min = box.parameters[0];
    ofVec3f max = box.parameters[1];
    ofVec3f size = max - min;
    ofVec3f center = size / 2 + min;
    ofVec3f p = ofVec3f(center.x, center.y, center.z);
    float w = size.x;
    float h = size.y;
    float d = size.z;
    ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    return Box(ofVec3f(min.x, min.y, min.z), ofVec3f(max.x, max.y, max.z));
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    
}

// Set the camera to use the selected point as it's new target
//
void ofApp::setCameraTarget() {
    
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::savePicture() {
    ofImage picture;
    picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    picture.save("screenshot.png");
    cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
    ofVec3f point;
    mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
    
    if (lander.loadModel(dragInfo.files[0])) {
        lander.setScaleNormalization(false);
        lander.setScale(.005, .005, .005);
        lander.setPosition(point.x, point.y, point.z);
        bRoverLoaded = true;
    }
    else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
    ofVec2f mouse(mouseX, mouseY);
    ofVec3f rayPoint = cam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - cam.getPosition();
    rayDir.normalize();
    return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

// create octree
void ofApp::createOctree(const ofMesh & mesh) {
    // get mesh points
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    Box boundingBox;
    vector<ofVec3f> points;
    
    for (int i = 1; i < n; i++) {
        points.push_back(mesh.getVertex(i)); // push mesh point into a vector
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    boundingBox = Box(ofVec3f(min.x, min.y, min.z), ofVec3f(max.x, max.y, max.z));
    
    // create new octree, takes in center and half of dimension
    octree = new Octree(
                        ofVec3f(
                                (min.x + max.x) / 2,
                                (min.y + max.y) / 2,
                                (min.z + max.z) / 2
                                ),
                        ofVec3f(
                                (max.x - min.x) / 2,
                                (max.y - min.y) / 2,
                                (max.z - min.z) / 2
                                ),
                        boundingBox
                        );
    
    // insert points into octree
	octreePoints = new OctreePoint[n];
	for (int i = 0; i< sizeof(octreePoints); i++) {
		octreePoints[i].setPosition(points[i]);
		octree->insert(octreePoints + i);
	}
}

float ofApp::getAGL() {
    vector<ofVec3f> foundPoints;
    //    std::vector<ofVec3f>().swap(foundPoints);
    ofVec3f spaceshipPosition = sys.particles[0].position;
    ofVec3f rayPoint = spaceshipPosition;
    ofVec3f rayDir = ofVec3f(0, -1, 0).normalize();
    Ray ray = Ray(rayPoint, rayDir);
    float closestDist = -1;
    
    // find closest point
    octree->findRayIntersection(ray, foundPoints);
    if (foundPoints.size() > 0) {
        closestPoint = foundPoints[0];
    } else {
        closestPoint = ofVec3f(rayPoint.x, 0, rayPoint.z);
    }
    for (int i=0; i<foundPoints.size(); i+=1) {
        ofVec3f rayToPoint = foundPoints[i] - rayPoint;
        float mag = rayToPoint.dot(rayDir);
        float length = rayToPoint.dot(rayToPoint);
        float b = mag/length;
        ofVec3f pointOnRay = rayPoint + (b * rayDir);
        float dist = ofDist(foundPoints[i].x, foundPoints[i].y, foundPoints[i].z, pointOnRay.x, pointOnRay.y, pointOnRay.z);
        if (closestDist == -1) closestDist = dist;
        if (dist < closestDist) {
            closestDist = dist;
            closestPoint = foundPoints[i];
        }
    }
    
    return ofDist(closestPoint.x, closestPoint.y, closestPoint.z, rayPoint.x, rayPoint.y, rayPoint.z);
}

void ofApp::setupBools() {
    bWireframe = false;
    bDisplayPoints = false;
    bAltKeyDown = false;
    bCtrlKeyDown = false;
    bRoverLoaded = false;
    bTerrainSelected = true;
}

void ofApp::setupCam() {
    cam.setDistance(10);
    cam.setNearClip(.1);
    cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
    cam.setPosition(ofVec3f(175, 75, 0));
    cam.disableMouseInput();
}

void ofApp::setupEnv() {
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableDepthTest();
    ofEnableLighting();
}

void ofApp::setupSceneLights() {
    initLightingAndMaterials();
    keyLight.setup();
    keyLight.enable();
    keyLight.setAreaLight(1, 1);
    keyLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
    keyLight.setDiffuseColor(ofFloatColor(1, 1, 1));
    keyLight.setSpecularColor(ofFloatColor(1, 1, 1));
    keyLight.setPosition(cam.getPosition());
    keyLight.lookAt(spaceship.position);
    
    fillLight.setup();
    fillLight.enable();
    fillLight.setPointLight();
    fillLight.setAttenuation(1, .001, .001);
    fillLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
    fillLight.setDiffuseColor(ofFloatColor(.5, .5, .5));
    fillLight.setSpecularColor(ofFloatColor(.7,.7,.7));
    fillLight.lookAt(spaceship.position);
    fillLight.setPosition(0, spaceship.position.y - 100, 150);
    
    rimLight.setup();
    rimLight.enable();
    rimLight.setPointLight();
    rimLight.setAttenuation(.2, .001, .001);
    rimLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
    rimLight.setDiffuseColor(ofFloatColor(.5, .5, .5));
    rimLight.setSpecularColor(ofFloatColor(.7,.7,.7));
    rimLight.lookAt(spaceship.position);
    rimLight.setPosition(-50, spaceship.position.y, -150);
}

void ofApp::setupLander() {
    spaceship.position = ofVec3f(0, 50, 0);
    spaceship.lifespan =  -1;
    sys.add(spaceship);
    sys.addForce(&thruster);
    sys.addForce(&collisionForce);
    
    lander.loadModel("geo/lander.obj");
    bRoverLoaded = true;
    lander.setScaleNormalization(false);
    lander.setScale(5, 5, 5);
    lander.setPosition(spaceship.position.x, spaceship.position.y, spaceship.position.z);
    
    exhaust.visible = false;
	//exhaust.sys->addForce(new TurbulenceForce(ofVec3f(-5, -5, -5), ofVec3f(5, 5, 5)));
	exhaust.sys->addForce(new ImpulseRadialForce(10));
	exhaust.sys->addForce(new CyclicForce(10));
    exhaust.setMass(1);
    exhaust.setLifespan(1.5);
    exhaust.radius = 15;
    exhaust.setParticleRadius(3);
    exhaust.setRate(20);
    exhaust.setEmitterType(DirectionalEmitter);
}

//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {
    
    static float ambient[] =
    { .5f, .5f, .5, 1.0f };
    static float diffuse[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float position[] =
    {5.0, 5.0, 5.0, 0.0 };
    
    static float lmodel_ambient[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float lmodel_twoside[] =
    { GL_TRUE };
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
}

void ofApp::playSound() {
	if (soundFileLoaded) soundPlayer.play();
}

void ofApp::stopSound() {
	if (soundFileLoaded) soundPlayer.stop();
}