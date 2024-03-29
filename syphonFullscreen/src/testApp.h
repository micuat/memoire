#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSyphon.h"

class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
	void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
	void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
	
	ofxSyphonClient mClient;
	ofxSyphonServerDirectory dir;
	bool serverAvailable;
};

#endif

