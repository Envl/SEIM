#pragma once
#include <cstddef>
#include <ofMain.h>
#include <ofMesh.h>
#include <ofVboMesh.h>
#include <ofxCv.h>
#include "SequenceImage.h"

class ofxHelper
{
private:
	ofxHelper();
	~ofxHelper();
	ofxHelper(const ofxHelper&);//这行不懂
	ofxHelper& operator=(const ofxHelper&);//这行也不懂

public:
	static ofxHelper& Instance()
	{
		static ofxHelper instance;
		return instance;
	}
	
	template<class C>
	static void ExchangeValue(C &a , C&b);

	static void loadFrameColors(char* path , vector<ofColor>& vec);

	static void LoadSequence(SequenceImage& sequence,char startName[] , int num);
	
	static ofMesh GenMesh(int gridRow , int gridColumn , int gridSize = 30 , ofPrimitiveMode MODE = OF_PRIMITIVE_TRIANGLES);// 水平与垂直格子数
	
	static ofVboMesh GenVboMesh(int gridRow , int gridColumn , int gridSizeW = 30 ,int gridSizeH=30, ofPrimitiveMode MODE = OF_PRIMITIVE_TRIANGLES);// 水平与垂直格子数
	
	static int Index4FPS(int fps , int frameAmount);
	
	static int GetClosestVertexIndex( int x,int y,vector<ofVec3f> vec);
	
	static void DrawVertices(vector<ofPoint> vertices,int radius=4,ofColor color=ofColor(222,33,33));
	
	static float HeartBeat( );
	
	static void MyThreshold( );
	
	static cv::Mat GetTransformMatrix(ofPoint A1,ofPoint A2,ofPoint B1,ofPoint B2
		,ofPoint dstA1,ofPoint dstA2,ofPoint dstB1,ofPoint dstB2);
	
	static ofVec3f transCoord(cv::Mat& transMat, ofPoint origin);
	
	/*----------- 
	maxValue 最大为 255   因为只支持 uchar  类型
	*/
	template <class SD>
	static void RangeExtract(SD& src_dst , float from , float to , int maxValue);
	
};

template<class C>
inline void ofxHelper::ExchangeValue(C & a , C & b)
{
	C tmp = a;
	a = b;
	b = tmp;
}

template <class SD>
void ofxHelper::RangeExtract(SD& src_dst, float from, float to, int maxValue)
{
	cv::Mat srcMat = toCv(src_dst);
	cv::threshold(srcMat , srcMat , to , maxValue , CV_THRESH_TOZERO_INV);
	cv::threshold(srcMat , srcMat , from , maxValue , CV_THRESH_BINARY);
}