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
	int startTime;
	int stopTime;
	ofImage img1;
	ofImage img2;
};