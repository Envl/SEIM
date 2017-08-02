#include "ofMain.h"
#include "ofApp.h"
#include "CheckInApp.h"
#include "AisleApp.h"
#include "MainApp.h"
#include "testApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.width = GLOBALS::WindowW+1024;
	settings.height = GLOBALS::WindowH;
	settings.setGLVersion(3, 3);// 最大支持3.3 貌似
	settings.multiMonitorFullScreen = true;
	ofCreateWindow(settings);

	cout << "0 ofApp contains all" << endl
		<< "1 CheckInApp" << endl
		<< "2 AisleApp" << endl
		<< "3 MainApp" << endl
		<< "4 testApp" << endl;
	char c='8';
	while(c - '0' > 4 || c - '0' < 0)
	{
		c = getchar( );
	}
	switch(c)
	{
		case '0':
			ofRunApp(new ofApp( ));
			break;
		case '1':
			ofRunApp(new CheckInApp( ));
			break;
		case '2':
			ofRunApp(new AisleApp( ));
			break;
		case '3':
			ofRunApp(new MainApp( ));
			break;
		case '4':
			ofRunApp(new testApp( ));
			break;
	}
}
