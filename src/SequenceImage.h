#pragma once
#include <ofTexture.h>

class SequenceImage
{
private:
	vector<ofTexture> sequence;
	float FPS = 24.0f;
	int currentFrame = 0;
	uint64_t startTime = 0;
	bool playing = false;
	bool playOnce = false;

public:
	void setFPS(int fps);
	void addFrame(ofTexture frame);
	void update( );
	void draw(int x , int y , int w , int h);
	void play(bool playOnce);
	void stop( );
};
