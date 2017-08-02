#include "ofApp.h"

//--------C style 函数--------------

//-----------------以上


float posX;
float posY;
Magics mMagic;
ofMesh mesh;
bool first = true;
int counter = 0;
//--------------------------------------------------------------
void ofApp::setup(){
	//内存分配 & 对象初始化 & 对象设置
	{
	}
	//从硬盘加载资源
	{
		mainVideo.loadMovie("intro.mp4"); 
		mainAudio.load("intro.mp3");
		liveHelper.setup(mainVideo.getFrameRate( ) , mainVideo.getDuration( ),5);
		ofxHelper::loadFrameColors("tone.clrs" , frameColors);
		logoVideo.loadMovie("stamp.mp4");
		yhFont.load("msyh.ttc" , 22);
		img.load("img.png");
		ofxHelper::LoadSequence(logoSequence , "Stamp/1200_" , 5);//测试时只读取 5 帧提高启动速度
	}
	//GUI 设定
	{
		gui.setup( );
		gui.setPosition(0 , 0);
		gui.add(bgColor.set("BG color" , ofColor(0)));
		gui.add(logicScreenIndex.set("Logic screen Num" , logicScreenIndex , 0 , logicScreenNum-1));
		gui.add(seimGifFPS.set("SEIM gif FPS" , seimGifFPS , 1 , 60));
		gui.add(seimOffSet.set("Seim drawing Offset" , seimOffSet 
							   , ofVec3f(-300 , -300 , 0) , ofVec3f(300 , 300 , 50)));
		gui.add(isPlaySeimGifOnce.set("Play SEIM gif Once" , isPlaySeimGifOnce));
		gui.add(doBlur.set("do blur" , false));
		gui.add(blurSize.set("blur size " , blurSize , 1 , 10));
		gui.add(btnLoad.setup("load"));
		gui.add(btnSave.setup("save"));
		btnLoad.addListener(this , &ofApp::load);
		btnSave.addListener(this , &ofApp::save);
		

		handFinderParams.setName("Hand Finder Params");
		handFinderParams.add(imgXFlipped.set("X Flipped" , imgXFlipped));
		handFinderParams.add(imgYFlipped.set("Y Flipped" , imgYFlipped));
		handFinderParams.add(findHandInterval.set("Find Hand Interval" , findHandInterval,1,15));
		handFinderParams.add(findColorROI.set("Color ROI" , findColorROI));
		handFinderParams.add(handROI.set("Hand ROI",handROI));
		handFinderParams.add(bgRecorded.set("bg recorded" , false));
		handFinderParams.add(thresh.set("ExtractRange" , thresh,ofVec3f(0,0,1),ofVec3f(1,1,100)));
		handFinderParams.add(dilateIter.set("dilate Iter" , dilateIter , 0 , 15));
		handFinderParams.add(erodeIter.set("erode Iter" , erodeIter , 0 , 20));
		handFinderParams.add(finderMinRad.set("finder Min Rad" , finderMinRad , 1 , 50));
		handFinderParams.add(finderMaxRad.set("finder Max Rad" , finderMaxRad , 50 , 300));
		handFinderParams.add(finderThresh.set("finder Thresh" , finderThresh , 1 , 80));
		handFinderParams.add(finderMaxMove.set("finder Max Move" , finderMaxMove , 1 , 100));
		gui.add(handFinderParams);

		projectionParams.setName("HandProjectArea");
		projectionParams.add(outputSize.set("Projector Size" ,outputSize
											, ofVec2f(1024 , 768) , ofVec2f(1920 , 1080)));
		projectionParams.add(A1.set("A1" , A1));
		projectionParams.add(A2.set("A2" , A2));
		projectionParams.add(B1.set("B1" , B1));
		projectionParams.add(B2.set("B2" , B2));
		gui.add(projectionParams);
	}
	//参数设置
	{

		bDrawDebug = false; // debug 绘图
		ofSetFrameRate(GLOBALS::FrameRate);
		ofBackground(55);
		ofSetCircleResolution(30);// 对于投影精度无需太高
		ofDisableArbTex( );
//		ofSetVerticalSync(true);		//垂直同步
		//ofEnableSmoothing();			//抗锯齿

		//ofSetBackgroundColor(255);
	}
	//mMagic = Magics(ofGetWidth()/2, ofGetHeight() / 2);

	/*///////-----Kinect things-----/////////*/
	{
		kinect_.open( );
		kinect_.initDepthSource( );
//		kinect_.initBodyIndexSource( );
		//	kinect.initBodySource();
		kinect_.initInfraredSource( );
		kinect_.initColorSource( );
		// get frames
		depth_ = kinect_.getDepthSource( );
		ir_ = kinect_.getInfraredSource( );
		color_ = kinect_.getColorSource( );
	}
	/*////////------  Simulate Input / Spell / Drawing  --------/////////*/
	{
		mesh = ofxHelper::Instance( ).GenMesh(6 , 6);
		mesh.enableTextures( );
		//建立逻辑屏幕
		for(int i = 0; i < logicScreenNum; i++)
		{
			logicScreen[i] = ofxHelper::GenVboMesh(2 , 2 , 150 , 300);
			screenBuffer[i].allocate(ofGetWindowWidth( ) , ofGetWindowHeight( ) , GL_RGB);
		}
		// spell
		auto wave0 = new SPEnterWave( );
		auto wave1 = new SPEnterWave( );
		auto soundcloud = new SPSoundCloud( );
		wave0->init(0 , 0 , 300 , 200);
		wave1->init(100 , 250 , 600 , 300);
		soundcloud->init(1600 , 500 , 500 , 500);
		soundcloud->setup(SPECTRUM_BANDS);
		SpellManager::Instance( ).addSpell(wave0);
		SpellManager::Instance( ).addSpell(wave1);
		SpellManager::Instance( ).addSpell(soundcloud);
		InputManager::Instance( ).addReceiver(wave0);
		InputManager::Instance( ).addReceiver(wave1);
		InputManager::Instance( ).addReceiver(soundcloud);
		
	}

	/*//////----Test Area----////////*/
	{
	
	}
// last line  
}

