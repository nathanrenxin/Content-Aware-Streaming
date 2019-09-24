#include "ofApp.h"

using namespace std;

void ofApp::setup() {
	ofSetVerticalSync(true);
	currentTime = ofGetTimestampString();
	ofLogToFile("myLogFile_" + currentTime + ".txt", true);
	
	//ofBuffer buffer = ofBufferFromFile("myLogFile_2019-09-24-12-52-42-680.txt");
	auto lines = ofSplitString(ofBufferFromFile("myLogFile_2019-09-24-12-52-42-680.txt").getText(), "\r\n");
	for (auto line : lines) {
		size_t pos = line.find_last_of(" ");
		string usefulString = line.substr(pos + 1);

		pos = usefulString.find(",");
		int frameNum = ofToInt(usefulString.substr(0, pos));

		ofRectangle object;
		string RecString = usefulString.substr(pos + 1);
		pos = RecString.find(",");
		object.x = ofToInt(RecString.substr(0, pos));
		RecString.erase(0, pos + 1);

		pos = RecString.find(",");
		object.y = ofToInt(RecString.substr(0, pos));
		RecString.erase(0, pos + 1);

		pos = RecString.find(",");
		object.width = ofToInt(RecString.substr(0, pos));
		object.height = ofToInt(RecString.substr(pos + 1));

		map<int, vector < ofRectangle >>::iterator it = faceLocations.find(frameNum);
		if (it != faceLocations.end()) {
			it->second.push_back(object);
		}
		else {
			vector < ofRectangle > objects;
			objects.push_back(object);
			faceLocations[frameNum] = objects;
		}
	}

	faceVideo.load("movies/face.mp4");
	faceVideo.play();
	faceVideo.setPaused(true);

	backgroundVideo.load("movies/background.mp4");
	backgroundVideo.play();
	backgroundVideo.setPaused(true);

	firstFrame = true;


	currentFrame = 1;
}

void ofApp::exit() {

}

void ofApp::update() {

	ofResetElapsedTimeCounter();
	if (firstFrame) {
		faceVideo.firstFrame();
		backgroundVideo.firstFrame();
		firstFrame = false;
	}
	else {
		faceVideo.nextFrame();
		backgroundVideo.nextFrame();
	}
	ofSleepMillis(500);
	faceVideo.update();
	backgroundVideo.update();

	map<int, vector < ofRectangle >>::iterator it = faceLocations.find(backgroundVideo.getCurrentFrame());
	if (it != faceLocations.end()) {
		objects = it->second;
	}

	backgroundImg.setFromPixels(backgroundVideo.getPixels());
	facesImg.setFromPixels(faceVideo.getPixels());
	backgroundImg.resize(facesImg.getWidth(), facesImg.getHeight());

	/* Using images
	backgroundImg.load("movies/background/background" + ofToString(currentFrame) + ".jpg");
	facesImg.load("movies/face/face" + ofToString(currentFrame) + ".jpg");
	ofResetElapsedTimeCounter();
	backgroundImg.resize(facesImg.getWidth(), facesImg.getHeight());
	if (currentFrame == 248) {
		OF_EXIT_APP(0);
	}
	currentFrame += 1;
	ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(1000 / ofGetElapsedTimeMillis()) + "\r\n");
	*/

	ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(1000 / (ofGetElapsedTimeMillis() - 500)) + "\r\n");
	if (backgroundVideo.getCurrentFrame() == backgroundVideo.getTotalNumFrames()-1) {
		OF_EXIT_APP(0);
	}
}


void ofApp::draw() {
	backgroundImg.draw(0, 0);
	for (int i = 0; i < objects.size(); i++) {
		ofRectangle object = objects[i];
		facesImg.drawSubsection(object.x, object.y, object.width, object.height, object.x, object.y);
	}
	ofDrawBitmapStringHighlight(ofToString(objects.size()), 10, 20);
	ofDrawBitmapStringHighlight(ofToString(backgroundVideo.getTotalNumFrames()), 30, 20);
	ofDrawBitmapStringHighlight(ofToString(backgroundVideo.getCurrentFrame()), 60, 20);
}