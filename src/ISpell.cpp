#include "ISpell.h"

int ISpell::AMOUNT = 0;

ofVec2f ISpell::mapScreenPosToViewPort(int x,int y)
{
	return ofVec2f(ofClamp(x - viewPort.getLeft( ) , 0 , viewPort.width)
				   , ofClamp(y - viewPort.getTop( ) , 0 , viewPort.height));
}

/* 检测输入的点是否在绘图区域内 */
bool ISpell::checkPosInViewport(int x,int y)
{
	return viewPort.inside(ofPoint(x,y));
}

ofRectangle ISpell::getViewport( )
{
	return viewPort;
}

void ISpell::Activate( )
{
	bIsActive = true;
}

void ISpell::Deactivate( )
{
	bIsActive = false;
}

void ISpell::draw() 
{
	if(!bIsActive)
	{
		return;
	}
	ofPushStyle( );
	ofPushView( );
	ofViewport(viewPort.x , viewPort.y , viewPort.width , viewPort.height);
	ofSetupScreen( );
	ofNoFill( );
	transformGL( );
	
	customDraw( );

	restoreTransformGL( );
	ofPopView( );
	ofPopStyle( );
}

void ISpell::commonInit(int x, int y, int w, int h, int deg)
{
	setPosition(w/2 , h/2 , 0);//位置放到 viewport 中心
	rotate(deg , 0 , 0 , 1);
//	viewPort.set(x - w / 2 , y - h / 2 , w , h);
	viewPort.set(x , y , w , h);
	width = w;
	height = h;
}

void ISpell::finish()
{
	delete this;
}

string ISpell::getSpellName() const
{
	return spellName;
}

