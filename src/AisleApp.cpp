#include "AisleApp.h"

void AisleApp::setup( )
{
	ofDisableArbTex( );
	screen.setup(1024 , 768 , 40 , 30);
	manDetectHelper.setUseTracker(false);
	// GUI
	{
		gui.setup( );
		gui.setPosition(0 , 0);
			gui.add(lineW.set("line width" ,1, 1 , 6));
			gui.add(bgColor.set("BG color" , ofColor(0)));
			gui.add(doBlur.set("do blur" , false));
			gui.add(blurSize.set("blur size " , blurSize , 1 , 10));

		manFinderParams.setName("Man Finder Params");
			manFinderParams.add(imgXFlipped.set("X Flipped" , imgXFlipped));
			manFinderParams.add(imgYFlipped.set("Y Flipped" , imgYFlipped));
			manFinderParams.add(ROI.set("ROI" , ROI));
			manFinderParams.add(bgRecorded.set("bg recorded" , false));
			manFinderParams.add(thresh.set("ExtractRange" , thresh , ofVec3f(0 , 0 , 1) , ofVec3f(1 , 1 , 100)));
			manFinderParams.add(dilateIter.set("dilate Iter" , dilateIter , 0 , 15));
			manFinderParams.add(erodeIter.set("erode Iter" , erodeIter , 0 , 20));
			manFinderParams.add(finderMinRad.set("finder Min Rad" , finderMinRad , 1 , 50));
			manFinderParams.add(finderMaxRad.set("finder Max Rad" , finderMaxRad , 50 , 300));
			manFinderParams.add(finderThresh.set("finder Thresh" , finderThresh , 1 , 80));
			manFinderParams.add(finderMaxMove.set("finder Max Move" , finderMaxMove , 1 , 100));
		gui.add(manFinderParams);

		projectionParams.setName("Aisle Project Params");
			projectionParams.add(screenPos.set("Screen pos" , screenPos , ofVec2f(0) , ofVec2f(1920 , 1080)));
			projectionParams.add(outputSize.set("Projector Size" , outputSize
												, ofVec2f(1024 , 768) , ofVec2f(1920 , 1080)));
			projectionParams.add(screenUL.set("Screen UL" , screenUL));
			projectionParams.add(screenUR.set("Screen UR" , screenUR));
			projectionParams.add(screenDL.set("Screen DL" , screenDL));
			projectionParams.add(screenDR.set("Screen DR" , screenDR));
			projectionParams.add(A1.set("A1" , A1));
			projectionParams.add(A2.set("A2" , A2));
			projectionParams.add(B1.set("B1" , B1));
			projectionParams.add(B2.set("B2" , B2));
		gui.add(projectionParams);
	}
	// Params
	bDrawDebug = false;
	ofSetCircleResolution(25);
	ofDisableArbTex( );
	// Kinect
	kinect_.open( );
	kinect_.initDepthSource( );
	kinect_.initInfraredSource( );
	ir_ = kinect_.getInfraredSource( );
	depth_ = kinect_.getDepthSource( );
	// Spell
	auto wave0 = new SPEnterWave( );
	auto wave1 = new SPEnterWave( );
	auto wave2 = new SPEnterWave( );
	wave0->init(0 , 0 , 427 , PROJECTOR_H,90);
	wave1->init(240 , 0 , 800 , PROJECTOR_H,90);
	wave2->init(854, 0 , 427 , PROJECTOR_H,90);
	wave0->setup(PROJECTOR_H , 427);
	wave1->setup(PROJECTOR_H , 427);
	wave2->setup(PROJECTOR_H , 427);
	SpellManager::Instance( ).addSpell(wave0);
	SpellManager::Instance( ).addSpell(wave1);
	SpellManager::Instance( ).addSpell(wave2);
	InputManager::Instance( ).addReceiver(wave0);
	InputManager::Instance( ).addReceiver(wave1);
	InputManager::Instance( ).addReceiver(wave2);

}

