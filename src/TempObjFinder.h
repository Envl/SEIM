#pragma once
#include <ofImage.h>
#include "ofxCv.h"

using namespace ofxCv;
class TempObjFinder
{
public:
	ContourFinder contourFinder;
	ofShortImage bg , diff;
	ofFloatImage diff_float; 
	ofImage objImg;
	ofPoint threshRange;
	float thresh;
	int erodeIter , dilateIter;

	void setup(ofShortPixels& pixels,ofPoint threshRange);
	void update(ofShortPixels& pixels);
	void updateParams(float thresh  ,  int erodeIter ,int dilateIter);
	ofImage getObjImg( ) const;
};
