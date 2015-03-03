#include "ofApp.h"

class myUser : public ofxOpenNIUser {
public:
    void test(){
        cout << "test" << endl;
    }
};

//--------------------------------------------------------------
void ofApp::setup() {
    
 
    ofSetLogLevel(OF_LOG_VERBOSE);
	
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
	
	dir.open(ofToDataPath(""));
	dir.allowExt("oni");
	dir.listDir();
	fileIndex = 0;
	
	cam.setFarClip(100000);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(openNIPlayer) openNIPlayer->update();
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	ofSetColor(255, 255, 255);
    
    ofPushMatrix();
	
	if(ofGetKeyPressed('a'))
		if(openNIPlayer) openNIPlayer->drawDebug(0, 0);

//	ofViewport(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight()/2);
	ofPushMatrix();
	
	if(openNIPlayer && openNIPlayer->isPlaying()) {
		cam.begin();
		
		ofMesh mesh;
		mesh = ofxPCL::organizedFastMesh(openNIPlayer->getImagePixels(), openNIPlayer->getDepthRawPixels(), 2, 1);
		ofPushMatrix();
		// the projected points are 'upside down' and 'backwards'
		ofScale(1, -1, -1);
		ofTranslate(0, 0, -2000); // center the points a bit
		ofEnableDepthTest();
		mesh.drawWireframe();
		ofDisableDepthTest();

		ofScale(1, -1, 1);
		int numUsers = openNIPlayer->getNumTrackedUsers();

		for (int i = 0; i < numUsers; i++){
			
			// get a reference to this user
			ofxOpenNIUser & user = openNIPlayer->getTrackedUser(i);
			
			for(int j = 0; j < user.getNumJoints(); j++){
				ofxOpenNIJoint joint = user.getJoint((Joint)j);
				ofPushStyle();
				
				ofSetColor(ofColor::darkBlue, 100);
				ofSetLineWidth(5);
				if(joint.isParent()){
					ofLine(joint.getWorldPosition(), joint.getParent().getWorldPosition());
				}
				
				ofPopStyle();
			}
		}
		ofPopMatrix();
		
		cam.end();
		

	}
}

//--------------------------------------------------------------
void ofApp::userEvent(ofxOpenNIUserEvent & event){
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void ofApp::gestureEvent(ofxOpenNIGestureEvent & event){
    ofLogNotice() << event.gestureName << getGestureStatusAsString(event.gestureStatus) << "from device" << event.deviceID << "at" << event.timestampMillis;
}

//--------------------------------------------------------------
void ofApp::exit(){
    //if(openNIPlayer) openNIPlayer->stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    int cloudRes = -1;
    switch (key) {
		case OF_KEY_RIGHT:
			fileIndex = (fileIndex+1)%dir.numFiles();
			playOpenNI();
			break;
		case OF_KEY_LEFT:
			fileIndex = (fileIndex-1+dir.numFiles())%dir.numFiles();
			playOpenNI();
			break;
        case 'p':
			playOpenNI();
            break;
        case '/':
            if(openNIPlayer) openNIPlayer->setPaused(!openNIPlayer->isPaused());
            break;
        case 'm':
            if(openNIPlayer) openNIPlayer->firstFrame();
            break;
        case '<':
        case ',':
            if(openNIPlayer) openNIPlayer->previousFrame();
            break;
        case '>':
        case '.':
            if(openNIPlayer) openNIPlayer->nextFrame();
            break;
    }

}

//--------------------------------------------------------------
void ofApp::playOpenNI(){
	if(openNIPlayer) {
		openNIPlayer->waitForThread(true);
		openNIPlayer->setPaused(true);
		openNIPlayer->removeUserGenerator();
		openNIPlayer->stop();
	}
	openNIPlayer = ofPtr<ofxOpenNI>(new ofxOpenNI);
	openNIPlayer->setupFromONI(dir.getPath(fileIndex));
	openNIPlayer->addImageGenerator();
	openNIPlayer->addDepthGenerator();
	openNIPlayer->setRegister(true);
	openNIPlayer->addUserGenerator();
	openNIPlayer->setMaxNumUsers(10);
	openNIPlayer->start();
	openNIPlayer->setUseDepthRawPixels(true);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}