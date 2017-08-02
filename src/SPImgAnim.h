#pragma once
#include "ISpell.h"
#include "ofMain.h"
#include "ISimuInputReceiver.h"
#include "GLOBALS.h"

enum AnimType
{
	SingleFrameAuto ,
	SingleFrameManual,
	MultiFrame
};

class SPImgAnim:public ISpell,public ISimuInputReceiver
{
public:

	~SPImgAnim( );
	SPImgAnim( );
	void customDraw( ) override;
	void update( ) override;
	void onTouch(InputParam p) override;
	ISpell* init(int x , int y , int w , int h,float rotationDeg) override;
	void setupTexture(vector<ofTexture> tex);
	void setParameters(ofVec2f offsetDelta , int fps,bool singleAuto=true, ofColor color=ofColor(177));
	void setColor(ofColor newColor);
	int getTexNum( );

	const string NamePrefix = "ImgAnim";

private:
	int FPS = 24;
	bool bSingleAuto = true;
	AnimType animType = SingleFrameAuto;
	ofColor color;
	ofVec2f offset,delta;
	float currentTex = 0;
	vector<ofTexture> textures_;
};

