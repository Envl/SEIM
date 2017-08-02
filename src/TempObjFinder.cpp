#include "TempObjFinder.h"

void TempObjFinder::setup(ofShortPixels& pixels,ofPoint threshRange)
{
	bg.setFromPixels(pixels);
	this->threshRange = threshRange;

	contourFinder.setMinAreaRadius(9);
	contourFinder.setMaxAreaRadius(100);
	contourFinder.setThreshold(15);
	contourFinder.getTracker( ).setMaximumDistance(32);
	contourFinder.getTracker( ).setPersistence(30);
}

void TempObjFinder::updateParams(float thresh ,  int erodeIter ,int dilateIter)
{
	this->thresh = ofMap(thresh,0,1,threshRange.x,threshRange.y);
	this->erodeIter = erodeIter;
	this->dilateIter = dilateIter;
	
}

ofImage TempObjFinder::getObjImg() const
{
	return objImg;
}

void TempObjFinder::update(ofShortPixels& pixels)
{
	absdiff(bg , pixels , diff);
	diff_float.setFromPixels(diff.getPixels( ));
	threshold(diff_float , thresh);
	erode(diff_float , static_cast<int>(erodeIter));
	dilate(diff_float , static_cast<int>(dilateIter));
	objImg.setFromPixels(diff_float.getPixels());
	contourFinder.findContours(objImg);
}
