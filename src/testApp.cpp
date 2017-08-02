#include "testApp.h"

static int BANDS = 128;
int keyBand = 18;
float soundPos = 0.14f;
float DebugMaxBandAmount = 0;
void testApp::setup()
{
	sound.load("main.mp3");
	sound.play( );
	sound.setPosition(soundPos);
	ballMat.init(700 , 0 , 600 , 600 , 0);
	ballMat.setup(9,keyBand);
	rain.init(1600 , 0 , 1920 , 1080 , 0);
	rain.setup(300 , keyBand);
	rain.setXflip(true);
	rainL.init(0, 0, 600, 400,0);
	rainL.setup(300, keyBand);
	dotwave.init(300 , 0 , 900 , 900 , 0);
	dotwave.setup(40 , keyBand);
}

void testApp::update()
{
	ofSoundUpdate( );
	pSpectrum = ofSoundGetSpectrum(BANDS);
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	rain.DebugSetSpeedRate(ofGetMouseX( ) / (float)ofGetWidth());
	rain.DebugSetRainPercents(ofGetMouseY( ) / (float)ofGetHeight( ));
	rain.onSoundSpectrum(pSpectrum);
	rain.update( );

	rainL.DebugSetSpeedRate(ofGetMouseX() / (float)ofGetWidth());
	rainL.DebugSetRainPercents(ofGetMouseY() / (float)ofGetHeight());
	rainL.onSoundSpectrum(pSpectrum);
	rainL.update();

	DebugMaxBandAmount = max(pSpectrum[keyBand] , DebugMaxBandAmount);
	
	ballMat.onSoundSpectrum(pSpectrum);
	ballMat.update( );

	/*dotwave.onSoundSpectrum(pSpectrum);
	dotwave.update( );*/
}

void testApp::draw( )
{
	ofClear(0);
	//rain.draw( );
	rainL.draw();
	
	//dotwave.draw( );
	ballMat.draw( );

	//声音频谱
	ofPushStyle( );
	ofPushMatrix( );

	ofSetColor(233,233,55);
	//ofRect(0 , 500 , BANDS * 5 , 100);
	ofDrawLine(0 , 700 , 600 , 700);
	ofDrawLine(0 , 600 , 600 , 600);
	ofDrawLine(0 , 500 , 600 , 500);
	ofDrawLine(0 , 400 , 600 , 400);
	ofSetColor(233 , 23 , 23);
	for(int i = 0; i<BANDS; i++)
	{
		if(i == keyBand)
		{
			ofPushStyle( );
			ofSetColor(23 , 244 , 43);
			ofRect(10 + i * 5 , 800 , 3 , -pSpectrum[i] * 100);
			ofPopStyle( );
		}
		else
		{
			ofRect(10 + i * 5 , 800 , 3 , -pSpectrum[i] * 100);
		}
	}
	ofPopMatrix( );
	ofPopStyle( );

	ofDrawBitmapString("speed: "+ofToString(ofGetMouseX( ) / (float)ofGetWidth( ))
					   , ofGetMouseX( ) , ofGetMouseY( ));
	ofDrawBitmapString("amount: "+ofToString(ofGetMouseY( ) / (float)ofGetHeight( ))
					   , ofGetMouseX( ) , ofGetMouseY( )+30);
	ofDrawBitmapString("cos Pi: " + ofToString(
		cos(ofGetMouseY( )*TWO_PI / (float)ofGetHeight( )))
					   , ofGetMouseX( ) , ofGetMouseY( ) + 60);
	ofDrawBitmapString("cos 360: " + ofToString(
		cos(ofGetMouseY( )*360 / (float)ofGetHeight( )))
					   , ofGetMouseX( ) , ofGetMouseY( ) + 90);
}

void testApp::keyPressed(int key)
{
	switch(key)
	{
		case 'f':
			ofToggleFullscreen( );
			break;
		case 'r':
			sound.setPosition(soundPos);
			break;
	}
}
