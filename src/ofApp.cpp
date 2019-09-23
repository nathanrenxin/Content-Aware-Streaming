#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofLogToFile("myLogFile_" + ofGetTimestampString() + ".txt", true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/output_1080.mp4");
	video.play();
	video.setPaused(true);
	startTime = ofGetSystemTimeMillis();

	//ofSetFrameRate(60);
	//ofSetLogLevel(OF_LOG_VERBOSE);

#ifdef TARGET_WIN32
	//vidRecorder.setFfmpegLocation("C:\ffmpeg\bin"); // use this is you have ffmpeg installed in your data folder
#endif
	fileName = "faceOutput";
	fileExt = ".mp4"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

	// override the default codecs if you like
	// run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
	
	vidRecorder.setVideoCodec("h264");
	//vidRecorder.setVideoBitrate("12262k");
	//vidRecorder.setVideoBitrate(video..getStats().estimatedBandwidth);
	
	//vidRecorder.setAudioCodec("mp3");
	//vidRecorder.setAudioBitrate("192k");
	//soundStream.listDevices();
	//soundStream.setDeviceID(11);
	//soundStream.setup(this, 0, channels, sampleRate, 256, 4);

	ofSetWindowShape(video.getWidth(), video.getHeight());
	bRecording = false;
	ofEnableAlphaBlending();

	//sampleRate = 44100;
	//channels = 2;

	//vidRecorder.setup(fileName + ofGetTimestampString() + fileExt, video.getWidth(), video.getHeight(), 30, 0, channels);
	//vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30); // no audio
	//vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
	//vidRecorder.setupCustomOutput(video.getWidth(), video.getHeight(), 30, 25, channels, "-vcodec mpeg4 udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
	vidRecorder.setup(fileName + ofGetTimestampString() + fileExt, video.getWidth(), video.getHeight(), 25, sampleRate, channels);

	vidRecorder.start();
}
void ofApp::exit() {
	vidRecorder.close();
}

void ofApp::update() {
	video.nextFrame();
	video.update();
	ofResetElapsedTimeCounter();
	finder.update(video);
	ofLog(OF_LOG_NOTICE, ofToString(1000/ofGetElapsedTimeMillis()));

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
	ofSleepMillis(500);
	facesImg.update();
	bool success = vidRecorder.addFrame( facesImg );
	if (!success) {
		ofLog(OF_LOG_NOTICE, "This frame was not added!");
	}

	if (video.getCurrentFrame() == video.getTotalNumFrames()-1) {
		stopTime = ofGetSystemTimeMillis();
		ofLog(OF_LOG_NOTICE, "video streaming time " + ofToString(stopTime-startTime) + "\r\n");
		OF_EXIT_APP(0);
	}
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels) {
	if (bRecording)
		vidRecorder.addAudioSamples(input, bufferSize, nChannels);
}

void ofApp::draw() {
	/* Show face detection
	video.draw(0, 0);
	finder.draw();
	*/

	/* Show clear faces over blurry background*/
	/*resizedImg.draw(0, 0);
	for (int i = 0; i < finder.size(); i++) {
		ofRectangle object = finder.getObject(i);
		originalImg.drawSubsection(object.x, object.y, object.width, object.height, object.x, object.y);
	}*/
	

	facesImg.draw(0, 0); //show only faces
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getTotalNumFrames()), 30, 20);
	ofDrawBitmapStringHighlight(ofToString(video.getCurrentFrame()), 60, 20);

	/*stringstream ss;
	ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
		<< "audio queue size: " << vidRecorder.getAudioQueueSize() << endl
		<< "FPS: " << ofGetFrameRate() << endl
		<< (bRecording ? "pause" : "start") << " recording: r" << endl
		<< (bRecording ? "close current video file: c" : "") << endl;*/
}