void AisleApp::update( )
{
	InputManager::Instance( ).update( );
	// Kinect
	kinect_.update( );
	/*
	这里 110 frame 后比较安全, 否则可能 color 的 pixel 只
	有 3 个 channel ... 原因不明, 应该是 addon 的问题....
	*/
	if(kinect_.isFrameNew( ) && ofGetFrameNum( ) > 30)
	{
		manDetectHelper.updatePerspectiveParams(A1 , A2 , B1 , B2
												, ofRectangle(ROI.get( ).x*GLOBALS::OtherW
															  , ROI.get( ).y*GLOBALS::OtherH
															  , (ROI.get( ).z - ROI.get( ).x)*GLOBALS::OtherW
															  , (ROI.get( ).w - ROI.get( ).y)*GLOBALS::OtherH));
		if(!bgRecorded)
		{
			manDetectHelper.setupPerspectiveParams(A1 , A2 , B1 , B2 ,
												   ofRectangle(ROI.get( ).x*GLOBALS::OtherW
															   , ROI.get( ).y*GLOBALS::OtherH
															   , (ROI.get( ).z - ROI.get( ).x)*GLOBALS::OtherW
															   , (ROI.get( ).w - ROI.get( ).y)*GLOBALS::OtherH));
			manDetectHelper.setup(depth_->getPixels( ) , DEPTH_THRESH_RANGE
								  , ofRectangle(0 , 0 , 1280 , 1024) , depth_->getTypeName( ));
			bgRecorded = true;
			printf("%s" , "recorded BACKGROUND Image");
		}
		else
		{
			manDetectHelper.update(thresh , erodeIter , dilateIter , finderMinRad , finderMaxRad
								   , finderThresh , finderMaxMove , 30 , doBlur , blurSize , depth_->getPixels( ));
		}
	}
	for(int i = 0; i < manDetectHelper.getManPosition_Radius_s( ).size(); i++)
	{
		auto pos = manDetectHelper.getManPosition_Radius_s( )[i];
		InputManager::Instance( ).sendTouch(
			pos.x ,ofWrap(pos.y * 2 , 0 , screen.getScreenHeight( ))
		);
	}
	//InputManager		::Instance( ).sendTouches(manDetectHelper.getManPosition_Radius_s( ));
	//InputManager::Instance( ).sendTouch(ofGetMouseX( ) , ofGetMouseY( ));

	SpellManager::Instance( ).batchUpdate( );//这个放在最后一行,  中间可能会有 input 事件产生
	//--------- 所有 Update 完后， 绘图到 FBO -----------
	screen.begin( );
	ofPushStyle( );
	ofSetColor(bgColor);
	if(bCheckInfo)
	{
		SpellManager::Instance( ).batchDraw( );
	}
	ofPopStyle( );
	//脚下圆圈
	{
		auto mans = manDetectHelper.getManPosition_Radius_s( );
		magics_.begin( );
		for(int i = 0; i < mans.size( ); i++)
		{
			//ofDrawEllipse(mans[i].x , mans[i].y , 0 , 40 , 20);
			magics_.ellipseLoops( mans[i].x,mans[i].y,ofWrap(ofGetElapsedTimef(),0,1));
			//ofDrawCircle(mans[i].x , mans[i].y , 30);
		}
		magics_.end( );
	}
	screen.end( );
	screen.setScreenPos(screenPos.get( ));
}

void AisleApp::draw( )
{
	screen.draw();
	if(bDrawDebug)
	{
		screen.drawDebug( );
	}
	if(bCheckInfo)
	{
		ofDrawBitmapString(ofToString(ofGetFrameRate( )) , 10 , 20);
	}
	
	// Speed Mode 不画以下所有内容
	if(isSpeedMode)
		return;
	{
		if(ir_)
		{
			ir_->draw(GLOBALS::OtherW * 2 , GLOBALS::OtherH);
			ofDrawBitmapString(" IR Image " , GLOBALS::OtherW * 2 , GLOBALS::OtherH + 20);
		}
		if(depth_)
		{
			depth_->draw(GLOBALS::OtherW , GLOBALS::OtherH);
			ofDrawBitmapString(" Depth Image " , GLOBALS::OtherW , GLOBALS::OtherH + 20);
		}
		if(manDetectHelper.Finder( ).getObjImage( )->isAllocated( ))
		{
			manDetectHelper.Finder( ).getObjImage( )->draw(GLOBALS::OtherW , 0);
			manDetectHelper.Finder( ).getFinder( ).draw( );
		}
	}
	//画出 投影范围 的 A1 A2 B1 B2
	{
		ofPushMatrix( );
		ofPushStyle( );
			ofFill( );
			ofTranslate(DEP_IMG_POS + manDetectHelper.getPerspective().getROI( ).getPosition( ));
			auto aabb = manDetectHelper.getPerspective().getAABB( );// aabb 长度只有 4 
			ofSetColor(233 , 23 , 22);
			ofLine(aabb[0] , aabb[1]);
			ofLine(aabb[0] , aabb[2]);
			ofLine(aabb[1] , aabb[3]);
			ofLine(aabb[2] , aabb[3]);
		
			ofSetColor(233 , 23 , 22);
			ofCircle(aabb[0] , 4);
		
			ofSetColor(23 , 233 , 22);
			ofCircle(aabb[1] , 4);
		
			ofSetColor(23 , 23 , 232);
			ofCircle(aabb[2] , 4);
		
			ofSetColor(23 , 223 , 222);
			ofCircle(aabb[3] , 4);
	
			ofSetColor(233);
			ofDrawBitmapString("A1: " + ofToString(aabb[0]) , aabb[0] + ofPoint(0 , -20));
			ofDrawBitmapString("A2: " + ofToString(aabb[1]) , aabb[1] + ofPoint(0 , -20));
			ofDrawBitmapString("B1: " + ofToString(aabb[2]) , aabb[2] + ofPoint(0 , 30));
			ofDrawBitmapString("B2: " + ofToString(aabb[3]) , aabb[3] + ofPoint(0 , 30));
		ofPopMatrix( );
		ofPopStyle( );
		
		// 在 source 上面画出 ROI
		ofPushMatrix( );
		ofPushStyle( ); 
			ofNoFill( );
			ofSetColor(222 , 222 , 22);
			ofTranslate(DEP_IMG_POS);
			ofDrawRectangle(ROI.get( ).x*GLOBALS::OtherW
							, ROI.get( ).y*GLOBALS::OtherH
							, (ROI.get( ).z - ROI.get( ).x)*GLOBALS::OtherW
							, (ROI.get( ).w - ROI.get( ).y)*GLOBALS::OtherH);
		ofPopMatrix( );
		ofPopStyle( );
	}
	if(drawGui)
	{
		gui.draw( );
	}
}
void AisleApp::exit( )
{
	EasyFunction::SafeCloseKinect(kinect_);
}

