#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxCv.h"
//#include "ofxTrueTypeFontUC.h"
#include "ofxGui.h"

#include "objFinder.h"
#include "easyFunc.h"
#include "Magics.h"
#include "ofxHelper.h"
#include "SPEnterWave.h"
#include "InputManager.h"
#include "GLOBALS.h"
#include "SpellManager.h"
#include "TempObjFinder.h"
#include "SmoothValueFilter.h"
#include "SmoothVec3Filter.h"
#include "PerspectiveAgent.h"
#include <ofxWMFVideoPlayer.h>
#include "UpdateGuard.h"
#include "ThreadedFinder.h"
#include "HandTrackHelper.h"
#include "SequenceImage.h"
#include "VideoFrameHelper.h"
#include "ManDetectHelper.h"
#include "SPSoundCloud.h"
#include "MovementDetectHelper.h"
#include <ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h>

using namespace EasyFunction;
using namespace ofxCv;
class ofApp : public ofBaseApp{
	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x , int y , float scrollX , float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
		void audioRequested(float * , int , int) override;
		void audioReceived(float * , int , int) override;

		
		
	private:
		ofxKFW2::Device kinect_;
		shared_ptr<ofxKFW2::Source::Depth> depth_;
		shared_ptr<ofxKFW2::Source::Color> color_;
		shared_ptr<	ofxKFW2::Source::Infrared> ir_;
		shared_ptr< ofxKFW2::Source::BodyIndex> bIndex_;

		bool drawVideo = false;
		ofSoundPlayer mainAudio;
		static const int SPECTRUM_BANDS = 128;
		float soundSpec[SPECTRUM_BANDS];

		vector<ofColor> frameColors;
		VideoFrameHelper liveHelper;
		ofxWMFVideoPlayer mainVideo;
		ofxWMFVideoPlayer logoVideo;
		SequenceImage logoSequence;
		UpdateGuard kinectUpdateGuard=UpdateGuard(1);

//		vector<ofVec3f> points;
//		vector<ofFloatColor> colors;
//		vector<double> weights;
		bool bDrawDebug=false;
		bool drawWireFrame = true;
		bool isSpeedMode = false;
		bool bCheckInfo = true;
		bool bFirstFrame = true;
		
		const  ofPoint colorImgPos = ofPoint(GLOBALS::OtherW * 2 , 0);
		const ofPoint depthImgPos = ofPoint(GLOBALS::OtherW  , GLOBALS::OtherH);
		const ofPoint irImgPos = ofPoint(GLOBALS::OtherW * 2 , GLOBALS::OtherH);

		ofTrueTypeFont jhengheiFont;
		ofxTrueTypeFontUC yhFont;

		ofImage img;

		ofParameter<int> seimGifFPS = 24;
		ofParameter<bool> isNewHand = true;
		ofParameter<bool> isPlaySeimGifOnce = true;
		bool seimGifPlayed = false;
		ofParameter<ofVec3f> seimOffSet = ofVec3f(0 , 0 , 0);

		ofxButton btnLoad;
		ofxButton btnSave;

		SmoothValueFilter<int> handRad = SmoothValueFilter<int>(24);
		SmoothVec3Filter handPosFilter=SmoothVec3Filter(24);
		ofParameter<bool> findColorROI = false;// color 为16:9  其他为512:424
		ofParameter<int> findHandInterval = 1;
		bool drawGui = true;
		const ofPoint DEPTH_THRESH_RANGE = ofPoint(0 , 0.001f);
		const ofPoint IR_THRESH_RANGE = ofPoint(0 , 0.05f);
		const ofPoint COLOR_THRESH_RANGE=ofPoint(0,255);
		ofxPanel gui;
		ofParameterGroup handFinderParams;
		ofParameter<ofVec4f> handROI = ofVec4f(0 , 0 , 1 , 1);// topLeft and bottomRight
		ofParameter<ofPoint> thresh =ofPoint(0.0f,0.3f,1);
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
//		ContourFinder contourFinder;
//		ofFloatImage depDiff_float;
//		ofShortImage depDiff,depBG_img;
		ofImage depDiff_img,colorBG_img,colorDiff_img;	
		HandTrackHelper<ofShortPixels> handTrackHelper;
		ManDetectHelper<ofShortPixels> manDetectHelper;
		MovementDetectHelper<ofShortPixels> moveDetectHelper;
//		ThreadedFinder<ofShortPixels> threadedFnd_depth;
		ObjFinder<ofShortPixels> finder_ir;
		ObjFinder<ofPixels > finder_color;
//		PerspectiveAgent handPerspAgent = PerspectiveAgent(ofRectangle(0 , 0 , 1024 , 768));
		ofShortPixels depthROI_pix;
//				ofShortImage justForTest,
		ofShortImage shortTestImg;

		ofParameterGroup projectionParams;
		ofParameter<ofVec2f> outputSize = ofVec2f(1024 , 768);
		ofParameter<ofPoint> A1 = ofPoint(0 , 0) , A2 = ofPoint(200 , 0) , B1 = ofPoint(0 , 200) , B2 = ofPoint(200 , 200);
//		cv::Mat handProjTransMat = cv::Mat::eye(3 , 3 , CV_64FC1);

		static const int logicScreenNum = 3;
		int dragRad = 10;
		int targetVertexIndex=0;
		bool canDrag = false;
		int mouseDownX=-100 , mouseDownY=-100;
		cv::Point2f posSeim;
		ofVboMesh logicScreen[logicScreenNum];
		ofFbo screenBuffer[logicScreenNum];
		ofParameter<int> logicScreenIndex = 0;

		//我加的函数-----------
//		void setupContourFinder( );
		void saveParams(string fileCode );
		void loadParams(string fileCode );
		void save( );
		void load( );
		//------------以上
};
