#pragma once
#include "ofMain.h"
#include <ofxKinectForWindows2.h>
#include "SequenceImage.h"
#include <ofxWMFVideoPlayer.h>
#include "GLOBALS.h"
#include <ofxTrueTypeFontUC.h>
#include <ofxGui.h>
#include "HandTrackHelper.h"
#include "easyFunc.h"
#include <ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h>
#include "ofxFboScreen.h"
#include "VideoWrapper.h"
#include "ofxEasing\src\ofxEasing.h"

class CheckInApp:public ofBaseApp
{
public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;
	void windowResized(int w, int h) override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;

private:
	const  ofPoint COLOR_IMG_POS = ofPoint(GLOBALS::OtherW * 2 , 0);
	const ofPoint DEP_IMG_POS = ofPoint(GLOBALS::OtherW , GLOBALS::OtherH);
	const ofPoint IR_IMG_POS = ofPoint(GLOBALS::OtherW * 2 , GLOBALS::OtherH);
	const ofPoint DEPTH_THRESH_RANGE = ofPoint(0 , 0.001f);
	const ofPoint IR_THRESH_RANGE = ofPoint(0 , 0.05f);
	const ofPoint COLOR_THRESH_RANGE = ofPoint(0 , 255);
	const int static LOGO_VIDEO_NUM = 7;

	ofxKFW2::Device kinect_;
	shared_ptr<ofxKFW2::Source::Depth> depth_;
	shared_ptr<	ofxKFW2::Source::Infrared> ir_;
	VideoWrapper *logoVideos[LOGO_VIDEO_NUM];
	int logoIndex = 0;
	int lastLogo = 0;
	SequenceImage logoSequence;
	ofxTrueTypeFontUC yhFont;
	HandTrackHelper<ofShortPixels> handTrackHelper;
	ofxFboScreen screen;
	ofImage stampTip;

	ofPoint scanLinePos;
	int dragRad = 10;

	bool drawGui = true;
	bool bDrawDebug = false;
	bool drawWireFrame = true;
	bool isSpeedMode = false;
	bool bCheckInfo = true;
	bool bFirstFrame = true;
	bool seimGifPlayed = false;
	bool bNoHand = false;
	bool bgRecorded = false;

	ofxPanel gui;
		ofParameterGroup handFinderParams;
			ofParameter<ofVec4f> handROI = ofVec4f(0 , 0 , 1 , 1);// topLeft and bottomRight
			ofParameter<ofPoint> thresh = ofPoint(0.0f , 0.3f , 1);
			ofParameter<int> erodeIter = 1;
			ofParameter<int> dilateIter = 1;
			ofParameter<int> finderMinRad = 8;
			ofParameter<int> finderMaxRad = 100;
			ofParameter<int> finderThresh = 15;
			ofParameter<int> finderMaxMove = 32;
			ofParameter<bool> doBlur = false;
			ofParameter<int> blurSize = 3;
			ofParameter<int> stampSize = 3;
			ofParameter<ofColor> bgColor = ofColor(0);
			ofParameter<bool> imgXFlipped = false;
			ofParameter<bool> imgYFlipped = false;
			ofParameter<bool> screenXFlip = false , screenYFlip = false;
		ofParameterGroup projectionParams;
			ofParameter<ofVec2f> screenPos = ofVec2f(0);
			ofParameter<int> seimGifFPS = 24;
			ofParameter<bool> isNewHand = true;
			ofParameter<bool> isPlaySeimGifOnce = true;
			ofParameter<ofVec3f> seimOffSet = ofVec3f(0 , 0 , 0);
			ofParameter<ofVec2f> outputSize = ofVec2f(1024 , 768);
			ofParameter<ofVec3f> screenUL = ofVec2f(0 , 0);
			ofParameter<ofVec3f> screenUR = ofVec2f(1024 , 0);
			ofParameter<ofVec3f> screenDL = ofVec2f(0 , 768);
			ofParameter<ofVec3f> screenDR = ofVec2f(1024 , 768);
			ofParameter<ofPoint> A1 = ofPoint(0 , 0) , A2 = ofPoint(200 , 0) 
				, B1 = ofPoint(0 , 200) , B2 = ofPoint(200 , 200);
	//	
};
