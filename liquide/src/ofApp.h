#pragma once

#include "ofMain.h"
#include "ofxFluid.h"
#include "ofxCv.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

#define HOST_UNITY "192.168.1.3"
#define PORT_UNITY 14923

#define HOST_OF "192.168.1.3"
#define PORT_OF 14933

class testApp : public ofBaseApp{
public:
    void setup();
	void update();
	void updateClient();
	void updateServer();
    void draw();
	void exit();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	bool isClient();
	
	void userEvent(ofxOpenNIUserEvent & event);
	
	ofxOpenNI openNIDevice;
	
    ofxFluid fluid;
    
    vector<ofVec2f> oldLeftM, oldRightM;
    int     width, height;
	
	ofImage backImage;
	
	ofxSyphonServer syphonServer;
	
	ofxOscSender oscSenderUnity, oscSenderOf;
	ofPtr<ofxOscReceiver> oscReceiverOf;
	
	ofxToggle clearUsers;
	ofxToggle recording;
	ofxToggle logging;
	ofxLabel label;
	ofxFloatSlider disappearRate;
	ofxFloatSlider inertiaRate;
	ofxFloatSlider blobSize;
	ofxFloatSlider maskSize;
	
	ofxPanel gui;
	
	vector<int> dieCounts;
	
	cv::Mat proMatrix;
	bool isMapping;
	
	float proWidth, proHeight;
	
	vector<ofVec2f> headPoints;
	
	ofVboMesh headMask;
	bool bLogging;
};
