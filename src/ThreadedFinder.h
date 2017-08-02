#pragma once
#include <string>
#include <ofPoint.h>
#include "ObjFinder.h"

/*       暂且有很多问题      */
template<typename PixelType>
class ThreadedFinder:public ofThread
{
public:
	void start( );
	void stop( );
	void threadedFunction( ) override;

	ObjFinder<PixelType>* get( ) ;
	bool tracked( );
	void update(PixelType pixels );// 得传复制进来, 否则是同一块内存.
	void updateParams(ofPoint thresh , int erodeIter , int dilateIter
					  , int minAreaRad , int maxAreaRad , int countourThresh
					  , int contourMaxMove , int contourPersistence
					  , bool doBlur , int blurSize);
	void setup(PixelType pixel, ofPoint threshRange, std::string sourceName
	           , bool findContour=true);
private:
	ObjFinder<PixelType> finder;
	bool bTracked = false;
	bool isFrameNew = false;
	PixelType pixels_;
};

template <typename PixelType>
bool ThreadedFinder<PixelType>::tracked()
{
	unique_lock<std::mutex> lock(mutex);
	return bTracked;
}

template <typename PixelType>
void ThreadedFinder<PixelType>::start()
{
	startThread( );
}

template <typename PixelType>
void ThreadedFinder<PixelType>::stop()
{
	stopThread( );
}

template <typename PixelType>
void ThreadedFinder<PixelType>::threadedFunction()
{
	while(isThreadRunning())
	{
		if(lock())
		{
			if(isFrameNew)
			{
				isFrameNew = false;
				finder.update(pixels_);
				if(finder.checkHandTracked())
				{
					bTracked = true;
				}
			}
			unlock( );
		}
	}
}

template <typename PixelType>
ObjFinder<PixelType>* ThreadedFinder<PixelType>::get() 
{
	unique_lock<std::mutex> lock(mutex);
	return &finder;
}

template <typename PixelType>
void ThreadedFinder<PixelType>::update(PixelType pixels)
{
	if(lock( ))
	{
		pixels_ = pixels;
		isFrameNew = true;
		unlock( );
	}
}

template <typename PixelType>
void ThreadedFinder<PixelType>::updateParams(ofPoint thresh, int erodeIter, int dilateIter, int minAreaRad, int maxAreaRad, int countourThresh, int contourMaxMove, int contourPersistence, bool doBlur, int blurSize)
{
	if(lock( ))
	{
		finder.updateParams(thresh , erodeIter , dilateIter
							, minAreaRad , maxAreaRad , countourThresh
							, contourMaxMove , contourPersistence
							, doBlur , blurSize);
		unlock( );
	}
}

template <typename PixelType>
void ThreadedFinder<PixelType>::setup(PixelType pixel, ofPoint threshRange, std::string sourceName, bool findContour)
{
	if(lock( ))
	{
		pixels_ = pixel;
		finder.setup(pixel , threshRange , sourceName , findContour);
		unlock( );
	}
}
