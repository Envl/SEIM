#include "ManDetectHelper.h"




void FollowRing::setup(const cv::Rect& track)
{
	smoothedPos = toOf(track).getCenter( );
}


void FollowRing::update(const cv::Rect& track)
{
	auto t = toOf(track);
	float newR = t.getWidth( ) > t.getHeight( ) ? t.getWidth( ) / 2 : t.getHeight( ) / 2;
	smoothedPos.interpolate(t.getCenter() , 0.5);
	radius = ofLerp(radius , newR , 0.6f);
}

void FollowRing::kill()
{
	float curTime = ofGetElapsedTimef( );
	if(startDieTime == 0)
	{
		startDieTime = curTime;
	}
	else if(curTime - startDieTime > DIE_DURATION)
	{
		dead = true;
	}
}

void FollowRing::draw(int x,int y,int z,ofColor c)
{
	ofSetColor(c);
	ofCircle(x,y , radius+z);
	//ofDrawRectangle(x + (radius + z) / 2
					//, y + (radius + z) / 2
					//, radius + z , (radius + z)*2/3);
}

void FollowRing::draw(ofVec3f pos,ofColor c)
{
	draw(pos.x , pos.y , pos.z,c);
}

ofVec2f FollowRing::Pos() 
{
	return smoothedPos;
}

