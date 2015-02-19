#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    
    width = 640;
    height = 480;

    // Initial Allocation
    //
    fluid.allocate(width, height, 0.5);
    
    // Seting the gravity set up & injecting the background image
    //
    fluid.dissipation = 0.95;
    fluid.velocityDissipation = 1;
    
    fluid.setGravity(ofVec2f(0.0,0.0));
	
    ofSetWindowShape(width, height);
	
	syphonServer.setName("Screen Output");
}

//--------------------------------------------------------------
void testApp::update(){
    
	//fluid.addColor(backImage);
	
    // Adding temporal Force
    //
    ofPoint m = ofPoint(mouseX,mouseY);
    ofPoint d = (m - oldM)*1.0;
    oldM = m;
    ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
    c.normalize();
	fluid.addTemporalForce(m, d, ofFloatColor(0, 0.1, 0.2, 0.2), 10.0f);
	fluid.addTemporalForce(m, d, ofFloatColor(0, 0.1, 0.2, 0.05), 20.0f);
	fluid.addTemporalForce(m, d, ofFloatColor(0, 0.1, 0.2, 0.01), 30.0f);

    //  Update
    //
    fluid.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    //ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_LINEAR);
    
    fluid.draw();
	
	syphonServer.publishScreen();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}