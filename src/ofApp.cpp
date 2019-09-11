#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/istanbul.mp4");
	video.play();
	scaleX = 5; 
	scaleY = 1; 
	w = video.getWidth() / scaleX;  
	h = video.getHeight() / scaleY;  
	texture.allocate(w, h, GL_RGB);

	bytes_per_pixel = 3;
	//video.setup(640, 480);
}

void ofApp::update() {
    /*video.update();
	if (video.isFrameNew()) {
		finder.update(video);
	}*/
	video.update();

	/*if (lowerResolution) {
		bytes_per_pixel -= 1;
		if (bytes_per_pixel <= 1) {
			lowerResolution = false;
		}
	}
	else {
		bytes_per_pixel += 1;
		if (bytes_per_pixel >= 10) {
			lowerResolution = true;
		}
	}*/
	unsigned char* pixels = video.getPixels().getData();

	unsigned char* nPixels = new unsigned char[w * h * bytes_per_pixel];;
	for (int i = 0; i < w * h; i++) {
		int base = i * bytes_per_pixel;
		int nBase = base * (scaleX * scaleY);


		unsigned char r = pixels[nBase + 0];
		unsigned char g = pixels[nBase + 1];
		unsigned char b = pixels[nBase + 2];


		nPixels[base + 0] = r;
		nPixels[base + 1] = g;
		nPixels[base + 2] = b;
	}


	texture.loadData(nPixels, w, h, GL_RGB);
}

void ofApp::draw() {
	//video.draw(0, 0);
	texture.draw(0, 0, w * scaleX, h * scaleY);
	//finder.draw();
	//ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}
