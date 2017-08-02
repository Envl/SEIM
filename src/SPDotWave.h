#pragma once
#include "ofMain.h"
#include "ISimuInputReceiver.h"
#include "ISpell.h"

class Circle
{
public:
	Circle( int x,int y,float rt=500, float size=50)
	{
		cX = x;
		cY = y;
		rT = rt;
		offset = ofRandom(-PI / 8 , PI / 8);
		this->size = size;
	}
	~Circle( ) = default;

	float cX , cY , w , h;
	float rT;
	float offset;
	float size;
};


class SPDotWave:public ISpell , public ISimuInputReceiver
{
public:
	SPDotWave( );
	~SPDotWave( );

	void setup(int space,int keyBand);

	// 通过 ISpell 继承
	ISpell * init(int x , int y , int w , int h , float deg) override;
	virtual void update( ) override;
	virtual void customDraw( ) override;

	// 通过 ISimuInputReceiver 继承
	virtual void onTouch(InputParam p) override;
	//void onMovement(std::vector<float>& movement) override;
	//void onMeanMove(float meanmove) override;
	void onSoundSpectrum(float* spectrum) override;
private:
	const string NamePrefix = "DotWave";
	float* spectrum = nullptr;
	vector<Circle> dotPool;
	ofColor color=ofColor(255);

	int keyBand = 0;
	float bandAmount;
	float space;
	float rT = 500;
	float SIZE = 50;
};

