#pragma once

#include "ofMain.h"
#include "ofxCv.h"


class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofVideoPlayer video;
	//ofVideoGrabber video;
	ofxCv::ObjectFinder finder;

	ofTexture texture;
	int scaleX, scaleY;
	int w, h;

	bool lowerResolution = true;
	int bytes_per_pixel;
};