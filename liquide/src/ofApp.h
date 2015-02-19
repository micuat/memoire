#pragma once

#include "ofMain.h"
#include "ofxFluid.h"
#include "ofxCv.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
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
	
	void userEvent(ofxOpenNIUserEvent & event);
	
	ofxOpenNI openNIDevice;
	
    ofxFluid fluid;
    
    ofVec2f oldM;
    int     width,height;
    bool    bPaint, bObstacle, bBounding, bClear;
	
	ofImage backImage;
	
	ofxSyphonServer syphonServer;
};
