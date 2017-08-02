#include "SPSoundCloud.h"
#include <ofMatrixStack.h>
#include <ofEasyCam.h>

SPSoundCloud::SPSoundCloud()
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName;
}

SPSoundCloud::~SPSoundCloud( )
{
	delete[] spectrum;
	delete[] offset;
	cout << spellName << " destructor called " << --AMOUNT << endl;
}

void SPSoundCloud::setup(int bandNum , int pNum ,float scale,  int bRad , int bVel , float vel)
{
	this->scale = scale;
	bandNumber = bandNum;
	pointNum = pNum;
	bandRad = bRad;
	bandVel = bVel;
	velocity = vel;

	mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	spectrum = new float[bandNumber];
	offset = new float[pointNum][2];
	for(int i = 0; i<pointNum; i++)
	{
		mesh.addVertex(ofVec3f( ));
		offset[i][0] = ofRandom(0 , viewPort.width);
		offset[i][1] = ofRandom(0 , viewPort.height);
		auto a = offset[i][0];
		auto b = offset[i][1];
	}
}

void SPSoundCloud::onSoundSpectrum(float* spectrum)
{
	if(!bIsActive)
	{
		return;
	}
	for(int i = 0; i<bandNumber; i++)
	{
		this->spectrum[i] = max(spectrum[i] , this->spectrum[i]);
	}
	float dt = ofClamp(ofGetLastFrameTime( ) , 0 , 0.1f);
	float newHR = scale*ofMap(this->spectrum[bandRad] , 1 , 3 , viewPort.width * 0.4f , viewPort.width * 0.8f);
	float newVR = scale*ofMap(this->spectrum[bandRad] , 1 , 3 , viewPort.height * 0.4f , viewPort.height * 0.8f);
	velocity = ofMap(this->spectrum[bandVel] , 0 , 0.1f , 0.05f*0.8f , 0.05f * 5);

	for(int j = 0; j<pointNum; j++)
	{
		offset[j][0] += velocity*dt;
		offset[j][1] += velocity*dt;
		mesh.setVertex(j , ofVec3f(ofSignedNoise(offset[j][0])*newHR ,
								   ofSignedNoise(offset[j][1])*newVR));
	}

	//clear former lines
	mesh.clearIndices( );
	// 连线
	for(int a = 0; a<pointNum; a++)
	{
		for(int b = a + 1; b<pointNum; b++)
		{
			if(ofDistSquared(mesh.getVertex(a).x , mesh.getVertex(a).y
							 , mesh.getVertex(b).x , mesh.getVertex(b).y)<50 * 50)
			{
				mesh.addIndex(a);
				mesh.addIndex(b);
			}
		}
	}
}

void SPSoundCloud::onTouch(InputParam p)
{
	if(!bIsActive)
	{
		return;
	}
	if(p.type == InputType::LIGHT)
	{
		meshColor.set(p.x , p.y , p.force);
		meshColor *= 1.1f;
	}
}

ISpell* SPSoundCloud::init(int x , int y , int w , int h , float deg)
{
	commonInit(x , y , w , h , deg);
	return this;
}

void SPSoundCloud::update( )
{
	if(!bIsActive)
	{
		return;
	}
	for(int i = 0; i<bandNumber; i++)
	{
		spectrum[i] *= 0.97f;
	}
}

void SPSoundCloud::customDraw( )
{
	ofSetColor(255);
	mesh.draw( );
	ofSetColor(meshColor);
	ofFill( );
	for(int i = 0; i<pointNum; i++)
	{
		ofCircle(mesh.getVertex(i) , ofRandom(8,16));
	}
}
