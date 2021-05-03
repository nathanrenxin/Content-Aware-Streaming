#include "ofApp.h"
//#include "ofxTimeMeasurements.h"

using namespace std;

void ofApp::setup() {
	ofSetVerticalSync(true);
	currentTime = ofGetTimestampString();
	ofLogToFile("myLogFile_" + currentTime + ".txt", true);

	//TIME_SAMPLE_SET_AVERAGE_RATE(0.1);
	//TIME_SAMPLE_DISABLE_AVERAGE();
	//TIME_SAMPLE_SET_REMOVE_EXPIRED_THREADS(true);
	
	//ofBuffer buffer = ofBufferFromFile("myLogFile_2019-09-24-12-52-42-680.txt");
	ofResetElapsedTimeCounter();
	auto lines = ofSplitString(ofBufferFromFile("face_locations_3rd.txt").getText(), "\r\n");
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
	ofLog(OF_LOG_NOTICE, "file processing " + ofToString(ofGetElapsedTimeMillis()) + "\r\n");

	faceVideo.load("movies/face_3rd.mp4");
	faceVideo.play();
	faceVideo.setPaused(true);

	backgroundVideo.load("movies/background.mp4");
	backgroundVideo.play();
	backgroundVideo.setPaused(true);

	firstFrame = true;


	currentFrame = 0;
}

void ofApp::exit() {

}

void ofApp::update() {
	/* Using images
	if (firstFrame) {
		faceVideo.firstFrame();
		backgroundVideo.firstFrame();
		firstFrame = false;
	}
	else {
		currentFrame += 1;
		//ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(static_cast<int>((1000 / (1000 / ofGetFrameRate() - 500)))) + "\r\n");
		ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(ofGetFrameRate()) + "\r\n");
		if (currentFrame == backgroundVideo.getTotalNumFrames()) {
			OF_EXIT_APP(0);
		}
		faceVideo.nextFrame();
		backgroundVideo.nextFrame();
	}
	*/
	faceVideo.setFrame(currentFrame);
	backgroundVideo.setFrame(currentFrame);
	ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(static_cast<int>((1000 / (1000 / ofGetFrameRate() - 500)))) + "\r\n");
	currentFrame += 1;
	if (currentFrame == backgroundVideo.getTotalNumFrames()) {
		OF_EXIT_APP(0);
	}
	ofSleepMillis(500);
	faceVideo.update();
	backgroundVideo.update();
	//ofResetElapsedTimeCounter();
	map<int, vector < ofRectangle >>::iterator it = faceLocations.find(currentFrame);
	if (it != faceLocations.end()) {
		objects = it->second;
	}
	backgroundImg.setFromPixels(backgroundVideo.getPixels());
	facesImg.setFromPixels(faceVideo.getPixels());
	//backgroundImg.resize(facesImg.getWidth(), facesImg.getHeight());

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

	//ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(1000 / (ofGetElapsedTimeMillis())) + "\r\n");
}


void ofApp::draw() {

		//auto begin = chrono::high_resolution_clock::now();
		//TSGL_START("drawing");
		backgroundImg.draw(0, 0, 1920, 1080);
		for (int i = 0; i < objects.size(); i++) {
			ofRectangle object = objects[i];
			facesImg.drawSubsection(object.x, object.y, object.width, object.height, object.x, object.y);
		}

		//TSGL_STOP("drawing");
		//auto end = chrono::high_resolution_clock::now();
		//auto dur = end - begin;
		//auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		//cout << "fps is " << (ms) << endl;

		ofDrawBitmapStringHighlight(ofToString(objects.size()), 10, 20);
		ofDrawBitmapStringHighlight(ofToString(backgroundVideo.getTotalNumFrames()), 30, 20);
		ofDrawBitmapStringHighlight(ofToString(backgroundVideo.getCurrentFrame()), 60, 20);
		ofDrawBitmapStringHighlight(ofToString(faceVideo.getTotalNumFrames()), 30, 60);
		ofDrawBitmapStringHighlight(ofToString(faceVideo.getCurrentFrame()), 60, 60);
}
