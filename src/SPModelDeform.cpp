#include "SPModelDeform.h"

void SPModelDeform::drawDebug()
{
	ofPushStyle( );

	cam.begin( );
	ofSetColor(255 , 0 , 0);// x axis
	ofDrawLine(0 , 0 , 0 , 200 , 0 , 0);
	ofSetColor(0 , 255 , 0);// y axis
	ofDrawLine(0 , 0 , 0 , 0 , 200 , 0);
	ofSetColor(0 , 0 , 255);// z axis
	ofDrawLine(0 , 0 , 0 , 0 , 0 , 200);
	cam.end( );

	ofPopStyle( );
}

ISpell* SPModelDeform::init(int x, int y, int w, int h, float deg)
{
	commonInit(x , y , w , h , deg);
	cam.setPosition(0 , 0 , 200);
	return this;
}

void SPModelDeform::setup(string path2model, int keyBand , int bands,int activeModel)
{
	activeIndex = activeModel;
	addModel(path2model);
	bandNumber = bands;
	this->keyBand = keyBand;
}

void SPModelDeform::setActiveModel(int index)
{
	activeIndex = index;
}

void SPModelDeform::onSoundSpectrum(float* spectrum)
{
	if(!bIsActive)
	{
		return;
	}
	camDist = ofMap(spectrum[keyBand]
					, 0 , 1
					, DEFAULT_CAM_DIST , MIN_CAM_DIST
					, true);
}

void SPModelDeform::addModel(string modelPath)
{
	model.loadModel(modelPath , true);
	auto m = model.getMesh(0);
	models.push_back(m);
	originModels.push_back(m);
	//计算所有 vertex 相对中心的向量
	auto center = m.getCentroid( );
	auto &verts = m.getVertices( );
	int index = models.size( )-1;
	modelVertDirs.resize(models.size());
	modelVertAngles.resize(models.size( ));
	for(int i=0;i<m.getNumVertices() ;i++)
	{
		auto dir = (verts[i] - center);
		modelVertDirs[index].push_back(dir.getNormalized());
//		modelVertAngles[index].push_back(dir.distance(cam.getPosition()));
		modelVertAngles[index].push_back(dir.angle(ofVec3f(0,0,1)));
	}
}

void SPModelDeform::setParams(float maxMove, float offsetSpeed)
{
	MAXMOVE = maxMove;
	this->offsetSpeed = offsetSpeed;
}

void SPModelDeform::setModelIndex(int index)
{
	activeIndex = index;
}

int SPModelDeform::getModelNum()
{
	return models.size();
}

void SPModelDeform::update()
{
	if(!bIsActive)
	{
		return;
	}
	phaseOffset += ofGetLastFrameTime( );
	phaseOffset = ofWrap(phaseOffset , 0 , 360);
	moveAmount *= 0.98f;
	camDist *= 1.02f;
	camDist = ofClamp(camDist , 10 , DEFAULT_CAM_DIST);
	for(int i =0;i < models[activeIndex].getNumVertices( ) ;i++)
	{
		float phase = phaseOffset*offsetSpeed + modelVertAngles[activeIndex][i];
		float period = 0.5f;
		float amount = sin(phase*period)*moveAmount*MAXMOVE;
		models[activeIndex].getVertices( )[i] = originModels[activeIndex].getVertices( )[i]
			+ modelVertDirs[activeIndex][i] * (amount + abs(amount) / 2);
	
	}
	auto camDir=ofVec3f(cos(phaseOffset) , 0 , sin(phaseOffset));
	cam.setPosition(camDir * camDist);
	cam.lookAt(ofVec3f(0 , 0 , 0));
}

SPModelDeform::SPModelDeform( )
{
	spellName = NamePrefix + ofToString(++AMOUNT);
	cout << spellName << " constructor called " << endl;
	receiverName = spellName; 
}

SPModelDeform::~SPModelDeform( )
{
	cout << spellName << " destructor called " << --AMOUNT << endl;
}

void SPModelDeform::setCircleColor(ofColor c)
{
	circleColor = c;
}

void SPModelDeform::clearAccumMoveAmount( )
{
	accumMoveAmount = 0;
	setCircleColor(CircleColor[(int)ofRandom(COLOR_NUM)]);

}

void SPModelDeform::onTouch(InputParam p)
{
	if(!bIsActive)
	{
		return;
	}
}

void SPModelDeform::onMovement(std::vector<float>& movement)
{
	if(!bIsActive)
	{
		return;
	}
	float meanAmount = 0;
	for(auto &v : movement)
	{
		meanAmount += v;
	}
	if (movement.size() > 0) {
		meanAmount /= movement.size();
	}
	meanAmount -= 0.024f;
	meanAmount = ofClamp(meanAmount, 0, 100);
	this->moveAmount = ofClamp(
		max(moveAmount , meanAmount * 100)// meanAmount 范围是 0~1
		, 0 , 2);
	accumMoveAmount += meanAmount*2000;
	//超过后缩小回去
	if (accumMoveAmount > 600) {
		clearAccumMoveAmount();
	}
	//cout << meanAmount << endl;
}
//
//void SPModelDeform::onMovement(std::vector<float>& movement)
//{
//	if(!bIsActive)
//	{
//		return;
//	}
//	float maxMove = 0;
//	for(auto &v : movement)
//	{
//		maxMove = max(maxMove , v);
//	}
//	maxMove -= 0.04f;
//	this->moveAmount = maxMove*15;
//	cout << maxMove << endl;
//}

void SPModelDeform::customDraw()
{
	ofSetColor(circleColor);
	ofFill( );
	ofDrawEllipse(0 , 0 , accumMoveAmount*2,accumMoveAmount*3.7);
	cam.begin( );
	
		ofSetColor(255);
		models[activeIndex].drawWireframe( );

	cam.end( );
}
