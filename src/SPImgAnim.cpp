#include "SPImgAnim.h"


SPImgAnim::~SPImgAnim()
{
	cout << spellName << " destructor called " << --AMOUNT << endl;
	receiverName = spellName;
}

SPImgAnim::SPImgAnim()
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName;
}

void SPImgAnim::customDraw()
{
	ofSetColor(color);
	ofPushMatrix( );
	//默认以 viewport 中心为原点, 移到左上角为原点.
	ofTranslate(-viewPort.width / 2 , -viewPort.height / 2);
		textures_[currentTex].draw(offset);
		if(animType == SingleFrameManual || animType == SingleFrameManual)
		{
			textures_[currentTex].draw(offset.x - textures_[currentTex].getWidth( ) , offset.y - textures_[currentTex].getHeight( ));
			textures_[currentTex].draw(offset.x - textures_[currentTex].getWidth( ) , offset.y);
			textures_[currentTex].draw(offset.x , offset.y - textures_[currentTex].getHeight( ));
		}
	ofPopMatrix( );
}

void SPImgAnim::update()
{
	if(!bIsActive)
	{
		return;
	}
	if(animType == SingleFrameAuto)
	{
		offset.set(ofWrap(delta.x + offset.x , 0 , textures_[currentTex].getWidth( ))
				   , ofWrap(offset.y + delta.y , 0 , textures_[currentTex].getHeight( )));
	}
	else if(animType==MultiFrame)
	{
		currentTex += ofGetLastFrameTime( )*FPS;
		currentTex = ofWrap(currentTex , 0 , textures_.size( ));
	}
}

void SPImgAnim::onTouch(InputParam p)
{
	if(!bIsActive)
	{
		return;
	}
	if(p.type != LIGHT)
		return;
	if(animType==SingleFrameManual)
	{
		offset.set(ofClamp(p.x , 0 , viewPort.width)
				   , ofClamp(p.y , 0 , viewPort.height));
	}
	else if(animType==SingleFrameAuto)
	{
		delta.set(p.x , p.y);
	}
	else if(animType==MultiFrame)
	{
		FPS = p.force;
	}
}

ISpell* SPImgAnim::init(int x, int y, int w, int h, float rotationDeg)
{
	commonInit(x , y , w , h , rotationDeg);

	return this;
}

void SPImgAnim::setupTexture(vector<ofTexture> texs)
{
	textures_.swap(texs);
	animType = textures_.size( ) > 1 ? MultiFrame : 
		(bSingleAuto? SingleFrameAuto:SingleFrameManual);
}

void SPImgAnim::setParameters(ofVec2f offsetDelta, int fps,bool singleAuto, ofColor color)
{
	bSingleAuto = singleAuto;
	FPS = fps;
	delta = offsetDelta;
	this->color = color;
	
}

void SPImgAnim::setColor(ofColor newColor)
{
	color = newColor;
}

int SPImgAnim::getTexNum()
{
	return textures_.size( );
}
