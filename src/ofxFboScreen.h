#pragma once
#include <ofVboMesh.h>
#include <ofFbo.h>
#include <ofMatrixStack.h>
#include <ofxCv.h>

enum ScreenType
{
	SinglePlane ,
	DoublePlane
};
using namespace ofxCv;
class ofxFboScreen
{
private:
	ScreenType type;
	ofVboMesh mesh;
	vector<cv::Point2f> originVerts;
	ofFbo fbo;
	ofFbo fbo2;
	bool dragAble = false;
	bool bDrawDebug = false;
	int dragVertIndex = 0;
	int gridRow , gridCol;
	//	ofMatrix4x4 ofTransMat;
	cv::Mat transMatL , transMatR;
	ofPoint A1 , A2 , B1 , B2 , MidU , MidD;// 记录原始矩形四个顶点的坐标
	int indexA1 , indexA2 , indexB1 , indexB2;// 四个顶点的 index
	int indexMidU , indexMidD;//左右异面的上下分界点的 index
	ofPoint screenPos;
	ofPoint screenFlip = ofPoint(1 , 1);

	int GetClosestVertexIndex(int mouseX , int mouseY , vector<ofVec3f> & vec);
	void moveVertices( );
	ofPoint transCoord(ofPoint origin);
	//	cv::Mat calcTransMat();
	cv::Mat GetTransformMatrix(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2
							   , ofPoint dstA1 , ofPoint dstA2 , ofPoint dstB1 , ofPoint dstB2);

public:
	void setup(int width , int height, int gridRow = 1 , int gridCol = 1 , ScreenType type = SinglePlane , ofPoint pos = ofPoint(0 , 0));
	void begin( );
	void end( );
	void draw();
	void onMouseDown(int x , int y , int affectRadius , int btn = 0);
	void onMouseUp(int x , int y , int btn = 0);
	void onMouseDragged(int x , int y , int btn = 0);
	void resetOutputSize(int w , int h);
	void drawDebug( );
	void setScreenPos(ofPoint pos);
	void setXflip(bool flag);
	void setYflip(bool flag);
	int getScreenWidth( );
	int getScreenHeight( );
	
	ofPoint getScreenUL( );
	ofPoint getScreenUR( );
	ofPoint getScreenDL( );
	ofPoint getScreenDR( );
	void loadAABB(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2);
};
