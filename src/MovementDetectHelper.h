#pragma once
#include "PerspectiveAgent.h"
#include "ofxHelper.h"

using namespace ofxCv;



template<typename PixelType>
class MovementDetectHelper
{
public:
	void setup( ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2,ofRectangle roi,string sourceName );
	void update(PixelType pixels);
	void updateParams( ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2, ofRectangle roi,float scaleParam);
	ofFloatImage getDiffImage( );
	vector<float>& getColMoveAmounts( );
	float getMeanMove( );
private:
	PixelType roiPix;
	PerspectiveAgent perspective;
	string sourceName;
	ofImage diffFrame , lastFrame , newFrame;
	ofShortImage diff_short , last_short ;
	ofRectangle lastROI; //用来监测 ROI 是否改变
	cv::Mat colMoveAmount;// 每一 column 像素的 diff 总量 , 用来描述人群动作剧烈程度
	vector<float> colMoveAmount_f;
	ofFloatImage floatImg;// 用于观察的图像
	float meanMove = 0;
	float scaleParam = 1;
};

template <typename PixelType>
void MovementDetectHelper<PixelType>::setup( ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2,ofRectangle roi,string sourceName)
{
	floatImg.setImageType(ofImageType::OF_IMAGE_GRAYSCALE);
	this->sourceName = sourceName;
	perspective.setup(A1,A2,B1,B2 , roi);
}

template <typename PixelType>
void MovementDetectHelper<PixelType>::update(PixelType pixels)
{
	cv::Mat dst;
	perspective.applyROI(pixels , dst);
	toOf(dst , roiPix);
	//roiPix.resize(roiPix.getWidth( ) / 3 , roiPix.getHeight( ) / 3);// 降低分辨率 提速    有问题, 原因未知
	if(sourceName == "Color")
	{
		convertColor(roiPix ,newFrame, CV_RGBA2GRAY);
		/*
		absdiff 会将尺寸小的图片变成和较大的同一尺寸.
		lastFrame 的更新在 absdiff 后. 所以当 roi 改变后,要趁 absdiff
		之前 , applyROI 到 lastFrame 上*/
		if(!lastFrame.isAllocated( )|| perspective.getROI( ) != lastROI)
		{
			lastFrame = newFrame;
			lastROI = perspective.getROI( );
		}
		//缩小尺寸提速
		resize(lastFrame , lastFrame , 0.6 , 0.6);
		resize(newFrame , newFrame , 0.6 , 0.6);
		absdiff(lastFrame , newFrame , diffFrame);
		lastFrame = newFrame;
		colMoveAmount = meanCols(diffFrame);
		colMoveAmount_f.clear( );
		for(int i = 0; i<colMoveAmount.rows; i++)
		{
			// 0~255  ---> 0~1
			colMoveAmount_f.push_back(colMoveAmount.at<uchar>(i)/255.0f);// range 0~255  因为是 ofImage
		}
		
	}
	else// both depth  and  ir  are      ofShortPixels
	{
		/*
		absdiff 会将尺寸小的图片变成和较大的同一尺寸.
		lastFrame 的更新在 absdiff 后. 所以当 roi 改变后,要趁 absdiff
		之前 , applyROI 到 lastFrame 上*/
		if(!last_short.isAllocated()|| perspective.getROI( ) != lastROI)
		{
			last_short = roiPix;
			lastROI = perspective.getROI( );
		}
		absdiff(last_short , roiPix , diff_short); // !!!! abddiff 会把较小的图变成和大图同尺寸
		last_short = roiPix;
		colMoveAmount = meanCols(diff_short);
		colMoveAmount_f.clear( );
		for(int i = 0; i<colMoveAmount.rows; i++)
		{
			// 0~65535---> 0~1
			auto amount_f = colMoveAmount.at<ushort>(i)
				/ 65535.0f;// range 0~65535  因为是 shortImage
			colMoveAmount_f.push_back(ofClamp(
			amount_f*scaleParam,0,1)// scaleParam 缩放系数
				);
		}
	}
	// 计算 meanmove
	for(int i = 0; i < colMoveAmount_f.size( ); i++)
	{
		meanMove += colMoveAmount_f[i];
	}
	if(colMoveAmount_f.size( ) > 0)
	{
		meanMove /= colMoveAmount_f.size( );
	}
}

template <typename PixelType>
void MovementDetectHelper<PixelType>::updateParams(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi,float scaleParam)
{
	perspective.updateParams(A1,A2,B1,B2, roi);
	this->scaleParam = scaleParam;
}

template <typename PixelType>
ofFloatImage  MovementDetectHelper<PixelType>::getDiffImage()
{
	if(sourceName=="Color")
	{
		copy(diffFrame , floatImg);
	}
	else
	{
		copy(diff_short , floatImg);
	}
	floatImg.update( );
	return floatImg;
}

template <typename PixelType>
vector<float>& MovementDetectHelper<PixelType>::getColMoveAmounts()
{
	return colMoveAmount_f;
}

template<typename PixelType>
inline float MovementDetectHelper<PixelType>::getMeanMove( )
{
	
	return meanMove;
}
