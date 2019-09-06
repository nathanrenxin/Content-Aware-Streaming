#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofBackground(255,255,255);
    ofSetVerticalSync(true);
    //video.load("movies/fingers.mov");
    //video.setLoopState(OF_LOOP_NORMAL);
    //video.play();
	video.setup(1024, 768);
    face.setup();
}

void ofApp::update() {
    video.update();
    if(video.isFrameNew()) {
        face.update(video);
    }
}

void ofApp::draw() {
    ofSetColor(255);
    video.draw(0, 0);
    face.draw();
}
