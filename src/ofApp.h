#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxVideoRecorder.h"



class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
	void exit();
	void audioIn(float* input, int bufferSize, int nChannels);


    ofVideoPlayer video;
	//
	ofxVideoRecorder    vidRecorder;
	ofSoundStream       soundStream;
	bool bRecording;
	int sampleRate;
	int channels;
	string fileName;
	string fileExt;

	ofFbo recordFbo;
	ofPixels recordPixels;
	//

	ofxCv::ObjectFinder finder;
	int startTime;
	int stopTime;
	ofImage resizedImg;
	ofImage originalImg;
	ofImage facesImg;
	bool firstFrame;
};