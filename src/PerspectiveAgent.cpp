#include "PerspectiveAgent.h"

void PerspectiveAgent::calcTransMat( )
{
	cout << "calced transmat" << endl;
	cv::Point2f outputCorners[4] = { cv::Point2f(outputSize.getLeft( ),outputSize.getTop( ))
		,cv::Point2f(outputSize.getRight( ),outputSize.getTop( ))
		,cv::Point2f(outputSize.getLeft( ),outputSize.getBottom( ))
		,cv::Point2f(outputSize.getRight( ),outputSize.getBottom( )) };
	cv::Point2f aabb_[4];
	for(int i = 0; i<4; i++)
	{
		aabb_[i].x = AABB[i].x;
		aabb_[i].y = AABB[i].y;
		cout << "AABB" << i << ": " << AABB[i] << "    ";
	}
	cout <<endl<< "ROI " << endl << ROI.x << " " << ROI.y << " " << ROI.width << " " << ROI.height << endl;

	transMat = cv::getPerspectiveTransform(aabb_ , outputCorners);
}

void PerspectiveAgent::setup(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi)
{
	updateParams(A1 , A2 , B1 , B2 , roi);
}

void PerspectiveAgent::updateParams(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofRectangle roi)
{
	bool aabbChanged = originAABB[0] != A1 || originAABB[1] != A2
		|| originAABB[2] != B1 || originAABB[3] != B2;
	bool roiChanged = ROI != roi;

	originAABB[0] = A1;
	originAABB[1] = A2;
	originAABB[2] = B1;
	originAABB[3] = B2;

	ROI = roi;
	AABB[0] = A1;
	AABB[1] = A2;
	AABB[2] = B1;
	AABB[3] = B2;
	//ROI 修复
	ROI.x = ROI.x < 0 ? 0 : ROI.x;
	ROI.y = ROI.y < 0 ? 0 : ROI.y;
	ROI.width = ROI.getRight( ) < ROI.getLeft( ) ? 1 : ROI.width;
	ROI.height = ROI.getBottom( ) < ROI.getTop( ) ? 1 : ROI.height;

	roiRect = cv::Rect(ROI.x , ROI.y , ROI.width , ROI.height);
	//AABB  修复
	for(int i = 0; i<4; i++)
	{
		//左右越界修复
		AABB[i].x = ofMap(AABB[i].x , ROI.getLeft( ) , ROI.getRight( ) , ROI.getLeft( ) , ROI.getRight( ) , true);
		//上下越界修复
		AABB[i].y = ofMap(AABB[i].y , ROI.getTop( ) , ROI.getBottom( ) , ROI.getTop( ) , ROI.getBottom( ) , true);
		//AABB转换为相对于 ROI 左上角的坐标
		AABB[i].set(AABB[i].x - ROI.x , AABB[i].y - ROI.y);
	}
	if(roiChanged||aabbChanged)
	{
		calcTransMat( );
	}
}

// getRectSubPix 似乎只支持像素值最大 255 的图像
void PerspectiveAgent::undistortImage(cv::Mat& src , cv::Mat& dst)
{
	// 截取 ROI , 在原有内存基础上建立 Mat 的 header ,不会 重新分配内存
	cv::Mat roiMat;
	cv::getRectSubPix(src , cv::Size(ROI.width , ROI.height)
					  , cv::Point2f(ROI.getCenter( ).x , ROI.getCenter( ).y) , roiMat);
	warpPerspective(roiMat , dst , transMat);
}

void PerspectiveAgent::applyROI(cv::Mat& src, cv::Mat& dst)
{
	dst = src(roiRect).clone( );
}

void PerspectiveAgent::transCoords(vector<cv::Point2f>& in, vector<cv::Point2f>& out)
{
	cv::perspectiveTransform(in , out , transMat);
}

ofPoint PerspectiveAgent::transCoord(ofVec3f origin)
{
	if(!ROI.inside(origin+ROI.getTopLeft())
		&& GLOBALS::isDebug)// origin 是相对 ROI 左上角的坐标
	{
		cout << "Point: " << origin << " not in ROI" << "  And ROI is: "
			<<ROI.x<<" "<<ROI.y<<" "<<ROI.width<<" "<<ROI.height<<endl;
	}
	vector<cv::Point2f> in(1) , out;
	in[0] = cv::Point2f(origin.x , origin.y);
	cv::perspectiveTransform(in , out , transMat);
	out[0].x = xFlipped ? outputSize.width - out[0].x : out[0].x;
	out[0].y = yFlipped ? outputSize.width - out[0].y : out[0].y;
	return ofPoint(out[0].x , out[0].y);
}

void PerspectiveAgent::resetOutputSize(ofRectangle outputSize)
{
	this->outputSize = outputSize;
	calcTransMat( );
}


PerspectiveAgent::PerspectiveAgent(ofRectangle outputSize)
{
	resetOutputSize(outputSize);
}

ofRectangle PerspectiveAgent::getOutputSize( ) const
{
	return outputSize;
}

ofRectangle PerspectiveAgent::getROI()
{
	return ROI;
}

ofPoint* PerspectiveAgent::getAABB()
{
	return AABB;
}

bool PerspectiveAgent::getXFlipped() const
{
	return xFlipped;
}

bool PerspectiveAgent::getYFlipped() const
{
	return yFlipped;
}

void PerspectiveAgent::setXFlipped(bool flag)
{
	xFlipped = flag;
}

void PerspectiveAgent::setYFlipped(bool flag)
{
	yFlipped = flag;
}
