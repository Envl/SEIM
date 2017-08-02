#pragma once
#include "ISimuInputReceiver.h"
#include "ISpell.h"
#include <ofVboMesh.h>

enum ManiaType
{
	ManiaBar ,
	ManiaDot
};
class SPMania :public ISimuInputReceiver , public ISpell
{
private:
	float *spectrum = nullptr;
	float bandNumber = 0;
	const string NamePrefix = "Mania";
	int hSpacing = 30;
	int barWidth = 40;
	int barNum;
	float* tintBarHeight;
	int soundBands = 0;
	int keyBand = 0;
	ofPoint offset;
	ManiaType type;
	bool flipX = false;
	ofColor colorDR = ofColor(254, 76, 88);
	ofColor colorDL = ofColor(89, 200, 252);
	ofColor colorUR = ofColor(155, 45, 197);
	ofColor colorUL = ofColor(88, 155, 255);

	const static int COLOR_NUM = 4;
	float accumMoveAmount = 0;
	const ofColor RectColor[COLOR_NUM] =
	{
		ofColor(107 , 167 , 223),
		ofColor(226 , 110 , 247),
		ofColor(87 , 188 , 98),
		ofColor(233,189,55)
	};
	int ColorIndex = 0;
	void clearAccumMoveAmount();

public:
	SPMania( );
	~SPMania( );
	void onTouch(InputParam p) override;
	void onMovement(std::vector<float>& movement) override;
	void onSoundSpectrum(float* spectrum) override;
	ISpell* init(int x , int y , int w , int h , float deg = 0) override;
	void setup(int keyBand = 0 , int bands = 0 , int hSpacing = 30 , int width = 40 , ManiaType type = ManiaDot);
	void setXflipped(bool flag);
	void update( ) override;
	void customDraw( ) override;
	void updateParams(int width , int hSpacing);
	void dotDraw( );
	void barDraw( );
};
