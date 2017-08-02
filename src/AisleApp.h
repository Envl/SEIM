#pragma once
#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "easyFunc.h"
#include "SpellManager.h"
#include "InputManager.h"
#include "SPEnterWave.h"
#include "ManDetectHelper.h"
#include "ofxFboScreen.h"
#include "Magics.h"

using namespace ofxCv;
class AisleApp:public ofBaseApp
{
public:
	void setup( ) override;
	void update( ) override;
	void draw( ) override;
	void exit( ) override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x , int y) override;
	void mouseDragged(int x , int y , int button) override;
	void mousePressed(int x , int y , int button) override;
	void mouseReleased(int x , int y , int button) override;
	void mouseScrolled(int x , int y , float scrollX , float scrollY) override;
private:
	ofxKFW2::Device kinect_;
	shared_ptr<ofxKFW2::Source::Depth> depth_;
	shared_ptr<	ofxKFW2::Source::Infrared> ir_;
	ManDetectHelper<ofShortPixels> manDetectHelper;
	ofxFboScreen screen;

	bool bDrawDebug = false;
	bool isSpeedMode = false;
	bool bCheckInfo = true;
	bool drawGui = true;

	const ofPoint DEPTH_THRESH_RANGE = ofPoint(0 , 0.001f);
	const ofPoint IR_THRESH_RANGE = ofPoint(0 , 0.1f);
	const ofPoint DEP_IMG_POS = ofPoint(GLOBALS::OtherW , GLOBALS::OtherH);
	const ofPoint IR_IMG_POS = ofPoint(GLOBALS::OtherW * 2 , GLOBALS::OtherH);
	const int PROJECTOR_W = 1280;
	const int PROJECTOR_H = 1024;
	int dragRad = 10;

	Magics magics_;

	//GUI
	ofxPanel gui; 
		ofParameter<int> lineW = 1;
	ofParameterGroup projectionParams;
		ofParameter<ofVec2f> screenPos = ofVec2f( 0);
		ofParameter<ofVec2f> outputSize = ofVec2f(1024 , 768);
		ofParameter<ofVec2f> posOffset = ofVec2f(0 , 0);
		ofParameter<ofVec3f> screenUL = ofVec2f(0 , 0);
		ofParameter<ofVec3f> screenUR = ofVec2f(1024 , 0);
		ofParameter<ofVec3f> screenDL = ofVec2f(0 , 768);
		ofParameter<ofVec3f> screenDR = ofVec2f(1024 , 768);
		ofParameter<ofPoint> A1 = ofPoint(0 , 0) , A2 = ofPoint(200 , 0) , B1 = ofPoint(0 , 200) , B2 = ofPoint(200 , 200);
	ofParameterGroup manFinderParams;
		ofParameter<ofVec4f> ROI = ofVec4f(0 , 0 , 1 , 1);// topLeft and bottomRight
		ofParameter<ofPoint> thresh = ofPoint(0.0f , 0.3f , 1);
		ofParameter<int> erodeIter = 1;
		ofParameter<bool> bgRecorded = false;
		ofParameter<int> dilateIter = 1;
		ofParameter<int> finderMinRad = 8;
		ofParameter<int> finderMaxRad = 100;
		ofParameter<int> finderThresh = 15;
		ofParameter<int> finderMaxMove = 32;
		ofParameter<bool> doBlur = false;
		ofParameter<int> blurSize = 3;
		ofParameter<ofColor> bgColor = ofColor(0);
		ofParameter<bool> imgXFlipped = false;
		ofParameter<bool> imgYFlipped = false;
};
