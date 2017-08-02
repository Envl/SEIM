#include "MainApp.h"


void MainApp::switchDrawings(Phase phase)
{
	switch (phase)
	{
	case Waiting:
		soundcloud->Deactivate( );
		dotWave->Activate( );
		ballMatL->Deactivate( );
		ballMatR->Deactivate( );
		rainLeft->Deactivate();
		rainR->Deactivate( );
		maniaL->Deactivate();
		maniaR->Deactivate();
		modelL->Deactivate();
		modelR->Deactivate();
		videoL.Deactivate();
		videoR.Deactivate();
		break;
	case La:
		modelL->setActiveModel((int)ofRandom(modelL->getModelNum()));
		modelL->clearAccumMoveAmount( );
		modelR->setActiveModel((int)ofRandom(modelR->getModelNum()));
		modelR->clearAccumMoveAmount( );
		modelL->Activate();
		modelR->Activate();
		dotWave->Deactivate( );
		soundcloud->Activate( );
		ballMatL->Deactivate( );
		ballMatR->Deactivate( );
		rainLeft->Deactivate();
		rainR->Deactivate( );
		maniaL->Deactivate();
		maniaR->Deactivate();
		videoL.Deactivate();
		videoR.Deactivate();
		break;
	case Video:
		videoL.Activate();
		videoR.Activate();
		dotWave->Activate( );
		soundcloud->Deactivate( );
		ballMatL->Deactivate( );
		ballMatR->Deactivate( );
		rainLeft->Deactivate();
		rainR->Deactivate( );
		maniaL->Deactivate();
		maniaR->Deactivate();
		modelL->Deactivate();
		modelR->Deactivate();
		break;
	case Hey:
		rainLeft->Deactivate();
		rainR->Deactivate();
		ballMatL->Deactivate();
		ballMatR->Deactivate();
		maniaL->Activate();
		maniaR->Activate();
		dotWave->Deactivate( );
		soundcloud->Activate( );
		modelL->Deactivate();
		modelR->Deactivate();
		videoL.Deactivate();
		videoR.Deactivate();
		break;
	case Prelude:
		ballMatL->Activate( );
		ballMatR->Activate( );
		dotWave->Deactivate( );
		soundcloud->Activate( );
		rainLeft->Deactivate();
		rainR->Deactivate( );
		maniaL->Deactivate();
		maniaR->Deactivate();
		modelL->Deactivate();
		modelR->Deactivate();
		videoL.Deactivate();
		videoR.Deactivate();
		//prelude 鼓点  ball matrix rotate speed according to beat
		break;
	case Solo:
		rainLeft->Activate();
		rainR->Activate( );
		dotWave->Deactivate( );
		soundcloud->Activate( );
		ballMatL->Deactivate( );
		ballMatR->Deactivate( );
		maniaL->Deactivate();
		maniaR->Deactivate();
		modelL->Deactivate();
		modelR->Deactivate();
		videoL.Deactivate();
		videoR.Deactivate();
		break;
	case End:
		ballMatL->Deactivate( );
		ballMatR->Deactivate( );
		dotWave->Deactivate( );
		soundcloud->Deactivate( );
		rainLeft->Deactivate();
		rainR->Deactivate( );
		maniaL->Deactivate();
		maniaR->Deactivate();
		modelL->Deactivate();
		modelR->Deactivate();
		videoL.Deactivate();
		videoR.Deactivate();
		//准备下一场
		showEnd.setReady();
		break;
	}
}

