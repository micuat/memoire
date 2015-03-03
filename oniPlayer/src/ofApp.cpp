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
}

//--------------------------------------------------------------
void ofApp::update(){
    if(openNIPlayer) openNIPlayer->update();
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
		int w = 640;
		int h = 480;
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_POINTS);
		int step = 2;
		for(int y = 0; y < h; y += step) {
			for(int x = 0; x < w; x += step) {
				ofVec3f v;
				
				const ofShortPixels& pix = openNIPlayer->getDepthRawPixels();
				const unsigned short *ptr = pix.getPixels();
				unsigned short z = ptr[pix.getWidth() * y + x];
				v = openNIPlayer->projectiveToWorld(ofVec3f(x, y, z));
				if(v.z > 0 && v.z < 4500) {
					mesh.addColor(openNIPlayer->getImagePixels().getColor(x, y));
					mesh.addVertex(v);
				}
			}
		}
		glPointSize(2);
		ofPushMatrix();
		// the projected points are 'upside down' and 'backwards'
		ofScale(1, 1, -1);
		ofTranslate(0, 0, -2000); // center the points a bit
		ofEnableDepthTest();
		mesh.drawVertices();
		ofDisableDepthTest();
		ofPopMatrix();

		cam.end();
	}
	ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
	verdana.drawString(msg, 20, 2 * 480 - 20);
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
    if(openNIPlayer) openNIPlayer->stop();
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
			fileIndex = (fileIndex-1)%dir.numFiles();
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
        case 'x':
            if(openNIPlayer) openNIPlayer->stop();
            break;
    }

}

//--------------------------------------------------------------
void ofApp::playOpenNI(){
	if(openNIPlayer) {
		openNIPlayer->waitForThread(true);
		openNIPlayer->setPaused(true);
		openNIPlayer->stop();
	}
	openNIPlayer = ofPtr<ofxOpenNI>(new ofxOpenNI);
	openNIPlayer->setup();
	openNIPlayer->start();
	openNIPlayer->startPlayer(dir.getPath(fileIndex));
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