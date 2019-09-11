#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	finder.setup("haarcascade_frontalface_default.xml");
	finder.setPreset(ObjectFinder::Fast);
	video.load("movies/istanbul.mp4");
	//video.setup(640, 480);

	ofApp::cascade.load("haarcascade_frontalface_default.xml");
}

void ofApp::update() {
    video.update();
	if (video.isFrameNew()) {
		//finder.update(video);

		ofPixels& pixels = video.getPixels();
		Mat frame = ofxCv::toCv(pixels);
		if (frame.empty()) {
			return;
		}

		vector<Rect> faces, faces2;
		Mat gray, small_gray;
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		resize(gray, small_gray, Size(), 1, 1, INTER_LINEAR);
		equalizeHist(small_gray, small_gray);

		ofImage res;
		toOf(frame, res);
		res.update();
		// Detect faces of different sizes using cascade classifier  
		//cascade.detectMultiScale(small_gray, faces, 1.1,
		//	2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		//imshow("test", faces);
		//for (size_t i = 0; i < faces.size(); i++)
		//{
		//	Rect r = faces[i];
		//	Mat smallImgROI;
		//	vector<Rect> nestedObjects;
		//	Point center;
		//	Scalar color = Scalar(255, 0, 0); // Color for Drawing tool 
		//	int radius;

		//	double aspect_ratio = (double)r.width / r.height;
		//	if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		//	{
		//		center.x = cvRound((r.x + r.width * 0.5) * 1);
		//		center.y = cvRound((r.y + r.height * 0.5) * 1);
		//		radius = cvRound((r.width + r.height) * 0.25 * 1);
		//		circle(frame, center, radius, color, 3, 8, 0);
		//	}
		//	else
		//		rectangle(frame, cvPoint(cvRound(r.x * 1), cvRound(r.y * 1)),
		//			cvPoint(cvRound((r.x + r.width - 1) * 1),
		//				cvRound((r.y + r.height - 1) * 1)), color, 3, 8, 0);
		//}

		// Show Processed Image with detected faces 
		//imshow("Face Detection", frame);
	}
}

void ofApp::draw() {
	video.draw(10, 10);
	if (video.isLoaded()) {
		video.play();
	}
	//finder.draw();


}
