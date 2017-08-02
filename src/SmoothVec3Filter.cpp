#include "SmoothVec3Filter.h"

void SmoothVec3Filter::push(ofVec3f v)
{
	while(deque_.size()>size)
	{
		deque_.pop_front( );
	}
	deque_.push_back(v);
	ofVec3f meanVec;
	for(int i=0;i<deque_.size();i++)
	{
		meanVec += deque_[i];
	}
	meanVec /= deque_.size( );//取平均位置
//	lastVec = smoothedVec;
	smoothedVec = meanVec;
}

void SmoothVec3Filter::clear()
{
	deque_.clear( );
}

void SmoothVec3Filter::setSize(int newSize)
{
	size = newSize;
}

ofVec3f SmoothVec3Filter::getVec3f()
{
	return smoothedVec;
}

SmoothVec3Filter::SmoothVec3Filter(int size)
{
	this->size = size;
}
