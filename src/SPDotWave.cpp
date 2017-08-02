#include "SPDotWave.h"



SPDotWave::SPDotWave( )
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName;
}


SPDotWave::~SPDotWave( )
{
	cout << spellName << " constructor called " << --AMOUNT << endl;

}

void SPDotWave::setup(int space,int keyb)
{
	keyBand = keyb;
	this->space = space;
	for(int i = -2 * space; i <= viewPort.width + space; i += space)
	{
		for(int j = -2 * space; j <= viewPort.height + space; j += space)
		{
			dotPool.push_back(Circle(i , j));
		}
	}
}

ISpell * SPDotWave::init(int x , int y , int w , int h , float deg)
{
	commonInit(x , y , w , h , deg);
	return this;
}

void SPDotWave::update( )
{
	if(!bIsActive)
	{
		return;
	}
	//rT = ofMap((float)ofGetMouseY( ) / ofGetHeight( )
	//		   , 0 , 1
	//		   , 450 ,60
	//		   , true);//debug 
	float mt = ofGetElapsedTimeMillis( ) / rT;
	for(auto &d : dotPool)
	{
		float t = mt
			+ cos(abs(1 - (d.cX / (viewPort.width / 2))))*TWO_PI
			+ cos(abs(1 - (d.cY / (viewPort.height / 2))))*TWO_PI
			+ d.offset;
		d.w = sin(t)*d.size;
		d.h = d.w == 0 ? 0 : d.w / 2;
	}
}

void SPDotWave::customDraw( )
{
	ofFill( );
	ofSetColor(color);
	ofTranslate(-width / 2 , -height / 2);
	for(auto &d:dotPool)
	{
		if(d.w > 0 && d.h > 0)
		{
			ofDrawEllipse(d.cX , d.cY , d.w , d.h);
		}
	}
}

void SPDotWave::onTouch(InputParam p)
{
	if(!bIsActive)
	{
		return;
	}
	if(p.type == InputType::LIGHT)
	{
		color.set(p.x , p.y , p.force);
	}
}

void SPDotWave::onSoundSpectrum(float * spectrum)
{
	//bandAmount *= 0.7f;
	//bandAmount = max(bandAmount , spectrum[keyBand]);
	rT = ofLerp(
		rT
		, 250
		, 0.1f);
	bandAmount *= 0.997f;
	if(spectrum[keyBand] > bandAmount)
	{
		bandAmount = spectrum[keyBand];
		rT = ofLerp(rT
					, 120
					, 0.1f);
	}
}

