#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/171124_C1_HD_002.mp4");
	video.play();
	//video.setup(640, 480);
}

void ofApp::update() {
    video.update();
	if (video.isFrameNew()) {
		finder.update(video);
	}
}

void ofApp::draw() {
	video.draw(0, 0);
	finder.draw();
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}
