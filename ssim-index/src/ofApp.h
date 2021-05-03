#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
	void exit();


    ofVideoPlayer comparisonVideo;
	ofVideoPlayer originalVideo;

	ofImage originalImg;
	ofImage comparisonImg;

	ofImage originalCrop;
	ofImage comparisonCrop;

	bool firstFrame;
	string currentTime;
	vector < ofRectangle > objects;
	map<int, vector < ofRectangle >> faceLocations;
	int currentFrame;
	map<int, vector < ofImage >> images;

	int totalssim;

private:
	void parseLogFile(std::vector<std::string>& lines);
	int runSSIM(int frame, ofImage src, ofImage faces);
	cv::Scalar getMSSIM(const cv::Mat& i1, const cv::Mat& i2);
};