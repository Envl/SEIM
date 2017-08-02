#pragma once
#include "ofMain.h"
#include "ISpell.h"
#include "ISimuInputReceiver.h"

class SPEnterWave:public ISpell,public ISimuInputReceiver
{
public:
	void customDraw() override;
	void update() override;
	~SPEnterWave();
	SPEnterWave();
	ISpell* init(int x, int y, int w=300, int h=200, float deg=0) override;
	void onTouch(InputParam p) override;
	void setup(int width , int height);
	//void updateParams(int lineWidth);

	const string NamePrefix="EnterWave";
private:
	//调节 wave 形状的参数
	float paramA = 1, paramB = 1;
	float v = 0, w = 0, rotation = 0;
	int lineWidth = 1;
	ofPolyline wave;
};

