#pragma once
#include "objFinder.h"
#include "PerspectiveAgent.h"
#include "SmoothVec3Filter.h"


class FollowRing:public  RectFollower
{
private:
	float DIE_DURATION = 1.5f;
protected:
	ofVec2f  smoothedPos;
	float startDieTime;
	float radius;
public:
	FollowRing( ) :startDieTime(0) , radius(30)
	{
	}

	void setup(const cv::Rect& track) override;
	void update(const cv::Rect& track) override;
	void kill( ) override;
	void draw( int x,int y,int z=0,ofColor c=ofColor(100));
	void draw(ofVec3f pos,ofColor c=ofColor(100));
	ofVec2f Pos( ) ;
};

template<typename PixelType>
class ManDetectHelper
{
private:
	bool bUseTracker = true;
	bool bFindFirstMan = false;
	bool lastFrameFoundMan = false;
	float startNoManTime = 0;// 开始没人时的时间点
	ofVec3f lightOffsetMax;
	ObjFinder<PixelType> finder;
	RectTrackerFollower<FollowRing> tracker;
	PerspectiveAgent perspective;
	PixelType roiPix;
	SmoothVec3Filter handPosFilter = SmoothVec3Filter(6);// size = 6
	vector<ofVec3f> manPositions;
public:
	ObjFinder<PixelType>& Finder( ) ;
	float getNoManTime( );
	void setUseTracker(bool flag);
	void setToNoMan( ); 
	void draw(int x=0,int y=0 );
	void setup(PixelType& pixels , ofPoint threshRange , ofRectangle outputSize
			   , string sourceName , bool findContour = true);
	void setupPerspectiveParams(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi);
	void resetBG(PixelType& pixels);
	void update(ofPoint thresh , int erodeIter , int dilateIter
				, int minAreaRad , int maxAreaRad , int countourThresh
				, int contourMaxMove , int contourPersistence
				, bool doBlur , int blurSize , ofShortPixels pix);
	void updatePerspectiveParams(ofPoint a1 , ofPoint a2 , ofPoint b1 , ofPoint b2 , ofRectangle roi);
	void setXYFlipped(bool x , bool y);
	void resetOutputSize(ofRectangle szie);
	void setDrawOffsetMax(ofVec3f offset);
	PerspectiveAgent & getPerspective( );
	vector<ofVec3f>& getManPosition_Radius_s( );
};


template <typename PixelType>
ObjFinder<PixelType>& ManDetectHelper<PixelType>::Finder() 
{
	return finder;
}

template<typename PixelType>
inline float ManDetectHelper<PixelType>::getNoManTime( )
{
	return ofGetElapsedTimef( ) - startNoManTime;
}

template<typename PixelType>
inline void ManDetectHelper<PixelType>::setUseTracker(bool flag)
{
	bUseTracker = flag;
}

template<typename PixelType>
inline void ManDetectHelper<PixelType>::setToNoMan( )
{
	bFindFirstMan = false;
}

template <typename PixelType>
void ManDetectHelper<PixelType>::draw(int x,int y)
{
	if(!bUseTracker)
	{
		return;
	}
	ofPushMatrix();
	ofPushStyle( );
	ofTranslate(x , y);
	for(auto &r:tracker.getFollowers())
	{
		ofColor c;
		c.setHsb(
			ofMap(perspective.transCoord(r.Pos( )).x
				  , 0
				  , perspective.getOutputSize( ).width
				  , 0
				  , 255)
			, 188 , 255
		);
		auto outputHeight = perspective.getOutputSize( ).getHeight( );
		auto targetPos = perspective.transCoord(r.Pos( ));
		targetPos.y += lightOffsetMax.y;// *targetPos.y / outputHeight;
		targetPos.z = lightOffsetMax.z;
		targetPos.x += lightOffsetMax.x;
		r.draw(targetPos , c);
	}
	ofPopStyle( );
	ofPopMatrix( );
}

