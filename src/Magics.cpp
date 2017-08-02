#include "Magics.h"
#include <ofAppBaseWindow.h>
#include <ofAppNoWindow.h>
#include <ofCamera.h>


Magics::Magics(int x, int y)
{
	posX = x; posY = y;
}

void  Magics::Random8dirCrawl()
{
	int north = 0;
	int north_east = 1;
	int east = 2;
	int south_east = 3;
	int south = 4;
	int south_west = 5;
	int west = 6;
	int north_west = 7;

	float step_size = 1;
	int direction;


	for (int i = 0; i <= ofGetMouseX(); i++) {

		direction = (int)ofRandom(0, 8);

		if (direction == north) {

			posY -= step_size;

		}
		else if (direction == north_east) {
			posX += step_size;
			posY -= step_size;
		}
		else if (direction == east) {
			posX += step_size;
		}
		else if (direction == south_east) {
			posX += step_size;
			posY += step_size;
		}
		else if (direction == south) {
			posY += step_size;
		}
		else if (direction == south_west) {
			posX -= step_size;
			posY += step_size;
		}
		else if (direction == west) {
			posX -= step_size;
		}
		else if (direction == north_west) {
			posX -= step_size;
			posY -= step_size;
		}

		// if agent flies off screen
		if (posX > ofGetWidth()) posX = 0;
		if (posX < 0) posX = ofGetWidth();
		if (posY < 0) posY = ofGetHeight();
		if (posY > ofGetHeight()) posY = 0;


		ofSetColor(255, 0, 0);
		ofDrawEllipse(posX + step_size / 2, posY + step_size / 2, 1, 1);
	}
}


void Magics::circleLoops(int x , int y , float percents , ofColor c , int circleNum)
{
	const int INNER_R = 20;
	const int MID_R = 40;
	const int OUTTER_R = 80;

	ofSetColor(c , 60);
	ofCircle(x , y , ofMap(percents , 0.7 , 1.0f , MID_R , OUTTER_R , true));
	ofSetColor(c , 150);
	ofCircle(x , y , ofMap(percents , 0.3f , 0.7f , INNER_R , MID_R , true));
	ofSetColor(c , 250);
	ofCircle(x , y , ofMap(percents , 0 , 0.3f , 0 , INNER_R , true));

}

void Magics::ellipseLoops(int x , int y , float percents , ofColor c , int circleNum)
{
	const int INNER_R = 20;
	const int MID_R = 40;
	const int OUTTER_R = 80;

	ofSetColor(c , 60);
	auto r = ofMap(percents , 0.7 , 1.0f , MID_R , OUTTER_R , true);
	ofEllipse(x , y ,r,r/2);
	ofSetColor(c , 150);
	auto rr = ofMap(percents , 0.3f , 0.7f , INNER_R , MID_R , true);
	ofCircle(x , y ,rr,rr/2);
	ofSetColor(c , 250);
	auto rrr = ofMap(percents , 0 , 0.3f , 0 , INNER_R , true);
	ofCircle(x , y , rrr,rrr/2);

}

void Magics::begin( )
{
	ofPushMatrix( );
	ofPushStyle( );
	ofPushView( );
}

void Magics::end( )
{
	ofPopMatrix( );
	ofPopStyle( );
	ofPopView( );
}
