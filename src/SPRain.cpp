#include "SPRain.h"

SPRain::SPRain( )
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName;
}


SPRain::~SPRain( )
{
	cout << spellName << " constructor called " << --AMOUNT << endl;
	for(int i = 0; i < MaxRainNum; i++)
	{
		delete rainPool[i];
	}
	delete[] rainPool;

}

void SPRain::setup(int maxRainNum,int keyb, float speedrate)
{
	this->speedRate = speedrate;
	MaxRainNum = maxRainNum;
	keyBand = keyb;
	rainPool = new RainLine*[MaxRainNum];
	for(int i = 0; i < MaxRainNum; i++)
	{
		rainPool[i] = new RainLine(ofRandom(-viewPort.width  , viewPort.width+200)
								   , ofRandom(-30 , -130) - viewPort.height / 2
								   , ofVec2f(ofRandom(0.5f) , ofRandom(0.4f))
								   , ofVec2f(ofRandom(.3) , ofRandom(2 , 4))
		);
	}
}

void SPRain::setXflip(bool flag)
{
	bXflipped = flag;
}

void SPRain::DebugSetSpeedRate(float rate)
{
	speedRate = rate;
}

void SPRain::DebugSetRainPercents(float percents)
{
	percents = ofClamp(percents , 0 , 1);
	activeRainNum = MaxRainNum*percents;
}

ISpell * SPRain::init(int x , int y , int w , int h , float deg)
{
	commonInit(x , y , w , h , deg);
	return this;
}

void SPRain::update( )
{
	if(!bIsActive)
	{
		return;
	}
	float timeStep = ofGetLastFrameTime();
	for(int i = 0; i < activeRainNum; i++)
	{
		rainPool[i]->vel += (rainPool[i]->acc+ofVec2f(accXOffset,0))*timeStep*20;
		rainPool[i]->pos += rainPool[i]->vel*speedRate;
		//落地
		auto dist = viewPort.height / 2 - rainPool[i]->pos.y;
		if(dist <  100 )
		{
			rainPool[i]->bOnFloor = true;
			rainPool[i]->rippleAlpha = ofMap(
				dist
				, 0 , 100
				, 0 , 255
				, true);
			// 首次到达 floor
			if(!rainPool[i]->bEnterFloor)
			{
				rainPool[i]->bEnterFloor = true;
				rainPool[i]->ripplePos = ofVec2f(rainPool[i]->pos.x
												 , rainPool[i]->pos.y + ofRandom(-30 , 30));
			}
		}
		//越界一定程度回到顶部重新下落
		if(dist < 0)
		{
			rainPool[i]->bOnFloor = false;
			rainPool[i]->bEnterFloor = false;
			int birthX = bXflipped ?
				ofRandom(-viewPort.width - 200 , viewPort.width / 2)
				: ofRandom(-viewPort.width / 2 , viewPort.width + 200);
			rainPool[i]->set(birthX
							, ofRandom(-30 , -130) - viewPort.height / 2
							, ofVec2f(ofRandom(0.5f) , ofRandom(0.4f))
							, ofVec2f(ofRandom(.3) , ofRandom(2 , 4)));
		}
	}
}

void SPRain::customDraw( )
{
	ofNoFill( );
	ofSetColor(233);
	for(int i = 0; i < activeRainNum; i++)
	{
		rainPool[i]->draw( );
	}
}

void SPRain::onTouch(InputParam p)
{
	if(!bIsActive)
	{
		return;
	}
}
//
//void SPRain::onMovement(std::vector<float>& movement)
//{
//	float meanMove = 0;
//	for(int i = 0; i < movement.size( ); i++)
//	{
//		meanMove += movement[i];
//	}
//	if(movement.size( ) > 0)
//	{
//		meanMove /= movement.size( );
//	}
//	activeRainNum = ofMap(meanMove
//					  , 0 , 1
//					  , 0.05f , 1
//					  , true);
//}

void SPRain::onMeanMove(float meanmove)
{
	meanMove *= 0.97f;
	meanmove -= 0.024f;
	if(!bIsActive)
	{
		return;
	}
	//cout << meanmove << endl;
	maxMove = max(maxMove, meanmove);
	meanMove = max(meanMove, meanmove);
	activeRainNum = ofMap(meanmove
						  , 0 , 0.02f
						  , 0.05f*MaxRainNum , MaxRainNum
						  , true);
}

void SPRain::onSoundSpectrum(float * spectrum)
{
	bandAmount *= 0.7f;
	accXOffset *= 0.97f;

	//DebugMaxBandAmount = max(DebugMaxBandAmount , bandAmount);
	bandAmount = max(bandAmount , spectrum[keyBand]);

	accXOffset = ofMap(bandAmount
					   , 0 , 0.2f
					   , 0 , 6 * (bXflipped ? 1 : -1)
					   , false);
/*
	cout << spectrum[keyBand]
		<< "num" << endl
		<< activeRainNum << endl
		<< "max" << DebugMaxBandAmount << endl;*/
}