template <typename PixelType>
void ManDetectHelper<PixelType>::setup(PixelType& pixels, ofPoint threshRange, ofRectangle outputSize, string sourceName, bool findContour)
{
	perspective.resetOutputSize(outputSize);
	cv::Mat dst;
	perspective.applyROI(pixels , dst);
	toOf(dst , roiPix);
	finder.setup(roiPix , threshRange , sourceName , findContour);
	if(bUseTracker)
	{
		tracker.setPersistence(0);
		tracker.setMaximumDistance(50);
	}
}

template <typename PixelType>
void ManDetectHelper<PixelType>::setupPerspectiveParams(ofPoint A1, ofPoint A2, ofPoint B1, ofPoint B2, ofRectangle roi)
{
	perspective.setup(A1 , A2 , B1 , B2 , roi);
}

template <typename PixelType>
void ManDetectHelper<PixelType>::resetBG(PixelType& pixels)
{
	cv::Mat dst;
	perspective.applyROI(pixels , dst);
	toOf(dst , roiPix);
	finder.resetBG(roiPix);
}

template <typename PixelType>
void ManDetectHelper<PixelType>::update(ofPoint thresh, int erodeIter, int dilateIter, int minAreaRad, int maxAreaRad, int countourThresh, int contourMaxMove, int contourPersistence, bool doBlur, int blurSize, ofShortPixels pix)
{
	lastFrameFoundMan = finder.checkHandTracked( );//上一次是否找到 人
	finder.updateParams(thresh , erodeIter , dilateIter , minAreaRad ,    // 1ms
							maxAreaRad , countourThresh , contourMaxMove , contourPersistence
							, doBlur , blurSize);
	cv::Mat dst;
	perspective.applyROI(pix , dst);
	toOf(dst , roiPix);
	finder.update(roiPix);
	// 保存透视变换后的位置
	manPositions.clear( );
	if(bUseTracker)
	{
		tracker.track(finder.getFinder( ).getBoundingRects( ));

		for(auto &r : tracker.getFollowers( ))
		{
			auto pos = perspective.transCoord(r.Pos( ));
			manPositions.push_back(pos);
		}
	}
	else
	{
		auto contourF = finder.getFinder( );
		for(int i = 0; i < contourF.getBoundingRects( ).size( ); i++)
		{
			auto cvPos = contourF.getCenter(i);
			auto p=perspective.transCoord(ofPoint(cvPos.x , cvPos.y));
			manPositions.push_back(p);
		}
	}
	//首次找到人
	if(finder.checkHandTracked( ) && !lastFrameFoundMan)
	{
		bFindFirstMan = true;
	}
	//从这一帧开始没找到人
	if(!finder.checkHandTracked() && lastFrameFoundMan){
		startNoManTime = ofGetElapsedTimef( );
	}
}

template <typename PixelType>
void ManDetectHelper<PixelType>::updatePerspectiveParams(ofPoint a1, ofPoint a2, ofPoint b1, ofPoint b2, ofRectangle roi)
{
	perspective.updateParams(a1 , a2 , b1 , b2 , roi);
}

template <typename PixelType>
void ManDetectHelper<PixelType>::setXYFlipped(bool x, bool y)
{
	perspective.setXFlipped(x);
	perspective.setYFlipped(y);
}

template<typename PixelType>
inline void ManDetectHelper<PixelType>::resetOutputSize(ofRectangle size)
{
	perspective.resetOutputSize(size);
}

template<typename PixelType>
inline void ManDetectHelper<PixelType>::setDrawOffsetMax(ofVec3f offset)
{
	lightOffsetMax = offset;
}

template<typename PixelType>
inline PerspectiveAgent & ManDetectHelper<PixelType>::getPerspective( )
{
	return perspective;
}

template<typename PixelType>
inline vector<ofVec3f>& ManDetectHelper<PixelType>::getManPosition_Radius_s( )
{
	return manPositions;
}
