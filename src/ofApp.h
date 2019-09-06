#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class FaceDetector {
public:
    ofxCv::ObjectFinder faceFinder;

    void setup() {
        faceFinder.setup("haarcascade_frontalface_default.xml");
        faceFinder.setPreset(ofxCv::ObjectFinder::Accurate);
        faceFinder.setFindBiggestObject(true);
    }
    template <class T>
    void update(T& img) {
        update(ofxCv::toCv(img));
    }
    void update(const cv::Mat& mat) {
        faceFinder.update(mat);
    }
    void draw() const {
        faceFinder.draw();
    }
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    //ofVideoPlayer video;
	ofVideoGrabber video;
    FaceDetector face;
};
