#include "SequenceImage.h"

void SequenceImage::setFPS(int fps)
{
	FPS = fps;
}

void SequenceImage::addFrame(ofTexture frame)
{
	sequence.push_back(frame);
}

void SequenceImage::update( )
{
	if(playing)
	{
		// play once then remain index at the last frame
		if(playOnce &&( currentFrame >=(int) (sequence.size( ) - 1)))
			return;

		currentFrame = (ofGetElapsedTimeMillis( ) - startTime)*FPS / 1000.0f;
		if(playOnce)
		{
			currentFrame = currentFrame > sequence.size( ) - 1 ? sequence.size( ) - 1 : currentFrame;
		}
		else
		{
			currentFrame %= sequence.size( );
		}
	}
}

void SequenceImage::play(bool playOnce)
{
	this->playOnce = playOnce;
	playing = true;
	startTime = ofGetElapsedTimeMillis( );
	currentFrame = 0;
}

void SequenceImage::stop()
{
	playing = false;
}

void SequenceImage::draw(int x , int y , int w , int h)
{
	if(playing)
	{
		sequence[currentFrame].draw(x , y , w , h);
	}
}
