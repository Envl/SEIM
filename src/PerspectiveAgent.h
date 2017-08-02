#pragma once
#include <ofRectangle.h>
#include <ofPixels.h>
#include "ofxCv.h"
#include "GLOBALS.h"
using namespace ofxCv;
class PerspectiveAgent
{
	///////外部传进来的 ROI 和 AABB 的坐标都是相对于原始图像的左上角而言
	ofRectangle ROI = ofRectangle(0 , 0 , 1.0f , 1.0f);// unit:  pixel
	cv::Rect roiRect = cv::Rect(0 , 0 , 1 , 1);
	ofPoint AABB[4] = { ofPoint(0,0),ofPoint(1,0),ofPoint(0,1),ofPoint(1,1) };// unit: pixel
	ofPoint originAABB[4] = { ofPoint(0,0),ofPoint(1,0),ofPoint(0,1),ofPoint(1,1) };// unit pixel
	ofRectangle outputSize = ofRectangle(0 , 0 , 100 , 100);
//	cv::Size patchSize = cv::Size(1 , 1);
//	cv::Point2f center = cv::Point2f(1 , 1);
	cv::Mat transMat;
	bool xFlipped = false;
	bool yFlipped = false;
	void calcTransMat( );

public:
	PerspectiveAgent( ) = default;
	PerspectiveAgent(ofRectangle outputSize);
	ofRectangle getOutputSize( ) const;
	ofRectangle getROI( );
	ofPoint*  getAABB( );
	bool getXFlipped( ) const;
	bool getYFlipped( ) const;
	void setXFlipped(bool flag);
	void setYFlipped(bool flag);
	void resetOutputSize(ofRectangle  outputSize);
	void setup(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi = ofRectangle(0 , 0 , 1 , 1));
	void updateParams(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi = ofRectangle(0 , 0 , 1 , 1));
	void undistortImage(cv::Mat& src , cv::Mat& dst);
	void transCoords(vector<cv::Point2f>& in , vector<cv::Point2f>& out );
	ofPoint transCoord(ofVec3f origin);
	void applyROI(cv::Mat& src , cv::Mat& dst);
	template<class T>
	inline void applyROI(ofPixels_<T>& src, cv::Mat& dstMat);//有空研究如何把这个 mat 改成 pixels_ 现在有问题
};

template <class T>
void PerspectiveAgent::applyROI(ofPixels_<T>& src, cv::Mat& dstMat)
{
	cv::Mat srcMat = toCv(src);
	dstMat = srcMat(roiRect).clone();
}