void MainApp::setup( )
{
	// debug 才取消注释
	/*firstFoundMan.setReady();
	firstFoundMan.check();*/

	//时间调度
	{
		/*auto tt = Trigger(0 , "drum_start");
		triggerPool.push_back(tt);
		phaseMap[tt.getName( )] = Phase::Solo;
		auto ttt = Trigger(50.042 , "la_one2solo");
		triggerPool.push_back(ttt);
		phaseMap[ttt.getName( )] = Phase::Solo;*/


		auto t0 = Trigger(0, "wait");
			triggerPool.push_back(t0);
			phaseMap[t0.getName()] = Phase::Waiting;
		auto t1 = Trigger(54.917,"drum_start");
			triggerPool.push_back(t1);
			phaseMap[t1.getName()] = Phase::Prelude;
		auto t2 = Trigger(62.375,"drum_end");
			triggerPool.push_back(t2);
			phaseMap[t2.getName()] = Phase::Video;
		auto t3 = Trigger(167.083, "la_one");
			triggerPool.push_back(t3);
			phaseMap[t3.getName()] = Phase::La;

		auto t4 = Trigger(181.875, "la_one2solo");
			triggerPool.push_back(t4);
			phaseMap[t4.getName()] = Phase::Solo;
		auto t5 = Trigger(189.083, "solo_end");
			triggerPool.push_back(t5);
			phaseMap[t5.getName()] = Phase::Video;
		auto t6 = Trigger(228.458, "la_two");
			triggerPool.push_back(t6);
			phaseMap[t6.getName()] = Phase::La;

		auto t7 = Trigger(243.417, "la_two2solo_two");
			triggerPool.push_back(t7);
			phaseMap[t7.getName()] = Phase::Solo;
		auto t8 = Trigger(248.958, "solo_two_end");
			triggerPool.push_back(t8);
			phaseMap[t8.getName()] = Phase::Video;
		auto t9 = Trigger(311.042, "hey");
			triggerPool.push_back(t9);
			phaseMap[t9.getName()] = Phase::Hey;
		auto t10 = Trigger(337.625, "hey_end");
			triggerPool.push_back(t10);
			phaseMap[t10.getName()] = Phase::Video;
		auto t11 = Trigger(339.875, "la_three");
			triggerPool.push_back(t11);
			phaseMap[t11.getName()] = Phase::La;
		auto t12 = Trigger(371.308, "the_end");
			triggerPool.push_back(t12);
			phaseMap[t12.getName()] = Phase::End;
	}
	// Params
	bDrawDebug = false;
	ofSetSphereResolution(15);
	ofSetCircleResolution(40);
	ofDisableArbTex( );//这个一定要放在创建 fboScreen 前
	//ofSetVerticalSync(true);// 垂直同步
	//从硬盘加载资源
	{
		for (int i = 0; i < END_SONG_NUM; i++)
		{
			char name[30];
			sprintf(name, "%s%d.mp4", "endVideo/", i);
			endVideos[i].load(name);
			endVideos[i].player.setLoop(false);
			endVideos[i].setHideWhenFinish(true);
			endSongIndex = ofRandom(END_SONG_NUM);
		}
		countVideo.load("endVideo/count.mp4");
		videoL.load("videoL.mp4");
		videoR.load("videoR.mp4");
		//videoF.load("fingers.mov");
		videoIntro.load("intro.mp4");
		mainVideo.load("main.mp4");
		mainAudio.load("main.mp3");
		liveHelper.setup(mainVideo.player.getFrameRate( ) , mainVideo.player.getDuration( ) , 5);
		ofxHelper::loadFrameColors("tone.clrs" , frameColors);
		//yhFont.load("msyh.ttc" , 22);
	}
	// Screens
	{
		screenD.setup(1280 , 1024 , 4 , 3);
		screenL.setup(1024 , 768 , 40 , 30);
		screenR.setup(1024 , 768 , 40 ,30);
		screenF.setup(1920 , 1080 , 16 , 9);
		logicScreen.allocate(screenL.getScreenWidth()
			+ screenR.getScreenWidth()
			+ screenD.getScreenWidth()
			//+ screenF.getScreenWidth( )
			,max(screenD.getScreenHeight()
				, max(screenL.getScreenHeight(), screenR.getScreenHeight())
			)
		);
	}
	// GUI
	{
		gui.setup( );
			gui.setPosition(0 , 0);
			gui.add(kinectUpdateInterval.set("Kinect update interval" , 1 , 1 , 20));
			gui.add(lightOffsetMax.set("light offset max" , lightOffsetMax , ofVec3f(-200 , -200 , 0) , ofVec3f(200 , 200 , 300)));
		//mainAppSpellParams.setName("MainApp Spell Params");
			gui.add(moveScaleParam.set("Move Scale Param" , moveScaleParam , 0 ,30));
		mainappFinderParams.setName("MainApp Finder Params");
			mainappFinderParams.add(bgColor.set("BG color" , ofColor(0)));
			mainappFinderParams.add(imgXFlipped.set("X Flipped" , imgXFlipped));
			mainappFinderParams.add(imgYFlipped.set("Y Flipped" , imgYFlipped));
			mainappFinderParams.add(ROI.set("ROI" , ROI));
			//mainappFinderParams.add(bgRecorded.set("bg recorded" , false));
			mainappFinderParams.add(thresh.set("ExtractRange" , thresh , ofVec3f(0 , 0 , 1) , ofVec3f(1 , 1 , 100)));
			mainappFinderParams.add(dilateIter.set("dilate Iter" , dilateIter , 0 , 15));
			mainappFinderParams.add(erodeIter.set("erode Iter" , erodeIter , 0 , 20));
			mainappFinderParams.add(finderMinRad.set("finder Min Rad" , finderMinRad , 1 , 50));
			mainappFinderParams.add(finderMaxRad.set("finder Max Rad" , finderMaxRad , 50 , 300));
			mainappFinderParams.add(finderThresh.set("finder Thresh" , finderThresh , 1 , 80));
			mainappFinderParams.add(finderMaxMove.set("finder Max Move" , finderMaxMove , 1 , 100));
			mainappFinderParams.add(doBlur.set("do blur" , false));
			mainappFinderParams.add(blurSize.set("blur size" , blurSize , 1 , 10));
		gui.add(mainappFinderParams);

		mainappProjectionParams.setName("MainApp Projection Area");
			mainappProjectionParams.add(screenLPos.set("Left Screen pos" , screenLPos , ofVec2f(0) , ofVec2f(1920 , 1080)*3));
			mainappProjectionParams.add(screenRPos.set("Right Screen pos" , screenRPos , ofVec2f(0) , ofVec2f(1920 , 1080)*3));
			mainappProjectionParams.add(screenDPos.set("Down Screen pos" , screenDPos , ofVec2f(0) , ofVec2f(1920 , 1080)*3));
			mainappProjectionParams.add(screenFPos.set("Front Screen pos" , screenFPos , ofVec2f(0) , ofVec2f(1920 , 1080)*3));
			mainappProjectionParams.add(outputSizeL.set("Left Projector Size" , outputSizeL
														, ofVec2f(1024 , 768) , ofVec2f(1920 , 1080)));
			mainappProjectionParams.add(outputSizeR.set("Right Projector Size" , outputSizeR
														, ofVec2f(1024 , 768) , ofVec2f(1920 , 1080)));
			mainappProjectionParams.add(outputSizeD.set("Down Projector Size" , outputSizeD
														, ofVec2f(1024 , 768) , ofVec2f(1920 , 1080)));
			mainappProjectionParams.add(outputSizeF.set("Front Projector Size" , outputSizeF
														, ofVec2f(1024 , 768) , ofVec2f(1920 , 1080)));
			mainappProjectionParams.add(A1.set("A1" , A1));
			mainappProjectionParams.add(A2.set("A2" , A2));
			mainappProjectionParams.add(B1.set("B1" , B1));
			mainappProjectionParams.add(B2.set("B2" , B2));
			// 屏幕四角的坐标
			{
				mainappProjectionParams.add(screenFUL.set("front screen UL" , screenFUL));
				mainappProjectionParams.add(screenLUL.set("left screen UL" , screenLUL));
				mainappProjectionParams.add(screenRUL.set("right screen UL" , screenRUL));
				mainappProjectionParams.add(screenDUL.set("down screen UL" , screenDUL));

				mainappProjectionParams.add(screenFUR.set("front screen UR" , screenFUR));
				mainappProjectionParams.add(screenLUR.set("left screen UR" , screenLUR));
				mainappProjectionParams.add(screenRUR.set("right screen UR" , screenRUR));
				mainappProjectionParams.add(screenDUR.set("down screen UR" , screenDUR));

				mainappProjectionParams.add(screenFDL.set("front screen DL" , screenFDL));
				mainappProjectionParams.add(screenLDL.set("left screen DL" , screenLDL));
				mainappProjectionParams.add(screenRDL.set("right screen DL" , screenRDL));
				mainappProjectionParams.add(screenDDL.set("down screen DL" , screenDDL));

				mainappProjectionParams.add(screenFDR.set("front screen DR" , screenFDR));
				mainappProjectionParams.add(screenLDR.set("left screen DR" , screenLDR));
				mainappProjectionParams.add(screenRDR.set("right screen DR" , screenRDR));
				mainappProjectionParams.add(screenDDR.set("down screen DR" , screenDDR));
			}

			gui.add(mainappProjectionParams);
			//读取gui参数
			gui.loadFromFile("settings.xml");
			cout << "GUI loaded from settings.xml" << endl;
	}
	// Kinect
	kinect_.open( );
	kinect_.initDepthSource( );
	kinect_.initInfraredSource( );
	ir_ = kinect_.getInfraredSource( );
	depth_ = kinect_.getDepthSource( );
	// Spell
	{
		auto maxHeight = max(screenD.getScreenHeight( )
							 , max(screenL.getScreenHeight( ) , screenR.getScreenHeight( ))
		);

		dotWave = new SPDotWave( );
		ballMatL = new SPBallMatrix( );
		ballMatR = new SPBallMatrix( );
		rainR = new SPRain( );
		rainLeft = new SPRain();
		maniaL = new SPMania( );
		maniaR = new SPMania( );
		modelL = new SPModelDeform( );
		modelR = new SPModelDeform( );
		soundcloud = new SPSoundCloud( );

		dotWave->init(screenL.getScreenWidth( ) + screenR.getScreenWidth( )
					  , maxHeight - screenD.getScreenHeight( )
					  , screenD.getScreenWidth( ) , screenD.getScreenHeight( ),0);
			dotWave->setup(40 , 0);
			dotWave->Deactivate();
			SpellManager::Instance( ).addSpell(dotWave);
			InputManager::Instance( ).addReceiver(dotWave);
		ballMatL->init(0 , maxHeight - screenL.getScreenHeight( )
					, screenL.getScreenWidth( ) , screenL.getScreenHeight( ) , 0);
			ballMatL->setup(9 , 0);
			ballMatL->Deactivate();
			SpellManager::Instance( ).addSpell(ballMatL);
			InputManager::Instance( ).addReceiver(ballMatL);
		ballMatR->init(screenL.getScreenWidth( ) , maxHeight - screenR.getScreenHeight( )
					, screenR.getScreenWidth( ) , screenR.getScreenHeight( ) , 0);
			ballMatR->setup(9 , 0);
			ballMatR->Deactivate();
			SpellManager::Instance( ).addSpell(ballMatR);
			InputManager::Instance( ).addReceiver(ballMatR);
		rainLeft->init(0, maxHeight - screenL.getScreenHeight()
			, screenL.getScreenWidth(), screenL.getScreenHeight(), 0);
			rainLeft->setup(600, 18, 0.13f);
			rainLeft->Deactivate();
			SpellManager::Instance().addSpell(rainLeft);
			InputManager::Instance().addReceiver(rainLeft);
		rainR->init(screenL.getScreenWidth( ) , maxHeight - screenR.getScreenHeight( )
					, screenR.getScreenWidth( ) , screenR.getScreenHeight( ),0);
			rainR->setup(600 , 18 , 0.13f);
			rainR->setXflip(true);
			rainR->Deactivate();
			SpellManager::Instance( ).addSpell(rainR);
			InputManager::Instance( ).addReceiver(rainR);
		
		maniaL->init(0 ,screenL.getScreenHeight()*3/7
					 , screenL.getScreenWidth( ) , screenL.getScreenHeight( )/2+100);
			maniaL->setup(1 , SPECTRUM_BANDS,10,18, ManiaType::ManiaDot);
			maniaL->setXflipped(true);
			maniaL->Deactivate();
			SpellManager::Instance( ).addSpell(maniaL);
			InputManager::Instance( ).addReceiver(maniaL);
		maniaR->init(screenL.getScreenWidth( ) , screenR.getScreenHeight()*3/7
						 , screenR.getScreenWidth( ) , screenR.getScreenHeight( ) / 2+100);
			maniaR->setup(1 , SPECTRUM_BANDS,10,18,ManiaType::ManiaDot);
			//maniaR->setScale(-1 , 1 , 1); 
			maniaR->setXflipped(true);
			maniaR->Deactivate();
			SpellManager::Instance( ).addSpell(maniaR);
			InputManager::Instance( ).addReceiver(maniaR);
		modelL->init(0 , maxHeight - screenL.getScreenHeight( )
					 , screenL.getScreenWidth( ) , screenL.getScreenHeight( ));
			modelL->setup("models\\head.ply" , 1 , SPECTRUM_BANDS);
			modelL->addModel("models\\horse.ply");
			modelL->addModel("models\\pliers.ply");
			modelL->addModel("models\\pyramid.ply");
			modelL->addModel("models\\mask.ply");
			modelL->setParams(45 , 15);
			modelL->setModelIndex(4);
			modelL->Deactivate();
			SpellManager::Instance( ).addSpell(modelL);
			InputManager::Instance( ).addReceiver(modelL);
		modelR->init(screenL.getScreenWidth( ) , maxHeight - screenR.getScreenHeight( )
					 , screenR.getScreenWidth( ) , screenR.getScreenHeight( ));
			modelR->setup("models\\head.ply" , 1 , SPECTRUM_BANDS);
			modelR->addModel("models\\horse.ply");
			modelR->addModel("models\\pliers.ply");
			modelR->addModel("models\\pyramid.ply");
			modelR->addModel("models\\mask.ply");
			modelR->setParams(45 , 15);
			modelR->setModelIndex(0);
			modelR->Deactivate();
			SpellManager::Instance( ).addSpell(modelR);
			InputManager::Instance( ).addReceiver(modelR);
		soundcloud->init(screenL.getScreenWidth( ) + screenR.getScreenWidth( )
							 , maxHeight - screenD.getScreenHeight( )
							 , screenD.getScreenWidth( ) , screenD.getScreenHeight( ));
			soundcloud->setup(SPECTRUM_BANDS , 150  ,2.0f,1);
			soundcloud->Deactivate();
			SpellManager::Instance( ).addSpell(soundcloud);
			InputManager::Instance( ).addReceiver(soundcloud);

	}
}

