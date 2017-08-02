#pragma once
#include "ofMain.h"
#include "ISimuInputReceiver.h"
#include "ISpell.h"

class SPBallMatrix :public ISpell , public ISimuInputReceiver
{
public:
	SPBallMatrix( );
	~SPBallMatrix( );

	void setup(int ballPerRow , int keyBand);

	// 通过 ISpell 继承
	ISpell * init(int x , int y , int w , int h , float deg) override;
	void update( ) override;
	void customDraw( ) override;

	// 通过 ISimuInputReceiver 继承
	void onTouch(InputParam p) override;
	void onMovement(std::vector<float>& movement) override;
	void onMeanMove(float meanmove) override;
	void onSoundSpectrum(float* spectrum) override;
private:
	void drawSphere(int x , int y , int z);
	ofSpherePrimitive **balls=nullptr;
	//ofVboMesh **ballMeshs = nullptr;
	const string NamePrefix = "BallMatrix";
	float* spectrum = nullptr;

	float speedRate = 1;
	float matScale = 0.2f;
	float bandAmount = 0;
	float diameterScale = 1;
	float spacingScale = 1;
	float moveAmount = 0;

	int keyBand = 0;
	int ballSpacing = 40;
	int ballRadius = 10;
	int ballNumPerRow = 9;
	float rotateSpeed = 1;
	
	ofLight light;
	ofMaterial material;

};

