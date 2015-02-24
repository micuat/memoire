#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("Screen Output","liquide");
	
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    mClient.draw(0, 0, ofGetWidth(), ofGetHeight());
}


//--------------------------------------------------------------
void testApp::keyPressed (int key){
	if(key == 'f') ofToggleFullscreen();
}