void MainApp::update( )
{
	//时间调度
	{
		for(auto &t : triggerPool)
		{
			if(t.checkTime(mainVideo.player.getPosition( )))
			{
				if(phaseMap.find(t.getName( )) != phaseMap.end( ))
				{
					auto pha = phaseMap[t.getName( )];
					switchDrawings(pha);
				}
			}
		}
	}
	
	InputManager::Instance( ).update( );
	if(bFirstFrame)
	{
		bFirstFrame = false;
		videoIntro.player.play();
		videoIntro.player.pause();
		mainVideo.player.play( );
		mainVideo.player.pause( );
		mainVideo.player.setVolume(0);// 关掉视频声音, 因为使用 windows api 无法访问 sample
		videoL.player.play( );
		videoL.player.pause( );
		//videoL.player.setVolume(0);
		videoR.player.play( );
		videoR.player.pause( );
		//videoR.player.setVolume(0);
		//videoF.player.play( );
		//videoF.player.pause( );
		//videoF.player.setVolume(0);
		mainAudio.play( );
		mainAudio.setPaused(true);
		liveHelper.start( );// video 第一次 update时 才开始播放. 
	}
	//第一次找到人 开播宣传视频
	if(firstFoundMan.check())
	{
		videoIntro.player.setLoop(false);
		videoIntro.player.play();
		videoIntro.Activate();
		bIntro = true;
	}
	//intro 播放完毕  开播演出视频
	if (videoIntro.player.getDuration() - videoIntro.player.getPosition() <= 1) {
		introEnd.setReady();
		if (introEnd.check()) {
			bPlaying = true;
			bIntro = false;
		}
	}
	//演出播放完毕
	if (showEnd.check()) {
		bFirstOpen = false;
		bPlaying = false;
		mainAudio.stop();
		bFirstFrame = true;
		endTime = ofGetElapsedTimef();
		chillReactivated.reActivate();
		mainVideo.player.stop();
		videoL.player.stop();
		videoR.player.stop();
		countVideo.Activate();
		endVideos[endSongIndex].Activate();
	}
	// 未进入 intro 也 不在 播放   也不是第一次启动程序
	if (!bPlaying && !bIntro &&!bFirstOpen)
	{
		//冷却完成
		if (ofGetElapsedTimef() - endTime > CHILL_SECONDS)
		{
			chillReactivated.setReady();
			if (chillReactivated.check()) { //只在初次check成功才返回true
				for (auto &t : triggerPool)
				{
					t.reActivate();
				}
				showEnd.reActivate();
				introEnd.reActivate();
				firstFoundMan.reActivate();
				//关掉倒计时
				countVideo.Deactivate();
				countVideo.player.stop();
				//关掉放歌
				endVideos[endSongIndex].player.stop();
				endVideos[endSongIndex].Deactivate();
			}
		}
		//冷却中   
		else {
			//刚演出结束  放歌
			if (ofGetElapsedTimef() - endTime < END_SONG_DURATION)
			{
				auto &p = endVideos[endSongIndex].player;
				if (!p.isPlaying())
				{
					p.play();
					//放歌同时 倒计时
					countVideo.player.play();
					countVideo.Activate();
				}
				else
				{
					//播放结束 换歌
					if (p.getDuration() - p.getPosition() <= 1)
					{
						p.stop();
						endVideos[endSongIndex].Deactivate();
						endSongIndex = ofRandom(END_SONG_NUM);
						endVideos[endSongIndex].Activate();
						endVideos[endSongIndex].player.play();
					}
				}
				
			}
			// 放歌结束  进入倒计时
			//else
			//{
			//	if (endVideos[endSongIndex].player.isPlaying()) {
			//		//关掉正在播放的歌
			//		endVideos[endSongIndex].player.stop();
			//		endVideos[endSongIndex].Deactivate();
			//	}
			//	if (!countVideo.player.isPlaying()){
			//		countVideo.player.play();
			//		countVideo.Activate();
			//	}
			//}
		}
	}
	// 播放控制
	{
		if(bPlaying )
		{
			videoIntro.player.stop();
			videoIntro.Deactivate();
			if(mainVideo.player.isPaused( ))
			{
				mainVideo.player.play( );
				videoL.player.play( );
				videoR.player.play( );
				//videoF.player.play( );
				mainAudio.setPaused(false);
				//((SPModelDeform*)(SpellManager::Instance( ).getSpell("Model1")))
					//->setActiveModel(ofRandom(4));
				//modelL->setActiveModel(ofRandom(4));
			}
		}
		else
		{
			mainVideo.player.pause( );
			videoL.player.pause( );
			videoR.player.pause( );
			//videoF.player.pause( );
			mainAudio.setPaused(true);
		}
	}
	// 声音
	{
		ofSoundUpdate( );
		float *val = ofSoundGetSpectrum(SPECTRUM_BANDS);
		for(int i = 0; i<SPECTRUM_BANDS; i++)
		{
			soundSpec[i] *= 0.97f;
			soundSpec[i] = max(soundSpec[i] , val[i]);
		}
		InputManager::Instance( ).sendSoundSpectrum(val);
	}
	//常规 update
	countVideo.update();
	endVideos[endSongIndex].update();
	mainVideo.player.update( );
	videoL.player.update( );
	videoR.player.update( );
				//videoF.player.update( );
	// Kinect
	kinectUpdateGuard.setInterval(kinectUpdateInterval);
	if(kinectUpdateGuard.canUpdate( ))
	{
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
									  , ofRectangle(0 , 0 , 1024 , 768) , depth_->getTypeName( ));
				moveDetectHelper.setup(A1 , A2 , B1 , B2
									   , ofRectangle(ROI.get( ).x*GLOBALS::OtherW
													 , ROI.get( ).y*GLOBALS::OtherH
													 , (ROI.get( ).z - ROI.get( ).x)*GLOBALS::OtherW
													 , (ROI.get( ).w - ROI.get( ).y)*GLOBALS::OtherH)
									   , depth_->getTypeName( ));
				bgRecorded = true;
				printf("%s" , "recorded BACKGROUND Image");
				//自动读取保存的参数
				{
					gui.loadFromFile("settings.xml");
					screenL.loadAABB(screenLUL, screenLUR, screenLDL, screenLDR);
					screenR.loadAABB(screenRUL, screenRUR, screenRDL, screenRDR);
					screenF.loadAABB(screenFUL, screenFUR, screenFDL, screenFDR);
					screenD.loadAABB(screenDUL, screenDUR, screenDDL, screenDDR);
					cout << "GUI loaded from settings.xml" << endl;
					ofSetBackgroundColor(bgColor);
					manDetectHelper.getPerspective().setXFlipped(imgXFlipped);
					manDetectHelper.getPerspective().setYFlipped(imgYFlipped);
					manDetectHelper.setDrawOffsetMax(lightOffsetMax.get());
					manDetectHelper.resetBG(depth_->getPixels());
				}
			}
			else
			{
				moveDetectHelper.updateParams(A1 , A2 , B1 , B2
											  , ofRectangle(ROI.get( ).x*GLOBALS::OtherW
															, ROI.get( ).y*GLOBALS::OtherH
															, (ROI.get( ).z - ROI.get( ).x)*GLOBALS::OtherW
															, (ROI.get( ).w - ROI.get( ).y)*GLOBALS::OtherH)
											  , moveScaleParam
				);
				moveDetectHelper.update(depth_->getPixels( ));
				manDetectHelper.update(thresh , erodeIter , dilateIter , finderMinRad , finderMaxRad
									   , finderThresh , finderMaxMove , 30 , doBlur , blurSize , depth_->getPixels( ));
				
				if (!bIntro && !bPlaying && manDetectHelper.Finder().checkHandTracked()) {
					firstFoundMan.setReady();
				}
			}
		}
	}
	// 更新 FboScreen 信息
	{
		screenL.setScreenPos(screenLPos.get( ));
		screenR.setScreenPos(screenRPos.get( ));
		screenF.setScreenPos(screenFPos.get( ));
		screenD.setScreenPos(screenDPos.get( ));
	}
	InputManager::Instance( ).sendTouches(manDetectHelper.getManPosition_Radius_s( ));
	InputManager::Instance( ).sendTouch(ofGetMouseX( ) , ofGetMouseY( ));//鼠标操作
	InputManager::Instance( ).sendMovement(moveDetectHelper.getColMoveAmounts( ));
	InputManager::Instance( ).sendMeanMove(moveDetectHelper.getMeanMove( ));
	auto c = frameColors[liveHelper.calcCurrentFrame(mainVideo.player.getPosition( ))];
	InputManager::Instance( ).sendTouch(c.r , c.g , c.b , InputType::LIGHT);
	SpellManager::Instance( ).batchUpdate( );//这个放在最后一行,  中间可能会有 input 事件产生

	//--------- 所有 Update 完后， 绘图到 FBO ------------------------------------------------------------
	logicScreen.begin( ); // 所有显示设备之和
	{
		ofClear(0);
		SpellManager::Instance( ).batchDraw( );
	}
	logicScreen.end( );
	screenF.begin( );
	{
		mainVideo.draw(0 , 0 , screenF.getScreenWidth( ) , screenF.getScreenWidth( ) * 9 / 16);
		videoIntro.draw(0, 0, screenF.getScreenWidth(), screenF.getScreenWidth() * 9 / 16);
		endVideos[endSongIndex].draw(0, 0, screenF.getScreenWidth(), screenF.getScreenWidth() * 9 / 16);
		
		//videoF.draw(0 , 0 , screenF.getScreenWidth( ) , screenF.getScreenWidth( ) * 9 / 16);
	}
	screenF.end( );
	screenL.begin( );
	{
		//ofClear(0);
		//auto modelL_viewport = modelL->getViewport( );
		logicScreen.getTexture()
			.drawSubsection(0, 0
				, screenL.getScreenWidth(), screenL.getScreenHeight()
				, 0,0);
		videoL.draw(0 , 0 , screenL.getScreenWidth( ) , screenL.getScreenHeight());
		countVideo.draw(0, 0, screenL.getScreenWidth(), screenL.getScreenHeight());
		
	}
	screenL.end( );
	screenR.begin( );
	{
		//ofClear(0);
		//auto modelR_viewport = modelR->getViewport( );
		logicScreen.getTexture()
			.drawSubsection(0, 0
				, screenR.getScreenWidth(), screenR.getScreenHeight()
				, screenL.getScreenWidth(), 0);
		videoR.draw(0 , 0 , screenR.getScreenWidth( ) , screenR.getScreenHeight( ));
		countVideo.draw(0, 0, screenR.getScreenWidth(), screenR.getScreenHeight() );
	}
	screenR.end( );
	screenD.begin( );
	{
		//ofClear(0);
		if(!bPlaying)
		{
			// intro 播放时
			if (bIntro) {
				//mandetecthelper 绘图
			}
			// 此时为播放歌曲或者倒计时  
			else {
				// 地上互动粒子系统 缓解无聊
			}
			manDetectHelper.draw( );
			// mandetecthelper没有找到人，  画扫描线  提醒进入场内
			if (!manDetectHelper.Finder().checkHandTracked()) {
				ofSetColor(0, 230, 92);
				scanLinePos.y -= ofGetLastFrameTime() * 300;
				scanLinePos.y = ofWrap(scanLinePos.y,0, screenD.getScreenHeight());
				ofDrawRectangle(0, scanLinePos.y, screenD.getScreenWidth(), 4);
				ofSetColor(255, 33, 33);
				ofDrawRectangle(0, 0, screenD.getScreenWidth() - 30, 6);
			}
		}
		else
		{
			logicScreen.getTexture( ).drawSubsection(0 , 0 , screenD.getScreenWidth( ) , screenD.getScreenHeight( )
													 , screenL.getScreenWidth( ) + screenR.getScreenWidth( ) , 0);
		}
	}
	screenD.end( );

	
}