//--------------------------------------------------------------
void ofApp::update(){
	InputManager::Instance( ).update( );//这个必须第一行,每一帧开始状态初始化. 最后一行也行.这样一切都滞后一帧
	if(bFirstFrame)
	{
		bFirstFrame = false;
		mainVideo.play( );
		mainVideo.setVolume(0);// 关掉视频声音, 因为使用 windows api 无法访问 sample
		mainAudio.play( );
		liveHelper.start( );// video 第一次 update时 才开始播放. 
	}
	// 声音相关
	{
		ofSoundUpdate( );
		float *val = ofSoundGetSpectrum(SPECTRUM_BANDS);
		for(int i=0;i<SPECTRUM_BANDS;i++)
		{
			soundSpec[i] *= 0.97f;
			soundSpec[i] = max(soundSpec[i] , val[i]);
		}
		InputManager::Instance( ).sendSoundSpectrum(val);
	}
	//常规 update 
	mainVideo.update( );
	logoSequence.setFPS(seimGifFPS);
//---Kinect Stuff---------------
	kinectUpdateGuard.setInterval(findHandInterval);
	if(kinectUpdateGuard.canUpdate())
	{
		kinect_.update( );
		/*
		这里 110 frame 后比较安全, 否则可能 color 的 pixel 只
		有 3 个 channel ... 原因不明, 应该是 addon 的问题....
		*/
		if(kinect_.isFrameNew( ) && ofGetFrameNum( ) > 30)
		{
			handTrackHelper.perspAgent.updateParams(A1 , A2 , B1 , B2
													, ofRectangle(handROI.get( ).x*GLOBALS::OtherW
																  , handROI.get( ).y*GLOBALS::OtherH
																  , (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
																  , (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH));
			manDetectHelper.updatePerspectiveParams(A1 , A2 , B1 , B2
													, ofRectangle(handROI.get( ).x*GLOBALS::OtherW
																  , handROI.get( ).y*GLOBALS::OtherH
																  , (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
																  , (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH));
			if(!bgRecorded)
			{
				moveDetectHelper.setup(A1 , A2 , B1 , B2
									   , ofRectangle(handROI.get( ).x*GLOBALS::OtherW
													 , handROI.get( ).y*GLOBALS::OtherH
													 , (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
													 , (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH)
									   , ir_->getTypeName( ));

				handTrackHelper.setup(depth_->getPixels( ) , DEPTH_THRESH_RANGE
									  , ofRectangle(0 , 0 , 1024 , 768) , depth_->getTypeName( ));
				manDetectHelper.setupPerspectiveParams(A1 , A2 , B1 , B2 ,
													   ofRectangle(handROI.get( ).x*GLOBALS::OtherW
																   , handROI.get( ).y*GLOBALS::OtherH
																   , (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
																   , (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH));
				manDetectHelper.setup(depth_->getPixels( ) , DEPTH_THRESH_RANGE
									  , ofRectangle(0 , 0 , 480 , 270) , depth_->getTypeName( ));
				
				finder_ir.setup(ir_->getPixels() , IR_THRESH_RANGE,ir_->getTypeName());
				finder_color.setup(color_ ->getPixels(), COLOR_THRESH_RANGE,color_->getTypeName());

				/* 前几 frame 有时只有 1 个 channel ....*/
	//			cout << "color  " << color_->getPixels( ).getNumChannels( ) << endl;
	//			cout << "colorBG_IMG  " << colorBG_img.getPixels( ).getNumChannels( ) << endl;

				colorBG_img.setFromPixels(color_->getPixels( ));
//				depBG_img.setFromPixels(depth_->getPixels( ));
				bgRecorded = true;
				printf("%s" , "recorded BACKGROUND Image");
			}
			else
			{
				moveDetectHelper.updateParams(A1 , A2 , B1 , B2
											  , ofRectangle(handROI.get( ).x*GLOBALS::OtherW
															, handROI.get( ).y*GLOBALS::OtherH
															, (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
															, (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH)
											  ,1
				);
				moveDetectHelper.update(ir_->getPixels( ));
				handTrackHelper.update(thresh , erodeIter , dilateIter , finderMinRad , finderMaxRad
									   , finderThresh , finderMaxMove , 30 , doBlur , blurSize , depth_->getPixels( ));
				manDetectHelper.update(thresh , erodeIter , dilateIter , finderMinRad , finderMaxRad
									   , finderThresh , finderMaxMove , 30 , doBlur , blurSize , depth_->getPixels( ));
				if(handTrackHelper.isHandEnter())
				{
					//logoVideo.play( );
					logoSequence.play( true);
				}
				if(handTrackHelper.isHandLeave())
				{
					//logoVideo.stop( );
					logoSequence.stop( );
				}
				if(handTrackHelper.isHandTracked())
				{
					//logoVideo.update( );
					logoSequence.update( );
				}
			}
		}
	}
	
	InputManager::Instance().sendTouch(ofGetMouseX(), ofGetMouseY());
	//ofSetWindowTitle("FPS: "+ofToString(ofGetFrameRate()));
	SpellManager::Instance().batchUpdate();//这个放在最后一行,  中间可能会有 sendTouch 事件产生
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(drawVideo)
	{
		mainVideo.draw(1600 , 0);
		ofPushStyle( );
		ofSetColor(frameColors[liveHelper.calcCurrentFrame(mainVideo.getPosition())]);
		ofRect(1600 , 0 , 100 , 1080);
		ofPopStyle( );
	}
	manDetectHelper.draw( 0, 500);

	SpellManager::Instance( ).batchDraw( );

	if(bCheckInfo)
	{
		ofDrawBitmapString(ofToString(ofGetFrameRate( )) , 10 , 20);
	}
	
	// Speed Mode 不画以下所有内容
	if(isSpeedMode)
		return;
	{
		ofPushStyle( );
		ofPushMatrix( );
		if(finder_color.getObjImage( )->isAllocated( ))
		{
			finder_color.getObjImage( )->draw(GLOBALS::OtherW * 2 , GLOBALS::OtherH);
		}
		if(ir_)
		{
			ir_->draw(GLOBALS::OtherW * 2 , GLOBALS::OtherH);
			ofDrawBitmapString(" IR Image " , GLOBALS::OtherW * 2 , GLOBALS::OtherH + 20);
		}
		if(finder_ir.getObjImage( )->isAllocated( ))
		{
			finder_ir.getObjImage( )->draw(GLOBALS::OtherW * 2 , 0
										   , GLOBALS::OtherW , GLOBALS::OtherH);
			ofPushMatrix( );
			ofTranslate(0 , GLOBALS::OtherH);
			finder_ir.getFinder( ).draw( );
			if(finder_ir.checkHandTracked( ))
			{
				ofTranslate(GLOBALS::OtherW * 2 , 0);
				ofPushStyle( );
				ofSetColor(22 , 233 , 22);
				ofDrawRectangle(finder_ir.getHand( ));
				ofPopStyle( );
			}
			ofPopMatrix( );
		}
		if(depth_)
		{
			depth_->draw(GLOBALS::OtherW , GLOBALS::OtherH);
			ofDrawBitmapString(" Depth Image " , GLOBALS::OtherW , GLOBALS::OtherH + 20);
		}
		if(moveDetectHelper.getDiffImage().isAllocated())
		{
			moveDetectHelper.getDiffImage( ).draw(GLOBALS::OtherW *3, 0);
		}
		if(manDetectHelper.Finder().getObjImage()->isAllocated())
		{
			manDetectHelper.Finder( ).getObjImage( )->draw(GLOBALS::OtherW , 0);
		}
		if(handTrackHelper.handFinder.getObjImage( )->isAllocated( ))
		{
			handTrackHelper.handFinder.getObjImage( )->draw(GLOBALS::OtherW , 0);
			handTrackHelper.handFinder.getFinder( ).draw( );

			if(handTrackHelper.handFinder.checkHandTracked( ))
			{
				ofPushMatrix( );
				ofTranslate(irImgPos + handTrackHelper.perspAgent.getROI( ).getPosition( ));// depth 图像看不清,故画到 ir 图像上
				ofPushStyle( );
				ofNoFill( );
				ofSetLineWidth(3);
				ofSetColor(22 , 233 , 22);
				handTrackHelper.handFinder.getHandFitQuad( ).draw( );
				//			threadedFnd_depth.get()->getHandFitQuad( ).draw( );
				ofPopStyle( );
				ofPopMatrix( );

				ofPushMatrix( );
				ofTranslate(1600 , 0);
				auto handProjectPos = handTrackHelper.getHandProjPos( );
				logoSequence.draw(handProjectPos.x - 15 * blurSize + seimOffSet.get( ).x
								  , handProjectPos.y - 15 * blurSize + seimOffSet.get( ).y
								  , 30 * blurSize + seimOffSet.get( ).z
								  , 30 * blurSize + seimOffSet.get( ).z);

				if(bDrawDebug)
				{
					ofDrawBitmapString("warped pos: " + ofToString(handProjectPos)
									   , handProjectPos + ofPoint(0 , 20));
					ofDrawCircle(handProjectPos , 33);
				}
				ofPopMatrix( );
			}
		}
		if(color_)
		{
			color_->draw(colorImgPos.x , colorImgPos.y , GLOBALS::OtherW , GLOBALS::OtherW*9.0f / 16);
			ofDrawBitmapString("Color Image" , GLOBALS::OtherW * 2 , 20);
		}
		//	ofDisableArbTex();//关掉默认 arbTex 坐标,
		//	ofEnableNormalizedTexCoords(); //换成 normalizedTex 坐标
		//	img.getTexture().bind();
		//	mesh.draw();
		//	img.getTexture().unbind();
		//	ofSetColor(111, 111, 44);
		//	yhFont.drawString("Chinese?你好", 555, 555);

		//	fatLine.draw();
		ofPopStyle( );
		ofPopMatrix( );
	}

	/////////--------------- 下面是 Debug 绘图------------///////////////
	do
	{
		if(!bDrawDebug)
			break;
		
		ofPushStyle( );
		ofPushMatrix( );
		ofNoFill( );
		//人群动作幅度
		ofPolyline moveLine;
		auto moves = moveDetectHelper.getColMoveAmounts( );
		for(int i = 0; i<moves.size( ); i++)
		{
			moveLine.addVertex(i , 100*moves[i]);
		}
		ofDrawBitmapString("Column 1 movement: "+ofToString(moves[0]) , 0 , 200);
		moveLine.draw( );
		//声音频谱
		for(int i=0;i<SPECTRUM_BANDS;i++)
		{
			ofRect(10+i*5,800,3,-soundSpec[i]*100 );
		}
		//		fatLine.drawDebug();
		if(drawWireFrame)
		{
			mesh.drawWireframe( );
			ofCircle(ofGetMouseX( ) , ofGetMouseY( ) , dragRad);
			for(auto a : logicScreen)
			{
				a.drawWireframe( );
				ofxHelper::DrawVertices(a.getVertices( ));
			}
		}
		//画出 hand投影范围 的 A1 A2 B1 B2
		{
			ofPushMatrix( );
			ofPushStyle( );
			ofFill( );
			ofTranslate(irImgPos + handTrackHelper.perspAgent.getROI( ).getPosition( ));
			auto aabb = handTrackHelper.perspAgent.getAABB( );// aabb 长度只有 4 
			ofSetColor(233 , 23 , 22);
			ofLine(aabb[0] , aabb[1]);
			ofLine(aabb[0] , aabb[2]);
			ofLine(aabb[1] , aabb[3]);
			ofLine(aabb[2] , aabb[3]);
			//		ofLine(A1 , A2);
			//		ofLine(B1 , B2);
			//		ofLine(A1 , B1);
			//		ofLine(A2 , B2);
			ofSetColor(233 , 23 , 22);
			//		ofCircle(A1 , 4);
			ofCircle(aabb[0] , 4);
			ofSetColor(23 , 233 , 22);
			//		ofCircle(A2 , 4);
			ofCircle(aabb[1] , 4);
			ofSetColor(23 , 23 , 232);
			//		ofCircle(A2 , 4);
			ofCircle(aabb[2] , 4);
			ofSetColor(23 , 223 , 222);
			//		ofCircle(A2 , 4);
			ofCircle(aabb[3] , 4);
			ofSetColor(233);
			//		ofDrawBitmapString("A1: " + ofToString(A1) , A1.get( ) + ofPoint(0 , 10));
			//		ofDrawBitmapString("A2: " + ofToString(A2) , A2.get( ) + ofPoint(0 , 10));
			//		ofDrawBitmapString("B1: " + ofToString(B1) , B1.get( ) + ofPoint(0 , 10));
			//		ofDrawBitmapString("B2: " + ofToString(B2) , B2.get( ) + ofPoint(0 , 10));
			ofDrawBitmapString("A1: " + ofToString(aabb[0]) , aabb[0] + ofPoint(0 , -20));
			ofDrawBitmapString("A2: " + ofToString(aabb[1]) , aabb[1] + ofPoint(0 , -20));
			ofDrawBitmapString("B1: " + ofToString(aabb[2]) , aabb[2] + ofPoint(0 , 30));
			ofDrawBitmapString("B2: " + ofToString(aabb[3]) , aabb[3] + ofPoint(0 , 30));
			ofPopStyle( );
			ofPopMatrix( );
		}
	
		// 在 source 上面画出 ROI
		ofSetColor(222 , 222 , 22);
		if(findColorROI)
		{
			ofTranslate(colorImgPos);
		}
		else
		{
			ofTranslate(irImgPos);
		}
		ofDrawRectangle(handROI.get( ).x*GLOBALS::OtherW 
								, handROI.get( ).y*GLOBALS::OtherH
						, (handROI.get( ).z - handROI.get( ).x)*GLOBALS::OtherW
						, (handROI.get( ).w - handROI.get( ).y)*GLOBALS::OtherH);

		ofPopMatrix( );
		ofPopStyle( );
	}
	while(false);
	if(drawGui)
	{
		gui.draw( );
	}
}

void ofApp::exit()
{
	SafeCloseKinect(kinect_);
	yhFont.unloadFont();
	mainVideo.close( );
	logoVideo.close( );
	ofSoundStreamClose( );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	std::cout << "key: " << key << endl;
	switch (key)
	{
	case VK_SPACE:
		ofSetBackgroundColor(bgColor);
		if(color_)
		{
			colorBG_img.setFromPixels(color_->getPixels( ));
		}
		if(ir_)
		{
			finder_ir.resetBG(ir_->getPixels() );
		}
		if(depth_)
		{
			handTrackHelper.resetBG(depth_->getPixels( ));
			manDetectHelper.resetBG(depth_->getPixels( ));
		}
		handTrackHelper.perspAgent.setXFlipped(imgXFlipped);
		handTrackHelper.perspAgent.setYFlipped(imgYFlipped);
		break;
	case OF_KEY_UP:
		dragRad++;
		std::cout << "drag rad " << dragRad << endl;
		break;
	case OF_KEY_DOWN:
		dragRad--;
		std::cout << "drag rad " << dragRad << endl;
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
		cout << "GUI loaded from settings.xml" << endl;
		break;
	case 'm':
		break;
	case 'p':
		mainVideo.isPlaying()? mainVideo.pause():mainVideo.play();
		break;
	case 's':
		isSpeedMode = !isSpeedMode;
		break;
	case 'v':
		drawVideo = !drawVideo;
		break;
	case 'w':
		drawWireFrame = !drawWireFrame;
		break;
	case '1':
	case '2':
	case '3':
		logicScreenIndex = key-'1';
		break;
	case '5': A1.set(ofVec3f(ofGetMouseX( ) - irImgPos.x , ofGetMouseY( ) - irImgPos.y)); break;
	case '6': A2.set(ofVec3f(ofGetMouseX( ) - irImgPos.x , ofGetMouseY( ) - irImgPos.y)); break;
	case '7': B1.set(ofVec3f(ofGetMouseX( ) - irImgPos.x , ofGetMouseY( ) - irImgPos.y)); break;
	case '8': B2.set(ofVec3f(ofGetMouseX( ) - irImgPos.x , ofGetMouseY( ) - irImgPos.y)); break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(canDrag)
	{
		logicScreen[logicScreenIndex].getVertices( )[targetVertexIndex].x = x;
		logicScreen[logicScreenIndex].getVertices( )[targetVertexIndex].y = y;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	printf("%s %d %d \n" , "PressPos : " , x , y);

	mouseDownX = x; mouseDownY = y;
	targetVertexIndex = ofxHelper::GetClosestVertexIndex(
		x , y , logicScreen[logicScreenIndex].getVertices( ));
	auto target = logicScreen[logicScreenIndex].getVertex(targetVertexIndex);
	// mouseDown 的时候距离最近的 vertex 在影响范围内
	if(ofDistSquared(mouseDownX , mouseDownY , target.x , target.y) < dragRad*dragRad)
	{
		canDrag = true;
	}
	
	//-------从这开始  xy 都被改了------------
	x = ofMap(x, GLOBALS::OtherW, GLOBALS::OtherW * 2, 0, GLOBALS::OtherW, true);
	y = ofMap(y, GLOBALS::OtherH,GLOBALS::OtherH*2,0,GLOBALS::OtherH,true);
//	printf("%s %d  \n" , "depth Value : " , depth_->getPixels( )[y*GLOBALS::OtherW + x]) ;;
//	cout << "depImg: " << depBG_img.getColor(x, y) << endl;
//	cout << "depDIFF Img: " << depDiff.getColor(x, y) << endl;
	cout << "Color Img: " << color_->getPixels( ).getColor(x , y) << endl;;
//	cout << "Color DIFF" << colorDiff_img.getPixels( ).getColor(x,y)<< endl;
	cout << "IR img: " << ir_->getPixels( )[y*GLOBALS::OtherW + x] << endl;

	//	printf("%s %d  \n", "ir Value : ", irObj_mat.data[y*GLOBALS::OtherW + x]);
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	canDrag = false;
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	dragRad = dragRad + scrollY > 0 && dragRad + scrollY < 30 
		? dragRad + scrollY : dragRad;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::audioRequested(float* input, int size, int channels)
{

}

void ofApp::audioReceived(float* input, int size, int channels)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//-----------------------------------

//void ofApp::setupContourFinder()
//{
//	contourFinder.setMinAreaRadius(finderMinRad);
//	contourFinder.setMaxAreaRadius(finderMaxRad);
//	contourFinder.setThreshold(finderThresh);
//	contourFinder.getTracker( ).setMaximumDistance(finderMaxMove);
//	contourFinder.getTracker( ).setPersistence(30);
//}

void ofApp::saveParams(string fileCode)
{
	ofXml xml;
	xml.serialize(projectionParams);
	xml.save(fileCode + "_projectionParams.xml");
	cout << fileCode + " projection parameters saved"<< endl;
	xml.clear( );
	xml.serialize(handFinderParams);
	xml.save(fileCode + "_handFinderParams.xml");
	cout << fileCode + " handFinder parameters saved" << endl;
}

void ofApp::loadParams(string fileCode)
{
	ofXml xml;
	if(xml.load(fileCode + "_projectionParams.xml"))
	{
		xml.deserialize(projectionParams);
		cout << "loaded: " + fileCode + "_projectionParams.xml" << endl;
	}
	xml.clear( );
	if(xml.load(fileCode + "_handFinderParams.xml"))
	{
		xml.deserialize(handFinderParams);
		cout << "loaded: " + fileCode + "_handFinderParams.xml" << endl;
	}
}

void ofApp::load( )
{
	loadParams("demo");
}

void ofApp::save()
{
	saveParams("demo");
}