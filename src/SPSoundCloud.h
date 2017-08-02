#pragma once
#include <ofVboMesh.h>
#include "ISimuInputReceiver.h"
#include "ISpell.h"

class SPSoundCloud :public ISimuInputReceiver , public ISpell
{
private:
	float velocity = 0.1f;
	int bandRad = 2;
	int bandVel = 15;
	int pointNum = 200;
	float deltaT = 0;// delta time per frame
	ofVboMesh mesh;
	float(*offset)[2] = nullptr;
	float *spectrum = nullptr;
	float bandNumber = 0;
	ofColor meshColor = ofColor(233 , 233 , 123);
	const string NamePrefix = "SoundCloud";
	float scale = 1;
public:
	SPSoundCloud( ) ;
	~SPSoundCloud( );
	void setup(int bandNum , int pNum = 200 ,float scale=1, int bRad = 2 , int bVel = 15 , float vel = 0.1f);

	void onSoundSpectrum(float* spectrum) override;
	void onTouch(InputParam p) override;
	ISpell* init(int x , int y , int w , int h , float deg=0) override;
	void update( ) override;
	void customDraw( ) override;
};
