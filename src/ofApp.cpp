#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofLogToFile("myLogFile.txt", true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/output_1080.mp4");
	video.play();
	video.setPaused(true);
	firstFrame = true;
	startTime = ofGetSystemTimeMillis();
}

void ofApp::update() {
	if (firstFrame) {
		video.firstFrame();
		firstFrame = false;
	}
	else {
		video.nextFrame();
	}
	//ofSleepMillis(500);
	video.update();
	ofResetElapsedTimeCounter();
	finder.update(video);
	ofLog(OF_LOG_NOTICE, "face detection fps is " + ofToString(1000/ofGetElapsedTimeMillis())+"\r\n");

	resizedImg.setFromPixels(video.getPixels());
	originalImg.setFromPixels(video.getPixels());
	resizedImg.resize(originalImg.getWidth()/10, originalImg.getHeight() / 10);
	resizedImg.resize(originalImg.getWidth(), originalImg.getHeight());

	ofResetElapsedTimeCounter();
	facesImg.clear();
	facesImg.allocate(originalImg.getWidth(), originalImg.getHeight(), OF_IMAGE_COLOR);
	/* Set background transparent in the face frames
	facesImg.allocate(originalImg.getWidth(), originalImg.getHeight(), OF_IMAGE_COLOR_ALPHA);
	originalImg.setImageType(OF_IMAGE_COLOR_ALPHA);
	facesImg.setColor(ofColor(255, 0, 0, 0));
	*/
	ofPixels crop;
	for (int i = 0; i < finder.size(); i++) {
		ofRectangle object = finder.getObject(i);
		originalImg.getPixels().cropTo(crop, object.x, object.y, object.width, object.height);
		crop.pasteInto(facesImg.getPixels(), object.x, object.y);
	}
	facesImg.update();
	ofLog(OF_LOG_NOTICE, "face frame generating fps is " + ofToString(1000 / ofGetElapsedTimeMillis()) + "\r\n");
	facesImg.save("imgs/facesImg"+ to_string(video.getCurrentFrame())+".jpg");

	if (video.getCurrentFrame() == video.getTotalNumFrames()-1) {
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
