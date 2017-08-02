#pragma once
#include "ofVec3f.h"
#include "ofMath.h"

class SmoothVec3Filter
{
private:
	int size=12;
	ofVec3f smoothedVec , lastVec;
public:
	deque<ofVec3f> deque_;
	void push(ofVec3f v);
	void clear( );
	void setSize(int newSize);
	ofVec3f getVec3f( );
	SmoothVec3Filter( ) = default;
	SmoothVec3Filter(int size);
};
