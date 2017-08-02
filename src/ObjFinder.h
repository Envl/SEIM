#pragma once
#include <ofImage.h>
#include "ofxCv.h"
#include "GLOBALS.h"
#include "ofxHelper.h"

/*------------------------------------------------
这里模版的用法不对   有空再用模版写  --
------------------------------------------------*/
using namespace ofxCv;
template<typename  PixelType>
class ObjFinder
{
private:
	ContourFinder contourFinder;
	ofShortImage bg_short , diff_short;
	ofImage bg_img , diff_img , newFrame_img;
	string sourceName;
	ofFloatImage diff_float;
	ofImage objImg;
	ofPoint thresholdRange;
//	ofVec4f roi=ofVec4f(0,0,1,1);
	ofPoint thresh = ofPoint(0 , 0.5f);
	int erodeIter =1, dilateIter=1;
	int blurSize=3;
	bool doBlur=false;
	bool findContour = true;

	ofRectangle getRectInImg(int index);
	int getHandIndex( );

public:
	enum DataType
	{
		Color,
		Depth,
		IR
	};

	inline void updateParams(ofPoint thresh , int erodeIter , int dilateIter , int minAreaRad
							 , int maxAreaRad , int countourThresh , int contourMaxMove , int contourPersistence
							 , bool doBlur , int blurSize);
	
	inline ofImage* getObjImage( ) 
	{
		return &objImg;
	}

	inline void setup(PixelType& pixels , ofPoint threshRange ,string sourceName, bool findContour = true);
	inline void resetBG(PixelType pixels);
	inline void update(PixelType& pixels);
	ContourFinder getFinder( ) const;
	bool checkHandTracked() const
	{
		return contourFinder.size( ) > 0;
	}
	ofRectangle getHand( );
	ofPolyline getHandFitQuad( );
	ofVec3f getHandPos( );
};

template <typename PixelType>
ContourFinder ObjFinder<PixelType>::getFinder() const
{
	return contourFinder;
}

template <typename PixelType>
ofRectangle ObjFinder<PixelType>::getHand()
{
	return getRectInImg(getHandIndex());
}

template <typename PixelType>
ofPolyline ObjFinder<PixelType>::getHandFitQuad()
{
	auto cvQuad = contourFinder.getFitQuad(getHandIndex( ));
	ofPolyline quad;
	for(int i=0;i<cvQuad.size();i++)
	{
		quad.addVertex(ofPoint(cvQuad[i].x , cvQuad[i].y));
	}
	quad.close( );
	return quad;
}

template <typename PixelType>
ofVec3f ObjFinder<PixelType>::getHandPos()
{
	auto d = getHand( );
	return ofVec3f(d.x + d.width / 2
					, d.y + d.height / 2 //center (x,y)
					, d.width<d.height ? d.width / 2 : d.height / 2); // radius
}

template <typename PixelType >
ofRectangle ObjFinder<PixelType >::getRectInImg(int index)
{
	assert(index < contourFinder.size( ) && index >= 0);
	auto rect = toOf(contourFinder.getBoundingRect(index));
	return rect;
//	return ofRectangle(roi.x*GLOBALS::OtherW + rect.x
//					   , roi.y*GLOBALS::OtherH + rect.y
//					   , rect.width , rect.height);
}

template <typename PixelType>
int ObjFinder<PixelType>::getHandIndex()
{
	int maxArea = 0;
	int maxID = -1 , lastMax = -1;
	int secondID = -1;
	for(int i = 0; i<contourFinder.size( ); i++)
	{
		int area = contourFinder.getBoundingRect(i).area( );
		lastMax = maxID;
		maxID = area > maxArea ? i : maxID;
		maxArea = area > maxArea ? area : maxArea;
		// maxID 改变了,说明 lastMax 变成第二大的了
		secondID = lastMax == maxID ? secondID : lastMax;
	}
	//	if(secondID != -1)
//	{
		//return getRectInImg(secondID);
	//}
	if(maxID!=-1)
	{
		return maxID;
	}
}