void AisleApp::keyPressed(int key)
{
	switch(key)
	{
		case VK_SPACE:
			//ofSetBackgroundColor(bgColor);
			if(ir_)
			{
				//manDetectHelper.resetBG(ir_->getPixels( ));
			}
			if(depth_)
			{
				manDetectHelper.resetBG(depth_->getPixels( ));
			}
			break;
		case OF_KEY_UP:
			dragRad++;
			std::cout << "drag rad " << dragRad << endl;
			break;
		case OF_KEY_DOWN:
			dragRad--;
			std::cout << "drag rad " << dragRad << endl;
			break;
		case 'a':
			ofSetBackgroundColor(0);
			manDetectHelper.getPerspective( ).setXFlipped(imgXFlipped);
			manDetectHelper.getPerspective( ).setYFlipped(imgYFlipped);
			break;
		case 'd':
			bDrawDebug = !bDrawDebug;
			break;
		case 'f':
			ofToggleFullscreen( );
			break;
		case 'g':
			drawGui = !drawGui;
			break;
		case 'i':
			bCheckInfo = !bCheckInfo;
			break;
		case 'l':
			gui.loadFromFile("settings.xml");
			screen.loadAABB(screenUL,screenUR,screenDL,screenDR );
			cout << "GUI loaded from settings.xml" << endl;
			break;
		case 'm':
			break;
		case 'p':
			break;
		case 'r':
			screen.resetOutputSize(outputSize.get( ).x , outputSize.get( ).y);
			manDetectHelper.resetOutputSize(
				ofRectangle(0 , 0 , outputSize.get( ).x , outputSize.get( ).y)
			);
			break;
		case 's':
			isSpeedMode = !isSpeedMode;
			break;
		case 'v':
			break;
		case 'w':
			break;
		case '1':
		case '2':
		case '3':
			break;
		case '5':
			A1.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y));
			break;
		case '6':
				A2.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); 
			break;
		case '7':
				B1.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); 
			break;
		case '8': 
				B2.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); 
			break;
	}
}

void AisleApp::keyReleased(int key)
{
}

void AisleApp::mouseMoved(int x , int y)
{
}

void AisleApp::mouseDragged(int x , int y , int button)
{
	screen.onMouseDragged(x , y , button);
}

void AisleApp::mousePressed(int x , int y , int button)
{
	screen.onMouseDown(x , y , dragRad , button);
}

void AisleApp::mouseReleased(int x , int y , int button)
{
	screen.onMouseUp(x , y , button);
	//更新屏幕顶点信息
	screenUL = screen.getScreenUL( );
	screenUR = screen.getScreenUR( );
	screenDL = screen.getScreenDL( );
	screenDR = screen.getScreenDR( );
}


void AisleApp::mouseScrolled(int x , int y , float scrollX , float scrollY)
{
}
