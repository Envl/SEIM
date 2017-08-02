#pragma once
#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "easyFunc.h"
#include "SpellManager.h"
#include "InputManager.h"
#include "ManDetectHelper.h"
#include "ofxFboScreen.h"
#include <ofxWMFVideoPlayer.h>
#include "UpdateGuard.h"
#include "SequenceImage.h"
#include "VideoFrameHelper.h"
#include "ManDetectHelper.h"
#include "MovementDetectHelper.h"

#include "SPEnterWave.h"
#include "SPMania.h"
#include "SPModelDeform.h"
#include "SPSoundCloud.h"
#include "SPRain.h"
#include "SPBallMatrix.h"
#include "SPDotWave.h"
#include "SPImgAnim.h"

#include "VideoWrapper.h"
#include "Trigger.h"


enum Phase{
	Video,
	Hey,
	La,
	Waiting,
	Prelude,
	Solo,
	End
};
using namespace ofxCv;
class MainApp :public ofBaseApp
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

	void switchDrawings(Phase phase);

private:
	static const int SPECTRUM_BANDS = 128;
	static const int CHILL_SECONDS = 40;
	static const int END_SONG_DURATION = 40;
	static const int END_SONG_NUM = 10;
	const ofPoint DEPTH_THRESH_RANGE = ofPoint(0 , 0.001f);
	const ofPoint IR_THRESH_RANGE = ofPoint(0 , 0.1f);
	const ofPoint DEP_IMG_POS = ofPoint(GLOBALS::OtherW , GLOBALS::OtherH);
	const ofPoint IR_IMG_POS = ofPoint(GLOBALS::OtherW * 2 , GLOBALS::OtherH);
	
	
	std::vector<Trigger> triggerPool;
	map<string , Phase> phaseMap;
	ofxKFW2::Device kinect_;
	shared_ptr<ofxKFW2::Source::Depth> depth_;
	shared_ptr<	ofxKFW2::Source::Infrared> ir_;
	ManDetectHelper<ofShortPixels> manDetectHelper;
	MovementDetectHelper<ofShortPixels> moveDetectHelper;
	ofxFboScreen screenL,screenR,screenF,screenD;
	ofFbo logicScreen;// 包含每一个投影仪的画面。
	ofSoundPlayer mainAudio;
	float soundSpec[SPECTRUM_BANDS];
	VideoWrapper videoIntro;
	VideoWrapper endVideos[END_SONG_NUM];
	VideoWrapper countVideo;
	int endSongIndex = 0;

	ofPoint scanLinePos;

	vector<ofColor> frameColors;
	VideoFrameHelper liveHelper;
	//ofxWMFVideoPlayer mainVideo;
	VideoWrapper videoL , videoR,mainVideo;
	//SequenceImage logoSequence;
	UpdateGuard kinectUpdateGuard = UpdateGuard(1);

	SPDotWave* dotWave;
	//SPDotWave* dotwaveR;
	SPBallMatrix* ballMatL;
	SPBallMatrix* ballMatR;
	SPRain* rainLeft;
	SPRain* rainR;
	SPMania* maniaL;
	SPMania* maniaR;
	SPModelDeform* modelL;
	SPModelDeform* modelR;
	SPSoundCloud* soundcloud;

	float endTime = 0;

	bool bFirstOpen = true;
	bool bPlaying = false;
	bool bDrawVideo = true;
	bool bDrawDebug = false;
	bool isSpeedMode = false;
	bool bCheckInfo = true;
	bool drawGui = true;
	bool bFirstFrame = true;
	bool bDrawLogicScreen = false;
	bool bgRecorded = false;
	bool bIntro = false;

	Trigger firstFoundMan;
	Trigger introEnd;
	Trigger showEnd;
	Trigger chillReactivated;


	int dragRad = 10;
	int dragScreen = 1;

	ofxPanel gui; 
		ofParameter<int> kinectUpdateInterval = 1;
		ofParameter<ofVec3f> lightOffsetMax = ofVec3f(0 , 0);
	ofParameterGroup mainAppSpellParams;
		ofParameter<float> moveScaleParam = 1;
	ofParameterGroup mainappProjectionParams;
		ofParameter<ofVec2f> screenLPos = ofVec2f(0);
		ofParameter<ofVec2f> screenRPos = ofVec2f(0);
		ofParameter<ofVec2f> screenDPos = ofVec2f(0);
		ofParameter<ofVec2f> screenFPos = ofVec2f(0);
		ofParameter<ofVec2f> outputSizeL = ofVec2f(1024 , 768);
		ofParameter<ofVec2f> outputSizeR = ofVec2f(1024 , 768);
		ofParameter<ofVec2f> outputSizeD = ofVec2f(1024 , 768);
		ofParameter<ofVec2f> outputSizeF= ofVec2f(1920 , 1080);
		ofParameter<ofPoint> A1 = ofPoint(0 , 0) , A2 = ofPoint(200 , 0) , B1 = ofPoint(0 , 200) , B2 = ofPoint(200 , 200);
		ofParameter<ofPoint> screenFUL = ofPoint(0 , 0) , screenFUR = ofPoint(0 , 1024) , screenFDL = ofPoint(0 , 768) , screenFDR = ofPoint(1024 , 768);
		ofParameter<ofPoint> screenLUL = ofPoint(0 , 0) , screenLUR = ofPoint(0 , 1024) , screenLDL = ofPoint(0 , 768) , screenLDR = ofPoint(1024 , 768);
		ofParameter<ofPoint> screenRUL = ofPoint(0 , 0) , screenRUR = ofPoint(0 , 1024) , screenRDL = ofPoint(0 , 768) , screenRDR = ofPoint(1024 , 768);
		ofParameter<ofPoint> screenDUL = ofPoint(0 , 0) , screenDUR = ofPoint(0 , 1024) , screenDDL = ofPoint(0 , 768) , screenDDR = ofPoint(1024 , 768);
	ofParameterGroup mainappFinderParams;
		ofParameter<bool> doBlur = false;
		ofParameter<int> blurSize = 3;
		ofParameter<ofVec4f> ROI = ofVec4f(0 , 0 , 1 , 1);// topLeft and bottomRight
		ofParameter<ofPoint> thresh = ofPoint(0.0f , 0.3f , 1);
		ofParameter<int> erodeIter = 1;
		//ofParameter<bool> bgRecorded = false;
		ofParameter<int> dilateIter = 1;
		ofParameter<int> finderMinRad = 8;
		ofParameter<int> finderMaxRad = 100;
		ofParameter<int> finderThresh = 15;
		ofParameter<int> finderMaxMove = 32;
		ofParameter<ofColor> bgColor = ofColor(0);
		ofParameter<bool> imgXFlipped = false;
		ofParameter<bool> imgYFlipped = false;
};