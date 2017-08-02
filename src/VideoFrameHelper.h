#pragma once
#include "ofMain.h"
class VideoFrameHelper
{
private:
	int fps;
	float duration; // unit  second
	float startTime;
	int currentFrame;
	int frameDefer;// load each frame to GPU needs time,  defer some frame to wait
public:
	VideoFrameHelper():fps(1),duration(0),startTime(0),currentFrame(0),frameDefer(0)
	{ }
	void setup(int FPS , float duration,int defer);
	void start( ); 
	int getCurrentFrame( );
	int calcCurrentFrame(float timePos);
	void update(float timePosition );
	void update( );
};
