#include "ofApp.h"
using namespace std;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofLogToFile("ssim_log" + ofGetTimestampString() + ".txt", true);
	
	auto lines = ofSplitString(ofBufferFromFile("myLogFile_2019-09-24-12-52-42-680.txt").getText(), "\r\n");
	parseLogFile(lines);

	faceVideo.load("movies/face.mp4");
	faceVideo.play();
	faceVideo.setPaused(true);

	comparisonVideo.load("movies/output_480.mp4");
	comparisonVideo.play();
	comparisonVideo.setPaused(true);

	firstFrame = true;
	currentFrame = 1;
}

void ofApp::exit() {

}

void ofApp::update() {
	
	ofResetElapsedTimeCounter();
	if (firstFrame) {
		faceVideo.firstFrame();
		comparisonVideo.firstFrame();
		firstFrame = false;
	}
	else {
		faceVideo.nextFrame();
		comparisonVideo.nextFrame();
	}
	//ofSleepMillis(500);
	faceVideo.update();
	comparisonVideo.update();

	map<int, vector < ofRectangle >>::iterator it = faceLocations.find(comparisonVideo.getCurrentFrame());
	if (it != faceLocations.end()) {
		objects = it->second;
	}

	backgroundImg = comparisonVideo.getPixels();
	facesImg = faceVideo.getPixels();
	backgroundImg.resize(facesImg.getWidth(), facesImg.getHeight());
	runSSIM(currentFrame, backgroundImg, facesImg);
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
	currentFrame += 1;
	ofLog(OF_LOG_NOTICE, "client fps is " + ofToString(1000 / (ofGetElapsedTimeMillis() - 500)) + "\r\n");
	if (comparisonVideo.getCurrentFrame() == comparisonVideo.getTotalNumFrames()-1) {
		OF_EXIT_APP(0);
	}
}


void ofApp::draw() {
	/*backgroundImg.draw(0, 0);
	for (int i = 0; i < objects.size(); i++) {
		ofRectangle object = objects[i];
		facesImg.drawSubsection(object.x, object.y, object.width, object.height, object.x, object.y);
	}*/
	ofDrawBitmapStringHighlight(ofToString(objects.size()), 10, 20);
	ofDrawBitmapStringHighlight(ofToString(comparisonVideo.getTotalNumFrames()), 30, 20);
	ofDrawBitmapStringHighlight(ofToString(comparisonVideo.getCurrentFrame()), 60, 20);
}

void ofApp::parseLogFile(std::vector<std::string>& lines)
{
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
}

