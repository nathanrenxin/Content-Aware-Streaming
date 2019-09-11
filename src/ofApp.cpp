#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofLogToFile("myLogFile.txt", true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/171124_C1_HD_002.mp4");
	video.play();
	video.setPaused(true);
	startTime = ofGetSystemTimeMillis();
}

void ofApp::update() {
	video.nextFrame();
	//ofSleepMillis(500);
	video.update();
	ofResetElapsedTimeCounter();
	finder.update(video);
	ofLog(OF_LOG_NOTICE, "face detection fps is " + ofToString(1000/ofGetElapsedTimeMillis())+"\r\n");

	if (video.getCurrentFrame() == video.getTotalNumFrames()) {
		stopTime = ofGetSystemTimeMillis();
		ofLog(OF_LOG_NOTICE, "video streaming time " + ofToString(stopTime-startTime) + "\r\n");
		OF_EXIT_APP(0);
	}
}

void ofApp::draw() {
	video.draw(0, 0);
	finder.draw();
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getTotalNumFrames()), 30, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getCurrentFrame()), 60, 20);
}
