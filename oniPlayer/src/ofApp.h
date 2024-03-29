#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxPCL.h"

class ofApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void playOpenNI();
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    ofPtr<ofxOpenNI> openNIPlayer;
    
    ofTrueTypeFont verdana;
    
    void userEvent(ofxOpenNIUserEvent & event);
    void gestureEvent(ofxOpenNIGestureEvent & event);
	
	ofDirectory dir;
	int fileIndex;
	
	ofEasyCam cam;
};

#endif
