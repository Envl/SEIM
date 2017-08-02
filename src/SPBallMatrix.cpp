#include "SPBallMatrix.h"



SPBallMatrix::SPBallMatrix( )
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName;
}


SPBallMatrix::~SPBallMatrix( )
{
	cout << spellName << " constructor called " << --AMOUNT << endl;
	

}

void SPBallMatrix::setup(int rowBallNum,int keyb)
{
	ofSetSphereResolution(15);
	ballNumPerRow = rowBallNum;
	keyBand = keyb;
	balls = new ofSpherePrimitive*[ballNumPerRow*ballNumPerRow*ballNumPerRow];
	for(int x = 0; x < ballNumPerRow; x++)
	{
		for(int y = 0; y < ballNumPerRow; y++)
		{
			for(int z = 0; z < ballNumPerRow; z++)
			{
				auto myX = -ballSpacing*ballNumPerRow / 2 + ballSpacing*x + ballSpacing / 2;
				auto myY = -ballSpacing*ballNumPerRow / 2 + ballSpacing*y + ballSpacing / 2;
				auto myZ = -ballSpacing*ballNumPerRow / 2 + ballSpacing*z + ballSpacing / 2;
				auto c = ofColor(x*255.0f / ballNumPerRow
								 , y*255.0f / ballNumPerRow
								 , z*255.0f / ballNumPerRow);
				auto p = new ofSpherePrimitive( ballRadius,8);
				p->setPosition(myX , myY , myZ);
			/*	cout << p->getMeshPtr( )->getNumVertices( ) << endl;
				for(int i = 0; i < m->getNumVertices(); i++)
				{
					m->addColor(c);
				}*/
				balls[x*ballNumPerRow*ballNumPerRow
					+ y*ballNumPerRow
					+ z] = p;
			
			}
		}
	}
}

ISpell * SPBallMatrix::init(int x , int y , int w , int h , float deg)
{
	commonInit(x , y , w , h , deg);
	light.setDirectional( );
	light.setDiffuseColor(ofColor::greenYellow);
	light.setSpecularColor(ofColor::white);
	light.setPosition(1100 , 1100 , -10000);
	light.lookAt(ofVec3f(0));
	material.setShininess(68);
	return this;
}

void SPBallMatrix::update( )
{
	moveAmount *= 0.9f;
	if(!bIsActive)
	{
		return;
	}
	rotateSpeed = ofMap(bandAmount
						, 0 , 2
						, 0.02f , 1.0f
						, true);
	float t = ofGetLastFrameTime( )*rotateSpeed * 360;
	rotate(t , 1 , 0 , 0);
	rotate(t , 0 , 1 , 0);
	rotate(t , 0 , 0 , 1);
	//setScale(ofGetMouseY( ) / (float)(ofGetHeight( ) / 2));
}

void SPBallMatrix::customDraw( )
{
	ofEnableDepthTest( );

	/*ofEnableLighting( );
	light.enable( );
	material.begin( );*/
	for(int x = 0; x < ballNumPerRow; x++)
	{
		for(int y = 0; y < ballNumPerRow; y++)
		{
			for(int z = 0; z < ballNumPerRow; z++)
			{
				drawSphere(x , y , z);
			}
		}
	}
	/*material.end( );
	ofDisableLighting( );*/
	ofDisableDepthTest( );
}

void SPBallMatrix::onTouch(InputParam p)
{
	if(!bIsActive)
	{
		return;
	}
}

void SPBallMatrix::onMovement(std::vector<float>& movement)
{
	if(!bIsActive)
	{
		return;
	}
	
}

void SPBallMatrix::onMeanMove(float meanmove)
{
	if(!bIsActive)
	{
		return;
	}
	meanmove -= 0.024f;
	moveAmount = max(moveAmount , meanmove);
	setScale(ofMap(moveAmount
				   , 0 , 0.02f
				   , 0.2f , 1.7f
				   , true));
}

void SPBallMatrix::onSoundSpectrum(float * spectrum)
{
	if(!bIsActive)
	{
		return;
	}
	bandAmount *= 0.73f;
	 bandAmount= max( spectrum[keyBand],bandAmount);
/*
	cout << spectrum[keyBand]
		<< "num" << endl
		<< activeRainNum << endl
		<< "max" << DebugMaxBandAmount << endl;*/
}

void SPBallMatrix::drawSphere(int x , int y , int z)
{
	auto myX = -ballSpacing*ballNumPerRow / 2 + ballSpacing*x + ballSpacing / 2;
	auto myY = -ballSpacing*ballNumPerRow / 2 + ballSpacing*y + ballSpacing / 2;
	auto myZ = -ballSpacing*ballNumPerRow / 2 + ballSpacing*z + ballSpacing / 2;
	auto c = ofColor(x*255.0f / ballNumPerRow
					 , y*255.0f / ballNumPerRow
					 , z*255.0f / ballNumPerRow);
	light.setDiffuseColor(c
	); 
	
	ofEnableLighting( );
	light.enable( );
	material.begin( );
	balls[x*ballNumPerRow*ballNumPerRow
		+ y*ballNumPerRow
		+ z]->draw( );
		//ofDrawSphere(myX , myY , myZ , ballRadius);

	material.end( );
	ofDisableLighting( );
}

