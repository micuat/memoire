#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("Screen Output","liquide");
	
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
	
	dir.setup();
	//register for our directory's callbacks
	ofAddListener(dir.events.serverAnnounced, this, &testApp::serverAnnounced);
	// not yet implemented
	//ofAddListener(dir.events.serverUpdated, this, &testApp::serverUpdated);
	ofAddListener(dir.events.serverRetired, this, &testApp::serverRetired);

	serverAvailable = true;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	if(serverAvailable)
		mClient.draw(0, 0, ofGetWidth(), ofGetHeight());
}


//--------------------------------------------------------------
void testApp::keyPressed (int key){
	if(key == 'f') ofToggleFullscreen();
}

//these are our directory's callbacks
void testApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
	for( auto& dir : arg.servers ){
		ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
	}
	
	serverAvailable = true;
}

void testApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
	for( auto& dir : arg.servers ){
		ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
	}
	
	serverAvailable = false;
}