template <typename PixelType >
void ObjFinder<PixelType>::updateParams(ofPoint thresh, int erodeIter, int dilateIter
										, int minAreaRad, int maxAreaRad, int countourThresh
										, int contourMaxMove, int contourPersistence
										, bool doBlur, int blurSize)
{
	this->thresh.x = ofLerp(thresholdRange.x , thresholdRange.y , thresh.x*thresh.z);
	this->thresh.y = ofLerp(thresholdRange.x , thresholdRange.y , thresh.y*thresh.z);
	this->erodeIter = erodeIter;
	this->dilateIter = dilateIter;
	this->doBlur = doBlur;
	this->blurSize = blurSize;

	contourFinder.setMinAreaRadius(minAreaRad);
	contourFinder.setMaxAreaRadius(maxAreaRad);
	contourFinder.setThreshold(countourThresh);
	contourFinder.getTracker( ).setMaximumDistance(contourMaxMove);
	contourFinder.getTracker( ).setPersistence(contourPersistence);

}

template <typename PixelType>
void ObjFinder<PixelType >::setup(PixelType& pixels, ofPoint threshRange
								  ,string sourceName, bool findContour)
{
	
	this->findContour = findContour;
	bg_img.allocate(GLOBALS::ColorW , GLOBALS::ColorH , OF_IMAGE_GRAYSCALE);
	newFrame_img.allocate(GLOBALS::ColorW , GLOBALS::ColorH , OF_IMAGE_GRAYSCALE);

	this->sourceName = sourceName;
	if(sourceName == "Color")
	{
		convertColor(pixels , bg_img , CV_RGBA2GRAY);
//		bg_img.crop(roi.x*GLOBALS::ColorW , roi.y*GLOBALS::ColorH
//					, (roi.z - roi.x)*GLOBALS::ColorW , (roi.w - roi.y)*GLOBALS::ColorH);
	}
	bg_short.setFromPixels(pixels);
//	bg_short.crop(roi.x*GLOBALS::OtherW , roi.y*GLOBALS::OtherH
//				  , (roi.z - roi.x)*GLOBALS::OtherW , (roi.w - roi.y)*GLOBALS::OtherH);
	thresholdRange = threshRange;

}

template <typename PixelType>
void ObjFinder<PixelType >::resetBG(PixelType pixels)
{
//	this->roi = roi;
	if(sourceName == "Color")
	{
		convertColor(pixels , bg_img , CV_RGBA2GRAY);
//		bg_img.crop(roi.x*GLOBALS::ColorW , roi.y*GLOBALS::ColorH
//					, (roi.z - roi.x)*GLOBALS::ColorW , (roi.w - roi.y)*GLOBALS::ColorH);
	}
	else
	{
		bg_short.setFromPixels(pixels);
//		bg_short.crop(roi.x*GLOBALS::OtherW , roi.y*GLOBALS::OtherH
//					  , (roi.z - roi.x)*GLOBALS::OtherW , (roi.w - roi.y)*GLOBALS::OtherH);
	}
}

template <typename PixelType>
void ObjFinder<PixelType>::update(PixelType& pixels)
{

	if(sourceName == "Color")
	{
		convertColor(pixels , newFrame_img , CV_RGBA2GRAY);
//		newFrame_img.crop(roi.x*GLOBALS::ColorW , roi.y*GLOBALS::ColorH
//						  , (roi.z - roi.x)*GLOBALS::ColorW , (roi.w - roi.y)*GLOBALS::ColorH);
		absdiff(bg_img , newFrame_img , diff_img);
		//范围提取并二值化
		ofxHelper::RangeExtract(diff_img , thresh.x * 255 , thresh.y * 255 , 255);
		//denoise
		erode(diff_img , static_cast<int>(erodeIter));
		dilate(diff_img , static_cast<int>(dilateIter));
		objImg = diff_img;
		objImg.update( );
		if(findContour)
		{
			contourFinder.findContours(diff_img);
		}
	}
	else
	{
		/*--------------------
		可用 LUT 优化这一段
		主要是转换 float 耗时,
		threshold 不支持 >255 的数值所以得转换 float
		--------------------------*/
		absdiff(bg_short , pixels , diff_short);
		if(doBlur)
		{
			blur(diff_short , blurSize);
		}
//		diff_float.setFromPixels(diff_short.getPixels( ));//0---66535 --->  0--1
		copy(diff_short , diff_float);//0---66535 --->  0--1
		 //范围提取并二值化
		ofxHelper::RangeExtract(diff_float , thresh.x  , thresh.y  , 1);
		//去噪声
		erode(diff_float , static_cast<int>(erodeIter));
		dilate(diff_float , static_cast<int>(dilateIter));
		objImg.setFromPixels(diff_float.getPixels( ));//0--1  ----> 0--255  这里似乎不能用 copy
		if(findContour)
		{
			contourFinder.findContours(objImg);
		}

	}

}
