#include "VideoFrameHelper.h"


void VideoFrameHelper::setup(int FPS, float duration,int defer)
{
	fps = FPS;
	this->duration = duration;
}

void VideoFrameHelper::start()
{
	startTime = ofGetElapsedTimef( );
}

int VideoFrameHelper::getCurrentFrame()
{
	return currentFrame;
}

int VideoFrameHelper::calcCurrentFrame(float pos)
{
	update(pos);
	return currentFrame;
}

void VideoFrameHelper::update(float timePosition)
{
	currentFrame = ofClamp(timePosition*fps - frameDefer
						   , 0 , duration*fps - 5);
}

void VideoFrameHelper::update()
{
	currentFrame = ofClamp((ofGetElapsedTimef( ) - startTime)*fps-frameDefer
						   , 0 , duration*fps - 5);//  duration * FPS 计算出来可能比实际上 frame 要多.
}