int ofApp::runSSIM(int frame, ofImage &src, ofImage &face) {
	// default settings
	double C1 = 6.5025, C2 = 58.5225;

	IplImage
		* img1 = NULL, * img2 = NULL, * img1_img2 = NULL,
		* img1_temp = NULL, * img2_temp = NULL,
		* img1_sq = NULL, * img2_sq = NULL,
		* mu1 = NULL, * mu2 = NULL,
		* mu1_sq = NULL, * mu2_sq = NULL, * mu1_mu2 = NULL,
		* sigma1_sq = NULL, * sigma2_sq = NULL, * sigma12 = NULL,
		* ssim_map = NULL, * temp1 = NULL, * temp2 = NULL, * temp3 = NULL;

	img1_temp = new IplImage(ofxCv::toCv(src));
	img2_temp = new IplImage(ofxCv::toCv(face));

	if (img1_temp == NULL || img2_temp == NULL)
		return -1;

	int x = img1_temp->width, y = img1_temp->height;
	int nChan = img1_temp->nChannels, d = IPL_DEPTH_32F;
	CvSize size = cvSize(x, y);

	img1 = cvCreateImage(size, d, nChan);
	img2 = cvCreateImage(size, d, nChan);

	cvConvert(img1_temp, img1);
	cvConvert(img2_temp, img2);
	//cvReleaseImage(&img1_temp);
	//cvReleaseImage(&img2_temp);


	img1_sq = cvCreateImage(size, d, nChan);
	img2_sq = cvCreateImage(size, d, nChan);
	img1_img2 = cvCreateImage(size, d, nChan);

	cvPow(img1, img1_sq, 2);
	cvPow(img2, img2_sq, 2);
	cvMul(img1, img2, img1_img2, 1);

	mu1 = cvCreateImage(size, d, nChan);
	mu2 = cvCreateImage(size, d, nChan);

	mu1_sq = cvCreateImage(size, d, nChan);
	mu2_sq = cvCreateImage(size, d, nChan);
	mu1_mu2 = cvCreateImage(size, d, nChan);


	sigma1_sq = cvCreateImage(size, d, nChan);
	sigma2_sq = cvCreateImage(size, d, nChan);
	sigma12 = cvCreateImage(size, d, nChan);

	temp1 = cvCreateImage(size, d, nChan);
	temp2 = cvCreateImage(size, d, nChan);
	temp3 = cvCreateImage(size, d, nChan);

	ssim_map = cvCreateImage(size, d, nChan);
	/*************************** END INITS **********************************/


	//////////////////////////////////////////////////////////////////////////
	// PRELIMINARY COMPUTING
	cvSmooth(img1, mu1, CV_GAUSSIAN, 11, 11, 1.5);
	cvSmooth(img2, mu2, CV_GAUSSIAN, 11, 11, 1.5);

	cvPow(mu1, mu1_sq, 2);
	cvPow(mu2, mu2_sq, 2);
	cvMul(mu1, mu2, mu1_mu2, 1);


	cvSmooth(img1_sq, sigma1_sq, CV_GAUSSIAN, 11, 11, 1.5);
	cvAddWeighted(sigma1_sq, 1, mu1_sq, -1, 0, sigma1_sq);

	cvSmooth(img2_sq, sigma2_sq, CV_GAUSSIAN, 11, 11, 1.5);
	cvAddWeighted(sigma2_sq, 1, mu2_sq, -1, 0, sigma2_sq);

	cvSmooth(img1_img2, sigma12, CV_GAUSSIAN, 11, 11, 1.5);
	cvAddWeighted(sigma12, 1, mu1_mu2, -1, 0, sigma12);


	//////////////////////////////////////////////////////////////////////////
	// FORMULA

	// (2*mu1_mu2 + C1)
	cvScale(mu1_mu2, temp1, 2);
	cvAddS(temp1, cvScalarAll(C1), temp1);

	// (2*sigma12 + C2)
	cvScale(sigma12, temp2, 2);
	cvAddS(temp2, cvScalarAll(C2), temp2);

	// ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
	cvMul(temp1, temp2, temp3, 1);

	// (mu1_sq + mu2_sq + C1)
	cvAdd(mu1_sq, mu2_sq, temp1);
	cvAddS(temp1, cvScalarAll(C1), temp1);

	// (sigma1_sq + sigma2_sq + C2)
	cvAdd(sigma1_sq, sigma2_sq, temp2);
	cvAddS(temp2, cvScalarAll(C2), temp2);

	// ((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
	cvMul(temp1, temp2, temp1, 1);

	// ((2*mu1_mu2 + C1).*(2*sigma12 + C2))./((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2))
	cvDiv(temp3, temp1, ssim_map, 1);


	CvScalar index_scalar = cvAvg(ssim_map);

	// through observation, there is approximately 
	// 1% error max with the original matlab program

	ofLog(OF_LOG_NOTICE, "frame: " + ofToString(frame) + "\r\n");
	ofLog(OF_LOG_NOTICE, "(R, G & B SSIM index) \r\n");
	ofLog(OF_LOG_NOTICE, ofToString(index_scalar.val[2] * 100) + "% \r\n");
	ofLog(OF_LOG_NOTICE, ofToString(index_scalar.val[1] * 100) + "% \r\n");
	ofLog(OF_LOG_NOTICE, ofToString(index_scalar.val[0] * 100) + "% \r\n");

	// if you use this code within a program
	// don't forget to release the IplImages
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img1_img2);
	//cvReleaseImage(&img1_temp);
	//cvReleaseImage(&img2_temp);
	cvReleaseImage(&img1_sq);
	cvReleaseImage(&img2_sq);
	cvReleaseImage(&mu1);
	cvReleaseImage(&mu2);
	cvReleaseImage(&mu1_sq);
	cvReleaseImage(&mu2_sq);
	cvReleaseImage(&mu1_mu2);
	cvReleaseImage(&sigma1_sq);
	cvReleaseImage(&sigma2_sq);
	cvReleaseImage(&sigma12);
	cvReleaseImage(&ssim_map);
	cvReleaseImage(&temp1);
	cvReleaseImage(&temp2);
	cvReleaseImage(&temp3);
	return 1;
}