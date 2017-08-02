#include "SPEnterWave.h"
#include "InputManager.h"
#include "GLOBALS.h"


//------类静态成员初始化-----------

void SPEnterWave::customDraw()
{
	wave.draw( );
	/*ofTranslate(1 , 1);
	wave.draw( );
	ofTranslate(0 , 1);
	wave.draw( );
	ofTranslate(0 , 1);
	wave.draw( );
	ofTranslate(0 , 1);
	wave.draw( );*/
}

void SPEnterWave::update()
{
	if(!bIsActive)
	{
		return;
	}
	v -= 0.02*ofGetLastFrameTime()*30;
	w += 0.04*ofGetLastFrameTime()*30;
	auto ptr = InputManager::Instance().getClosestTouch(viewPort.getCenter().x,viewPort.getCenter().y);
	if(ptr!=nullptr && viewPort.inside(ptr->x,ptr->y))
	{
		auto posInView = mapScreenPosToViewPort(ptr->x , ptr->y);
		//宽比长 要多  --》 交换 x y 坐标
		if(viewPort.width < viewPort.height)
		{
			int tmp = posInView.x;
			posInView.x = posInView.y;
			posInView.y = tmp;
		}
		paramA	= posInView.y;
		paramB = posInView.x;
	}
	else
	{
		paramA = 7;
		paramB = width/2;
	}
	for(float x =0; x < width; x ++)
	{
		wave[x].set(x-width/2 ,   paramB
				 *sin(v + (x / ofMap(paramA , 0 , width , 1 , 100 , true)))
				 *tan(w + (x / ofMap(paramB , 0 , width , 100 , 400 , true)))
				 *sin(v + (x / ofMap(paramB , 0 , height , 1 , 100 , true)))
		);
	}
}

SPEnterWave::~SPEnterWave()
{
		cout << spellName << " destructor called " << --AMOUNT << endl;
}

SPEnterWave::SPEnterWave()
{
		spellName = NamePrefix + ofToString(++AMOUNT);
		cout << spellName << " constructor called "  << endl;
		receiverName = spellName;
}

ISpell* SPEnterWave::init(int x, int y, int w, int h, float deg)
{
	commonInit(x , y , w , h , deg);
	setup(w , h);
	return this;
}

void SPEnterWave::onTouch(InputParam p)
{

}

void SPEnterWave::setup(int width , int height)
{
	this->width = width;
	this->height = height;
	wave.clear( );// setup 可能被重新调用。 故先清除之前的
	for(float i = 0; i < width; i++)
	{
		wave.addVertex(i - width / 2 , paramB
					   *sin(v + (i / ofMap(paramA , 0 , width , 1 , 100 , true)))
					   *tan(w + (i / ofMap(paramB , 0 , width , 100 , 400 , true)))
					   *sin(v + (i / ofMap(paramB , 0 , height , 1 , 100 , true)))
		);
	}
}
//
//void SPEnterWave::updateParams(int lineWidth)
//{
//	this->lineWidth = lineWidth;
//}
