#pragma once

#include "ofMain.h"



class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
	void exit();


    ofVideoPlayer faceVideo;
	ofVideoPlayer backgroundVideo;

	ofImage backgroundImg;
	ofImage facesImg;
	bool firstFrame;
	string currentTime;
	vector < ofRectangle > objects;
	map<int, vector < ofRectangle >> faceLocations;
};