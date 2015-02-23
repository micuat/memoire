#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	openNIDevice.setup();
	openNIDevice.addImageGenerator();
	openNIDevice.addDepthGenerator();
	openNIDevice.setRegister(true);
	openNIDevice.setMirror(true);
	openNIDevice.addUserGenerator();
	openNIDevice.setMaxNumUsers(10);
	openNIDevice.start();
	
	// set properties for all user masks and point clouds
	//openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
	//openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
	//openNIDevice.setUsePointCloudsAllUsers(true);
	//openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
	//openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
	
	ofAddListener(openNIDevice.userEvent, this, &testApp::userEvent);
	
    width = 640;
    height = 480;

    // Initial Allocation
    //
    fluid.allocate(width, height, 0.5);
    
    // Seting the gravity set up & injecting the background image
    //
	fluid.dissipation = 0.95;
    fluid.velocityDissipation = 0.99;
    
    fluid.setGravity(ofVec2f(0.0,0.0));
	
    ofSetWindowShape(width, height);
	
	syphonServer.setName("Screen Output");
	
	oscSender.setup(HOST, PORT);
	
	gui.setup();
	gui.add(label.setup("", ""));
	gui.add(disappearRate.setup("Disappear Rate", 0.95f, 0.9f, 1));
	gui.add(inertiaRate.setup("Inertia Rate", 0.99, 0.8f, 1));
	gui.add(blobSize.setup("Blob Size", 2, 1, 4));
	gui.add(clearUsers.setup("Clear Users", false));
//	gui.setSize(400, 240);
}

//--------------------------------------------------------------
void testApp::update(){
	openNIDevice.update();
	
	fluid.dissipation = disappearRate;
	fluid.velocityDissipation = inertiaRate;

	if(clearUsers) {
		openNIDevice.waitForThread(true);
		openNIDevice.setPaused(true);
		openNIDevice.removeUserGenerator();
		openNIDevice.addUserGenerator();
		openNIDevice.setPaused(false);
		openNIDevice.startThread();
		clearUsers = false;
		
		oldLeftM.clear();
		oldRightM.clear();
		
		fluid.clear();
	}
	
	// get number of current users
	int numUsers = openNIDevice.getNumTrackedUsers();
	
	oldLeftM.resize(numUsers);
	oldRightM.resize(numUsers);
	dieCounts.resize(numUsers, 0);
	
	// iterate through users
	for (int i = 0; i < numUsers; i++){
		
		// get a reference to this user
		ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
		user.setForceResetTimeout(200);
		
		if(user.isCalibrating()) {
			continue;
		}
		
		if(!user.isTracking() || i > (int)oldLeftM.size() - 1) {
			continue;
		}
		
		ofVec2f m;
		m = user.getJoint(JOINT_LEFT_HAND).getProjectivePosition();
		
		ofVec2f d;
		d = (m - oldLeftM.at(i))*1.0;
		
		oldLeftM.at(i) = m;
		fluid.addTemporalForce(m, d, ofFloatColor(0.05, 0.1, 0.2), 5.0f * blobSize);
		fluid.addTemporalForce(m, d, ofFloatColor(0.05, 0.1, 0.2), 10.0f * blobSize);
		
		ofxOscMessage message;
		float th = 5*5;
		if(ofGetFrameNum() % 4 == 1 && d.lengthSquared() > th) {
			message.setAddress("/niw/client/aggregator/floorcontact");
			message.addStringArg("add");
			message.addIntArg(ofGetFrameNum());
			message.addFloatArg(ofMap(m.x, 0, width, 0, 6));
			message.addFloatArg(ofMap(m.y, 0, height, 6, 0));
			oscSender.sendMessage(message);
		}
		message.clear();
		
		m = user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition();
		
		d = (m - oldRightM.at(i))*1.0;
		
		oldRightM.at(i) = m;
		fluid.addTemporalForce(m, d, ofFloatColor(0.05, 0.1, 0.1), 5.0f * blobSize);
		fluid.addTemporalForce(m, d, ofFloatColor(0.05, 0.1, 0.1), 10.0f * blobSize);
		
		if(ofGetFrameNum() % 4 == 3 && d.lengthSquared() > th) {
			message.setAddress("/niw/client/aggregator/floorcontact");
			message.addStringArg("add");
			message.addIntArg(ofGetFrameNum());
			message.addFloatArg(ofMap(m.x, 0, width, 0, 6));
			message.addFloatArg(ofMap(m.y, 0, height, 6, 0));
			oscSender.sendMessage(message);
		}
		if(d.length() == 0 && !user.isCalibrating() && numUsers == openNIDevice.getMaxNumUsers()) {
			dieCounts.at(i)++;
			if(dieCounts.at(i) > 10) {
				openNIDevice.waitForThread(true);
				openNIDevice.setPaused(true);
				openNIDevice.removeUserGenerator();
				openNIDevice.addUserGenerator();
				openNIDevice.setPaused(false);
				openNIDevice.startThread();
				
				dieCounts.clear();
				oldLeftM.clear();
				oldRightM.clear();
				
				fluid.clear();
				return;
			}
		} else {
			dieCounts.at(i) = 0;
		}
	}
	
    //  Update
    //
    fluid.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDisableAlphaBlending();
	
    fluid.draw();
	
	syphonServer.publishScreen();
	
	int numUsers = openNIDevice.getNumTrackedUsers();
	
	if(!openNIDevice.isDepthOn()) {
		label = "Kinect not running";
	} else {
		label = ofToString(numUsers) + " users at " + ofToString(ofGetFrameRate(), 5) + " fps";
	}
	
	// debug
	for (int i = 0; i < numUsers; i++){
		
		// get a reference to this user
		ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
		
		// draw the mask texture for this user
		user.drawSkeleton();
	}
	
	ofSetColor(255);
	gui.draw();
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
	// show user event messages in the console
	ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
	openNIDevice.stop();
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