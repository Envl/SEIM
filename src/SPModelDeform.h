#pragma once
#include "ISpell.h"
#include "ISimuInputReceiver.h"
#include <ofEasyCam.h>
//#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include <ofxAssimpModelLoader/src/ofxAssimpModelLoader.h>

class SPModelDeform:public ISpell,public ISimuInputReceiver
{
	 int MAXMOVE = 20;
	 int DEFAULT_CAM_DIST = 200;
	 int MIN_CAM_DIST = 80;
private:
	ofCamera cam;
	ofxAssimpModelLoader model;
	vector<ofVboMesh> models;
	vector<ofVboMesh> originModels;
	vector<vector<ofVec3f>> modelVertDirs;
	vector<vector<float>> modelVertAngles;
	float camDist = 150;
	float phaseOffset = 0;
	float moveAmount = 0;
	int activeIndex = -1;
	void drawDebug( );
	float offsetSpeed = 5;
	const string NamePrefix = "Model";
	float *spectrum = nullptr;
	float bandNumber = 0;
	int keyBand;
	ofColor circleColor;
	const static int COLOR_NUM = 4;
	float accumMoveAmount = 0;
	const ofColor CircleColor[COLOR_NUM] =
	{
		ofColor(107 , 167 , 223),
		ofColor(226 , 110 , 247),
		ofColor(87 , 188 , 98),
		ofColor(233,189,55)
	};

	int ColorIndex = 0;

public:
	SPModelDeform( );
	~SPModelDeform( );
	void setCircleColor(ofColor c);
	void clearAccumMoveAmount( );
	void onMovement(std::vector<float>& movement) override;
	void onSoundSpectrum(float* spectrum) override; 
	ISpell* init(int x, int y, int w, int h, float deg=0) override;
	void update() override;
	void onTouch(InputParam p) override;
	void setup(string modelpath, int keyBand , int bands,int activeModel=0);
	void setActiveModel(int index);
	void addModel(string modelPath);
	void setParams(float maxMove , float offsetSpeed);
	void setModelIndex(int index);
	int getModelNum();
protected:
	void customDraw() override;
};
