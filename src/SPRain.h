#pragma once
#include "ofMain.h"
#include "ISimuInputReceiver.h"
#include "ISpell.h"

class RainLine
{
public:
	ofVec2f vel , acc , pos,ripplePos;
	bool bOnFloor = false;
	bool bEnterFloor = false;
	int lineLen = 50;
	int rippleAlpha = 255;
	int rippleWidth = 13;

	RainLine( int x,int y,ofVec2f vel,ofVec2f acc)
	{
		pos.set(x , y);
		this->acc = acc;
		this->vel = vel;
	};
	~RainLine( ) = default;

	void set(int x , int y , ofVec2f vel , ofVec2f acc)
	{
		pos.set(x , y);
		this->acc = acc;
		this->vel = vel;
		lineLen = ofRandom(20 , 70);
		rippleWidth = ofRandom(15 , 25);
	}

	void draw( )
	{
		if(!bOnFloor)
		{
			ofLine(pos - vel.getNormalized( )*lineLen
				   , pos);
		}
		else
		{
			ofPushStyle( );
				ofSetColor(rippleAlpha );
				//ofDrawEllipse(ripplePos, rippleWidth,rippleWidth*2/3);
				ofDrawCircle(ripplePos , rippleWidth);
			ofPopStyle( );
		}
	}

};


class SPRain:public ISpell , public ISimuInputReceiver
{
public:
	SPRain( );
	~SPRain( );

	void setup(int maxRainNum,int keyBand,float speedRate=0.43f );
	void setXflip(bool flag);
	void DebugSetSpeedRate(float rate);
	void DebugSetRainPercents(float percents);

	// 通过 ISpell 继承
	ISpell * init(int x , int y , int w , int h , float deg) override;
	void update( ) override;
	void customDraw( ) override;

	// 通过 ISimuInputReceiver 继承
	void onTouch(InputParam p) override;
	//void onMovement(std::vector<float>& movement) override;
	void onMeanMove(float meanmove) override;
	void onSoundSpectrum(float* spectrum) override;
private:
	const string NamePrefix = "Rain";
	float* spectrum = nullptr;
	int keyBand = 0;
	RainLine **rainPool=nullptr;
	int MaxRainNum = 100;
	int activeRainNum = 80;
	bool bXflipped = false;

	float speedRate = 0.3f;
	float bandAmount = 0;
	float accXOffset = 0;
	float meanMove = 0;
	//float DebugMaxBandAmount = 0;
	float maxMove = 0;
};

