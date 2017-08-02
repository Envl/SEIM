#pragma once
#include "ofMain.h"

class Magics
{
public:
	Magics(){}
	~Magics(){}
	 Magics(int x,int y);
	 int posX, posY;//这两个是给下面这个函数用的
	 void Random8dirCrawl();
	 void circleLoops(int x , int y , float percents , ofColor c =ofColor(255,255,255), int circleNum = 3); // percent 0~1
	 void ellipseLoops(int x , int y , float percents , ofColor c =ofColor(255,255,255), int circleNum = 3); // percent 0~1
	 void begin( );
	 void end( );
};
