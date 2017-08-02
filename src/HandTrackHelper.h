#pragma once
#include "objFinder.h"
#include "PerspectiveAgent.h"
#include "SmoothVec3Filter.h"

template<typename PixelType>
class HandTrackHelper
{
private:
	PixelType roiPix;
	SmoothVec3Filter handPosFilter=SmoothVec3Filter(12);// size 
	bool lastFrameTracked = false;
	bool tracked = false;
	bool firstTracked = false;
	bool lostHand = false;
	int delayCounter = 0;
	int delta = 0;
	const int delayFrame = 10;
public:
	PerspectiveAgent perspAgent;
	ObjFinder<PixelType> handFinder;

	bool isHandEnter();
	bool isHandLeave( );
	bool isHandTracked( );
	ofPoint getHandProjPos( );
	void setupPerspectiveParams(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi);
	void setup(PixelType& pixels , ofPoint threshRange,ofRectangle outputSize
			   , string sourceName , bool findContour = true);
	void resetBG(PixelType& pixels);
	void update(ofPoint thresh , int erodeIter , int dilateIter
				, int minAreaRad , int maxAreaRad , int countourThresh
				, int contourMaxMove , int contourPersistence
				, bool doBlur , int blurSize , ofShortPixels pix);
	void resetOutputSize(ofRectangle size);
};

template <typename PixelType>
void HandTrackHelper<PixelType>::setup(PixelType& pixels, ofPoint threshRange,ofRectangle outputSize ,string sourceName, bool findContour)
{
	perspAgent.resetOutputSize(outputSize);
	cv::Mat dst;
	perspAgent.applyROI(pixels , dst);
	toOf(dst , roiPix);
	handFinder.setup(roiPix , threshRange , sourceName , findContour);
}

template <typename PixelType>
void HandTrackHelper<PixelType>::resetBG(PixelType& pixels)
{
	cv::Mat dst;
	perspAgent.applyROI(pixels , dst);
	toOf(dst , roiPix);
	handFinder.resetBG(roiPix);
}


template <typename PixelType>
bool HandTrackHelper<PixelType>::isHandEnter( )
{
	if(firstTracked)
	{
		firstTracked = false;
		delta = 1;
	}
	delayCounter += delta;
	if(delayCounter >= delayFrame)
	{
		firstTracked = true;
		delta = 0;
		delayCounter = 0;
	}
	return firstTracked;
}

template <typename PixelType>
bool HandTrackHelper<PixelType>::isHandLeave()
{
	if(lostHand)
	{
		delayCounter = 0;
		delta = 0;
	}
	return lostHand;
}

template <typename PixelType>
bool HandTrackHelper<PixelType>::isHandTracked()
{
	return tracked;
}

template <typename PixelType>
ofPoint HandTrackHelper<PixelType>::getHandProjPos( )
{
	return perspAgent.transCoord(handPosFilter.getVec3f( ));
}

template <typename PixelType>
void HandTrackHelper<PixelType>::setupPerspectiveParams(ofPoint A1, ofPoint A2, ofPoint B1, ofPoint B2, ofRectangle roi)
{
	perspAgent.setup(A1 , A2 , B1 , B2 , roi);
}

template <typename PixelType>
void HandTrackHelper<PixelType>::update(ofPoint thresh , int erodeIter , int dilateIter , int minAreaRad , int maxAreaRad , int countourThresh , int contourMaxMove , int contourPersistence , bool doBlur , int blurSize , ofShortPixels pix)
{
	handFinder.updateParams(thresh , erodeIter , dilateIter , minAreaRad ,    // 1ms
							maxAreaRad , countourThresh , contourMaxMove , contourPersistence
							, doBlur , blurSize);
	cv::Mat dst;
	perspAgent.applyROI(pix , dst);
	toOf(dst , roiPix);
	handFinder.update(roiPix);
	lastFrameTracked = tracked;
	if(handFinder.checkHandTracked( ))
	{
		auto handPos = handFinder.getHandPos( );//手在截取后的图像中位置
		handPosFilter.push(handPos);
		tracked = true;
		//手 首次被 track 到
		firstTracked = !lastFrameTracked;
	}
	else
	{
		// 上一帧 track 到 代表从这一帧开始 手离开了
		lostHand = tracked;
		tracked = false;
		handPosFilter.clear( );
	}
}

template<typename PixelType>
inline void HandTrackHelper<PixelType>::resetOutputSize(ofRectangle size)
{
	perspAgent.resetOutputSize(size);
}

