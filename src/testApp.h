#pragma once
#include "ofMain.h"
#include "SPRain.h"
#include "SPBallMatrix.h"
#include "SPDotWave.h"

class testApp:public ofBaseApp
{
	ofSoundPlayer sound;
	float *pSpectrum;

public:
	void setup( );
	void update( );
	void draw( );
	void keyPressed(int key) override;

	SPDotWave dotwave;
	SPBallMatrix ballMat;
	SPRain rain , rainL;
};
