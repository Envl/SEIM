#include "SPMania.h"

SPMania::SPMania( )
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName;
}

SPMania::~SPMania( )
{
	delete[] tintBarHeight;
	cout << spellName << " constructor called " << --AMOUNT << endl;
}

ISpell* SPMania::init(int x , int y , int w , int h , float deg)
{
	commonInit(x , y , w , h , deg);

	return this;
}

void SPMania::setup(int keyBand , int bands , int hSpacing , int width , ManiaType type)
{
	this->type = type;
	this->keyBand = keyBand;
	soundBands = bands;
	this->hSpacing = hSpacing;
	barWidth = width;
	barNum = viewPort.width / (hSpacing + barWidth);
	tintBarHeight = new float[barNum]( );
	for(int i = 0; i<barNum; i++)
	{
		tintBarHeight[i] = 0;
	}
}

void SPMania::setXflipped(bool flag)
{
	flipX = flag;
}

void SPMania::onTouch(InputParam p)
{
}

void SPMania::onMovement(std::vector<float>& movement)
{
	if(!bIsActive)
	{
		return;
	}
	int barIndex = 0;
	int len = movement.size( );
	for(int i = 0; i < len; )
	{
		float amount = 0;
		while(1.0f*i*barNum / len<barIndex + 1)
		{
			//amount = max(amount , movement[i]);
			amount += movement[i];
			i++;
		}
		amount -= 0.15f;
		amount = ofClamp(amount, 0, 150);
		
		//cout << amount << endl;
		if(amount>1) // 第一帧图像时  diff 像素值为一个超大值  要排除
		{
			break;
		}

		if(flipX)
		{
			tintBarHeight[(barNum - 1) - barIndex] *= 0.95f;
			tintBarHeight[(barNum - 1) - barIndex] = max(amount , tintBarHeight[(barNum - 1) - barIndex]); 
		}
		else
		{
			tintBarHeight[barIndex] *= 0.95f;
			tintBarHeight[barIndex] = max(amount , tintBarHeight[barIndex]);
		}
		barIndex	= ofClamp(++barIndex , 0 , barNum - 1);
	}
	// 积累填充色块
	float meanAmount = 0;
	for (auto& v : movement) {
		meanAmount += v;
	}
	if (movement.size() > 0) {
		meanAmount /= movement.size();
	}
	meanAmount -= 0.0235f;
	meanAmount = ofClamp(meanAmount, 0, 100); 
	accumMoveAmount += meanAmount*1666;
	//cout << accumMoveAmount << endl;
	//超过后缩小回去
	if (accumMoveAmount > viewPort.height) {
		clearAccumMoveAmount();
	}
}

void SPMania::onSoundSpectrum(float* spectrum)
{
	if(!bIsActive)
	{
		return;
	}
	offset.set(
		ofMap(spectrum[keyBand]
			  , 0 , 1
			  , -hSpacing / 3.0f , hSpacing / 3.0f
			  , true)
		,
		ofMap(spectrum[keyBand]
			  , 0 , 1
			  , 0 , hSpacing / 5.0f
			  , true)
	);
}

void SPMania::update( )
{
	if(!bIsActive)
	{
		return;
	}
}


void SPMania::customDraw( )
{
	if(type == ManiaDot)
	{
		dotDraw( );
	}
	else if(type == ManiaBar)
	{
		barDraw( );
	}

}

void SPMania::updateParams(int width , int hSpacing)
{
	if(!bIsActive)
	{
		return;
	}
	delete[] tintBarHeight;
	setup(keyBand , soundBands , hSpacing , width , type);
}

void SPMania::dotDraw( )
{
	ofFill( );
	//bg rect fill
	ofSetColor(RectColor[ColorIndex]);
	ofDrawRectangle(-viewPort.width / 2, viewPort.height / 2
		, viewPort.width, -accumMoveAmount);
	//draw dots
	ofColor c;
	ofTranslate(offset);
	ofSetCircleResolution(9);
	ofSetColor(255);
	int top = -viewPort.height / 2;
	int hNum = viewPort.height/(barWidth+5); // 垂直方向点个数
	int r = barWidth / 2;

	int vSpacing = viewPort.height / hNum;
	for(int i = 0; i<barNum; i++)
	{
		int posX = i*(hSpacing + barWidth) - viewPort.width / 2;
		int colorNum = ofClamp(tintBarHeight[i] * viewPort.height*2.0f
							   , 0 , viewPort.height)*1.5 / (vSpacing +r);
		for(int j = 0; j<hNum - colorNum; j++)
		{
			//ofCircle(r + posX, r + j*vSpacing + top, r);
			ofEllipse(r + posX, r + j*vSpacing + top, r,r*2);
		}
	}
	for(int p = 0; p<barNum; p++)
	{
		int posX = p*(hSpacing + barWidth) - viewPort.width / 2;
		int colorNum = ofClamp(tintBarHeight[p] * viewPort.height*2.0f
							   , 0 , viewPort.height)*1.5 / (vSpacing+r );
		//int colorR = 100*p / barNum+155;
		ofColor cu = colorUL.getLerped(colorUR,(float)p / barNum);
		ofColor cd = colorDL.getLerped(colorDR, (float)p / barNum);
		// one column
		for(int j = hNum - colorNum; j<hNum; j++)
		{
			//c.set( colorR,255.0f*j / hNum , 125);
			c = cu.getLerped(cd, (float)j / hNum);
			ofSetColor(c);
			//ofCircle(r + posX , r + j*vSpacing + top , r);
			ofEllipse(r + posX, r + j*vSpacing + top, r , r*2);

		}
	}
}

void SPMania::barDraw( )
{
	ofFill( );
	ofColor c;
	//	ofSetLineWidth(barWidth);
	//	int singleW = viewPort.width / 32 * (2 / 3.0f);
	ofTranslate(offset);
	ofSetColor(255);
	int top = -viewPort.height / 2;
	for(int i = 0; i<barNum; i++)
	{
		int posX = i*(hSpacing + barWidth) - viewPort.width / 2;
		ofDrawRectangle(posX , -viewPort.height / 2 , barWidth , viewPort.height);
		//		ofDrawLine(posX , top , posX , viewPort.height);

	}
	for(int i = 0; i < barNum; i++)
	{
		int posX = i*(hSpacing + barWidth) - viewPort.width / 2;
		int height = -ofClamp(tintBarHeight[i] * viewPort.height*1.3f
							  , 0 , viewPort.height);
		//		c.setHsb(255.0f*i / barNum
		//				 , ofClamp(tintBarHeight[i] * 255 + 100 , 0 , 255)
		//				 , ofClamp(tintBarHeight[i] * 255 + 100 , 0 , 255));
		c.set(233 , 255.0f*i / barNum , 125 );
		ofSetColor(c);
		ofDrawRectangle(posX , viewPort.height / 2 , barWidth , height);
		//		ofDrawLine(posX , -top , posX , height);
	}
}

void SPMania::clearAccumMoveAmount()
{
	accumMoveAmount = 0;
	ColorIndex = ofRandom(COLOR_NUM);
}
