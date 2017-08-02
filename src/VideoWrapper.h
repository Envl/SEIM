#pragma once
#include "ofxWMFVideoPlayer.h"

class VideoWrapper
{
public:
	VideoWrapper( )=default;
	~VideoWrapper( )=default;
	void Activate( );
	void Deactivate( );
	ofxWMFVideoPlayer player;
	void load(string path);
	void draw(int x , int y , int w , int h);
	void update( );
	void setHideWhenFinish( bool flag);
private:
	bool bIsActive = true;
	bool bHideWhenFinish = true;
	bool bFirst = true;
};


