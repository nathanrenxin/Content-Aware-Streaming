#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/171124_C1_HD_002.mp4");
	video.play();
	video.setPaused(true);
    
	/*
	for (int i = 0; i < video.getTotalNumFrames(); i++) {
		video.nextFrame();
		video.update();
		finder.update(video);
	}
	/**/
}

void ofApp::update() {
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		video.nextFrame();
		video.update();
		finder.update(video);
		break;
	}
}

void ofApp::draw() {
	video.draw(0, 0);
	finder.draw();
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getTotalNumFrames()), 30, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getCurrentFrame()), 60, 20);
}