void MainApp::draw( )
{
	if(bDrawLogicScreen)
	{
		logicScreen.draw(0 , 0);
	}
	else
	{
		screenL.draw( );
		screenR.draw( );
		screenF.draw( );
		screenD.draw( );
	}
	if(bDrawDebug)
	{
		ofDrawCircle(ofGetMouseX( ) , ofGetMouseY( ) , dragRad);
		screenL.drawDebug( );
		screenR.drawDebug( );
		screenF.drawDebug( );
		screenD.drawDebug( );
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
	//画出 hand投影范围 的 A1 A2 B1 B2
	{
		ofPushMatrix( );
		ofPushStyle( );
		ofFill( );
		ofTranslate(DEP_IMG_POS + manDetectHelper.getPerspective( ).getROI( ).getPosition( ));
		auto aabb = manDetectHelper.getPerspective( ).getAABB( );// aabb 长度只有 4 
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
void MainApp::exit( )
{
	for (int i = 0; i < END_SONG_NUM; i++) {
		endVideos[i].player.close();
	}
	videoIntro.player.close();
	mainVideo.player.close( );
	videoL.player.close( );
	videoR.player.close( );
	//videoF.player.close( );
	mainAudio.unload( );
	EasyFunction::SafeCloseKinect(kinect_);
}


void MainApp::keyPressed(int key)
{
	switch(key)
	{
		case VK_SPACE:
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
			ofSetBackgroundColor(bgColor);
			manDetectHelper.getPerspective( ).setXFlipped(imgXFlipped);
			manDetectHelper.getPerspective( ).setYFlipped(imgYFlipped);
			manDetectHelper.setDrawOffsetMax(lightOffsetMax.get( ));
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
			screenL.loadAABB(screenLUL , screenLUR , screenLDL , screenLDR);
			screenR.loadAABB(screenRUL , screenRUR , screenRDL , screenRDR);
			screenF.loadAABB(screenFUL , screenFUR , screenFDL , screenFDR);
			screenD.loadAABB(screenDUL , screenDUR , screenDDL , screenDDR);
			cout << "GUI loaded from settings.xml" << endl;
			break;
		case 'm':
			bDrawLogicScreen = !bDrawLogicScreen;
			break;
		case 'p':
			bPlaying = !bPlaying;
			break;
		case 'r':
			screenL.resetOutputSize(outputSizeL.get( ).x , outputSizeL.get( ).y);
			screenR.resetOutputSize(outputSizeR.get( ).x , outputSizeR.get( ).y);
			screenF.resetOutputSize(outputSizeF.get( ).x , outputSizeF.get( ).y);
			screenD.resetOutputSize(outputSizeD.get( ).x , outputSizeD.get( ).y);
			manDetectHelper.resetOutputSize(
				ofRectangle(0 , 0 , outputSizeD.get( ).x , outputSizeD.get( ).y)
			);
			break;
		case 's':
			isSpeedMode = !isSpeedMode;
			break;
		case 'v':
			break;
		case 'w':
			break;
		case '0':
			dragScreen = -1;//  表示同时可以拖动所有屏幕
			break;
		case '1':
		case '2':
		case '3':
		case '4':
			dragScreen = key - '1' ;
			break;
		case '5': A1.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
		case '6': A2.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
		case '7': B1.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
		case '8': B2.set(ofVec3f(ofGetMouseX( ) - DEP_IMG_POS.x , ofGetMouseY( ) - DEP_IMG_POS.y)); break;
	}
}

void MainApp::keyReleased(int key)
{
}

void MainApp::mouseMoved(int x , int y)
{
}

void MainApp::mouseDragged(int x , int y , int button)
{
	switch(dragScreen)
	{
		case -1:
			screenL.onMouseDragged(x , y , button);
			screenR.onMouseDragged(x , y , button);
			screenF.onMouseDragged(x , y , button);
			screenD.onMouseDragged(x , y , button);
			break;
		case 0:
			screenL.onMouseDragged(x , y , button);
			break;
		case 1:
			screenR.onMouseDragged(x , y , button);
			break;
		case 2:
			screenF.onMouseDragged(x , y , button);
			break;
		case 3:
			screenD.onMouseDragged(x , y , button);
			break;
	}
}

void MainApp::mousePressed(int x , int y , int button)
{
	switch(dragScreen)
	{
		case -1:
			screenL.onMouseDown(x , y , dragRad , button);
			screenR.onMouseDown(x , y , dragRad , button);
			screenF.onMouseDown(x , y , dragRad , button);
			screenD.onMouseDown(x , y , dragRad , button);
			break;
		case 0:
			screenL.onMouseDown(x , y , dragRad , button);
			break;
		case 1:
			screenR.onMouseDown(x , y , dragRad , button);
			break;
		case 2:
			screenF.onMouseDown(x , y , dragRad , button);
			break;
		case 3:
			screenD.onMouseDown(x , y , dragRad , button);
			break;
	}
}

void MainApp::mouseReleased(int x , int y , int button)
{
	switch(dragScreen)
	{
		case -1:
			screenL.onMouseUp(x , y , button);
			screenR.onMouseUp(x , y , button);
			screenF.onMouseUp(x , y , button);
			screenD.onMouseUp(x , y , button);
			break;
		case 0:
			screenL.onMouseUp(x , y , button);
			break;
		case 1:
			screenR.onMouseUp(x , y , button);
			break;
		case 2:
			screenF.onMouseUp(x , y , button);
			break;
		case 3:
			screenD.onMouseUp(x , y , button);
			break;
	}
	//更新屏幕四个角信息, 到GUI 方便之后保存到 xml 中
	screenFUL = screenF.getScreenUL( );
	screenLUL = screenL.getScreenUL( );
	screenRUL = screenR.getScreenUL( );
	screenDUL = screenD.getScreenUL( );

	screenFUR = screenF.getScreenUR( );
	screenLUR = screenL.getScreenUR( );
	screenRUR = screenR.getScreenUR( );
	screenDUR = screenD.getScreenUR( );

	screenFDL = screenF.getScreenDL( );
	screenLDL = screenL.getScreenDL( );
	screenRDL = screenR.getScreenDL( );
	screenDDL = screenD.getScreenDL( );

	screenFDR = screenF.getScreenDR( );
	screenLDR = screenL.getScreenDR( );
	screenRDR = screenR.getScreenDR( );
	screenDDR = screenD.getScreenDR( );

}


void MainApp::mouseScrolled(int x , int y , float scrollX , float scrollY)
{
}

