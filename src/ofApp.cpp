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

	resizedImg.setFromPixels(video.getPixels());
	originalImg.setFromPixels(video.getPixels());
	facesImg.clear();
	facesImg.allocate(originalImg.getWidth(), originalImg.getHeight(), OF_IMAGE_COLOR);
	resizedImg.resize(originalImg.getWidth()/10, originalImg.getHeight() / 10);
	resizedImg.resize(originalImg.getWidth(), originalImg.getHeight());
	ofPixels crop;
	for (int i = 0; i < finder.size(); i++) {
		ofRectangle object = finder.getObject(i);
		originalImg.getPixels().cropTo(crop, object.x, object.y, object.width, object.height);
		crop.pasteInto(facesImg.getPixels(), object.x, object.y);
	}
	facesImg.update();

	if (video.getCurrentFrame() == video.getTotalNumFrames()) {
		stopTime = ofGetSystemTimeMillis();
		ofLog(OF_LOG_NOTICE, "video streaming time " + ofToString(stopTime-startTime) + "\r\n");
		OF_EXIT_APP(0);
	}
}

void ofApp::draw() {
	/* Show face detection
	video.draw(0, 0);
	finder.draw();
	*/

	/* Show clear faces over blurry background
	resizedImg.draw(0, 0);
	for (int i = 0; i < finder.size(); i++) {
		ofRectangle object = finder.getObject(i);
		originalImg.drawSubsection(object.x, object.y, object.width, object.height, object.x, object.y);
	}
	*/

	facesImg.draw(0, 0); //show only faces
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getTotalNumFrames()), 30, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getCurrentFrame()), 60, 20);
}
