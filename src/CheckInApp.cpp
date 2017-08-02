#include "CheckInApp.h"

void CheckInApp::setup()
{
	ofDisableArbTex( );
	screen.setup(1024 , 768,10,16);

	// Resources
	stampTip.load("tip.png");
	yhFont.load("msyh.ttc" , 66);
	for(int i = 0; i < LOGO_VIDEO_NUM; i++)
	{
		char name[30];
		sprintf(name , "%s%d.avi" , "stamp_video/" , i);
		logoVideos[i] = new VideoWrapper();
		logoVideos[i]->load(name);
		logoVideos[i]->player.setLoop(false);
		logoVideos[i]->setHideWhenFinish(true);

	}
	cout << "time: "<<ofGetElapsedTimef ()<<" loading images..."<< endl;
	ofxHelper::LoadSequence(logoSequence , "Stamp/1200_" , 5);//测试时只读取 5 帧提高启动速度
	cout << ofGetElapsedTimef( ) << std::endl;
	//　GUI　
	{
		gui.setup( );
		gui.setPosition(0 , 0);
			gui.add(bgColor.set("BG color" , ofColor(0)));
			gui.add(seimGifFPS.set("SEIM gif FPS" , seimGifFPS , 1 , 60));
			gui.add(seimOffSet.set("Seim drawing Offset" , seimOffSet
								   , ofVec3f(-300 , -300 , 0) , ofVec3f(300 , 300 , 50)));
			gui.add(isPlaySeimGifOnce.set("Play SEIM gif Once" , isPlaySeimGifOnce));
			gui.add(blurSize.set("blur size " , blurSize , 1 , 10));

		handFinderParams.setName("Hand Finder Params");
			handFinderParams.add(imgXFlipped.set("kinect X Flipped" , imgXFlipped));
			handFinderParams.add(imgYFlipped.set("kinect Y Flipped" , imgYFlipped));
			handFinderParams.add(screenXFlip.set("screen Y Flipped" , screenXFlip));
			handFinderParams.add(screenYFlip.set("screen Y Flipped" , screenYFlip));
			handFinderParams.add(handROI.set("Hand ROI" , handROI));
			handFinderParams.add(thresh.set("ExtractRange" , thresh , ofVec3f(0 , 0 , 1) , ofVec3f(1 , 1 , 100)));
			handFinderParams.add(dilateIter.set("dilate Iter" , dilateIter , 0 , 15));
			handFinderParams.add(erodeIter.set("erode Iter" , erodeIter , 0 , 20));
			handFinderParams.add(finderMinRad.set("finder Min Rad" , finderMinRad , 1 , 50));
			handFinderParams.add(finderMaxRad.set("finder Max Rad" , finderMaxRad , 2 , 50));
			handFinderParams.add(finderThresh.set("finder Thresh" , finderThresh , 1 , 80));
			handFinderParams.add(finderMaxMove.set("finder Max Move" , finderMaxMove , 1 , 100));
		gui.add(handFinderParams);
		
		projectionParams.setName("HandProject Params");
			projectionParams.add(screenPos.set("Screen pos" , screenPos , ofVec2f(0) , ofVec2f(1920 , 1080))); 
			projectionParams.add(stampSize.set("stamp size " , stampSize , 1 , 10));
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
	// parameters
	{
		bDrawDebug = false; // debug 绘图
		ofSetFrameRate(GLOBALS::FrameRate);
		ofBackground(55);
		ofSetCircleResolution(30);// 对于投影精度无需太高
	}
	// Kinect Stuffs
	{
		kinect_.open( );
		kinect_.initDepthSource( );
		kinect_.initInfraredSource( );
		ir_ = kinect_.getInfraredSource( );
		depth_ = kinect_.getDepthSource( );
	}
}

void CheckInApp::update()
{
	// 常规 update
	logoSequence.setFPS(seimGifFPS);
	// Kinect related
	kinect_.update( );
	/*
	这里 110 frame 后比较安全, 否则可能 color 的 pixel 只
	有 3 个 channel ... 原因不明, 应该是 addon 的问题....
	*/
	if(kinect_.isFrameNew( ) && ofGetFrameNum( ) > 110)
	{
		handTrackHelper.perspAgent.updateParams(A1 , A2 , B1 , B2
												, ofRectangle(handROI.get( ).x*GLOBALS::OtherW
															  , handROI.get( ).y*GLOBALS::OtherH
															  , (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
															  , (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH));
		if(!bgRecorded)
		{
			handTrackHelper.setup(depth_->getPixels( ) , DEPTH_THRESH_RANGE
								  , ofRectangle(0 , 0 , 1024 , 768) , depth_->getTypeName( ));
			bgRecorded = true;
			printf("%s" , "recorded BACKGROUND Image");
			gui.loadFromFile("settings.xml");
			screen.loadAABB(screenUL , screenUR , screenDL , screenDR);
			cout << "GUI loaded from settings.xml" << endl;
			ofSetBackgroundColor(bgColor);
			handTrackHelper.perspAgent.setXFlipped(imgXFlipped);
			handTrackHelper.perspAgent.setYFlipped(imgYFlipped);
			screen.setXflip(screenXFlip);
			screen.setYflip(screenYFlip);
			handTrackHelper.resetBG(depth_->getPixels( ));
		}
		else
		{
			handTrackHelper.update(thresh , erodeIter , dilateIter , finderMinRad , finderMaxRad
								   , finderThresh , finderMaxMove , 30 , doBlur , blurSize , depth_->getPixels( ));
			bNoHand = false;
			if(handTrackHelper.isHandEnter( ))
			{
				while(logoIndex == lastLogo)
				{
					logoIndex = (int)ofRandom(LOGO_VIDEO_NUM);
				}
				logoVideos[logoIndex]->player.play( );
				lastLogo = logoIndex;
				//logoSequence.play(true);
			}
			else if(handTrackHelper.isHandTracked( ))
			{
				logoVideos[logoIndex]->update( );
				//logoSequence.update( );
			}
			else if(handTrackHelper.isHandLeave( ))
			{
				logoVideos[logoIndex]->player.stop( );
				//logoSequence.stop( );
			}
			else// 没有追踪到手的时候
			{
				bNoHand = true;
			}
		}
		screen.begin( );
		//没追踪到手时 绘图
		if(bNoHand)
		{
			ofSetColor(0  , 111,255);
			scanLinePos.x += 666*ofGetLastFrameTime();
			scanLinePos.x = ofWrap(scanLinePos.x , 50 , screen.getScreenWidth( )-50);
 			ofDrawRectangle(scanLinePos.x,200 , 3 , screen.getScreenHeight( ));
			ofPushMatrix( );
			ofRotate(90);
			stampTip.draw(-screen.getScreenHeight( ) , 0
						  , screen.getScreenHeight( ) - 200
						  , (screen.getScreenHeight( ) - 200)*stampTip.getHeight( ) / stampTip.getWidth( ));
			ofPopMatrix( );
		}
		if(!handTrackHelper.isHandTracked( ))
			{
				ofPushMatrix( );
				ofPushStyle( );
				ofSetColor(255);
				ofRotate(-90);
				stampTip.draw(-screen.getScreenHeight( ) , 0
							  , screen.getScreenHeight( ) - 200
							  , (screen.getScreenHeight( ) - 200)*stampTip.getHeight( ) / stampTip.getWidth( ));
				ofPopStyle( );
				ofPopMatrix( );
		screen.end( );
				return;
			}
			auto handProjectPos = handTrackHelper.getHandProjPos( );
			/*logoSequence.draw(handProjectPos.x - 15 * stampSize + seimOffSet.get( ).x
								, handProjectPos.y - 15 * stampSize + seimOffSet.get( ).y
								, 30 * stampSize + seimOffSet.get( ).z
								, 30 * stampSize + seimOffSet.get( ).z);*/
			logoVideos[logoIndex]->draw(handProjectPos.x - 15 * stampSize + seimOffSet.get( ).x
									   , handProjectPos.y - 15 * stampSize + seimOffSet.get( ).y
									   , 30 * stampSize + seimOffSet.get( ).z
									   , 30 * stampSize + seimOffSet.get( ).z);
			if(bDrawDebug)
			{
				auto handProjectPos = handTrackHelper.getHandProjPos( );
				ofDrawBitmapString("warped pos: " + ofToString(handProjectPos)
									, handProjectPos + ofPoint(0 , 20));
				ofDrawCircle(handProjectPos+seimOffSet, 30);
			}
		screen.end( );
	}
	screen.setScreenPos(screenPos.get());
}

void CheckInApp::draw()
{
//	if(handTrackHelper.handFinder.checkHandTracked( ))
//	{
//		ofPushMatrix( );
//		ofTranslate(ofGetScreenWidth( ) , 0);
//		auto handProjectPos = handTrackHelper.getHandProjPos( );
//		logoSequence.draw(handProjectPos.x - 15 * stampSize + seimOffSet.get( ).x
//						  , handProjectPos.y - 15 * stampSize + seimOffSet.get( ).y
//						  , 30 * stampSize + seimOffSet.get( ).z
//						  , 30 * stampSize + seimOffSet.get( ).z);
//		ofPopMatrix( );
//	}
	screen.draw();

	if(bDrawDebug)
	{
		screen.drawDebug();
	}
	if(bCheckInfo)
	{
		ofDrawBitmapString(ofToString(ofGetFrameRate( )) , 10 , 20);
	}

	// Speed Mode 不画以下所有内容
	if(isSpeedMode)
		return;
	if(ir_)
	{
		ir_->draw(IR_IMG_POS.x,IR_IMG_POS.y);
		ofDrawBitmapString(" IR Image " ,IR_IMG_POS.x, IR_IMG_POS.y+ 20);
	}
	if(depth_)
	{
		depth_->draw(DEP_IMG_POS.x , DEP_IMG_POS.y);
		ofDrawBitmapString(" Depth Image " , DEP_IMG_POS.x , DEP_IMG_POS.y + 20);
	}
	if(handTrackHelper.handFinder.getObjImage()->isAllocated())
	{
		handTrackHelper.handFinder.getObjImage( )->draw(GLOBALS::OtherW , 0);
		handTrackHelper.handFinder.getFinder( ).draw( );
	}
	if(handTrackHelper.isHandTracked())
	{
		ofPushStyle( );
		ofNoFill( );
		ofSetLineWidth(3);
		ofSetColor(22 , 233 , 22);
		handTrackHelper.handFinder.getHandFitQuad( ).draw( );
		ofPopStyle( );
	}
	/////////--------------- 下面是 观察信息 绘图------------///////////////
	ofPushStyle( );
	ofPushMatrix( );
	ofNoFill( );
	//画出 hand投影范围 的 A1 A2 B1 B2
	{
		ofPushMatrix( );
		ofPushStyle( );
		ofFill( );
		ofTranslate(DEP_IMG_POS + handTrackHelper.perspAgent.getROI( ).getPosition( ));
		auto aabb = handTrackHelper.perspAgent.getAABB( );// aabb 长度只有 4 
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
		ofPopStyle( );
		ofPopMatrix( );
	}

	// 在 source 上面画出 ROI
	ofSetColor(222 , 222 , 22);
	ofTranslate(DEP_IMG_POS);
	ofDrawRectangle(handROI.get( ).x*GLOBALS::OtherW
					, handROI.get( ).y*GLOBALS::OtherH
					, (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
					, (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH);

	ofPopMatrix( );
	ofPopStyle( );
	if(drawGui)
	{
		gui.draw( );
	}
}

void CheckInApp::exit()
{
	EasyFunction::SafeCloseKinect(kinect_);
	yhFont.unloadFont( );
	for(int i=0;i<LOGO_VIDEO_NUM;i++)
	{
		logoVideos[i]->player.close( );
	}
}

void CheckInApp::windowResized(int w, int h)
{
}

void CheckInApp::keyPressed(int key)
{
	switch(key)
	{
		case VK_SPACE:
			if(depth_)
			{
				handTrackHelper.resetBG(depth_->getPixels( ));
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
			ofSetBackgroundColor(bgColor);
			handTrackHelper.perspAgent.setXFlipped(imgXFlipped);
			handTrackHelper.perspAgent.setYFlipped(imgYFlipped); 
			screen.setXflip(screenXFlip);
			screen.setYflip(screenYFlip);
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
			screen.loadAABB(screenUL , screenUR , screenDL , screenDR);
			cout << "GUI loaded from settings.xml" << endl;
			break;
		case 'm':   
			break;
		case 'p':
			break;
		case 'r':
			screen.resetOutputSize(outputSize.get( ).x , outputSize.get( ).y);
			handTrackHelper.resetOutputSize(
				ofRectangle(0,0,outputSize.get().x,outputSize.get().y)
			);
			break;
		case 's':
			isSpeedMode = !isSpeedMode;
			break;
		case 'v':
			break;
		case 'w':
			drawWireFrame = !drawWireFrame;
			break;
		case '1':
		case '2':
		case '3':
			break;
		case '5': A1.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
		case '6': A2.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
		case '7': B1.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
		case '8': B2.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
	}
}

void CheckInApp::keyReleased(int key)
{

}

void CheckInApp::mouseMoved(int x, int y)
{

}

void CheckInApp::mouseDragged(int x, int y, int button)
{
	screen.onMouseDragged(x , y , button);
}

void CheckInApp::mousePressed(int x, int y, int button)
{
	screen.onMouseDown(x , y ,dragRad,button);
}

void CheckInApp::mouseReleased(int x, int y, int button)
{
	screen.onMouseUp(x , y , button);
	//更新屏幕顶点信息
	screenUL = screen.getScreenUL( );
	screenUR = screen.getScreenUR( );
	screenDL = screen.getScreenDL( );
	screenDR = screen.getScreenDR( );
}

void CheckInApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	
}
