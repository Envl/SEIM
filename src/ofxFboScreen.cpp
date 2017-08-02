#include "ofxFboScreen.h"


void ofxFboScreen::moveVertices( )
{
	vector<cv::Point2f> newPos;
	cv::perspectiveTransform(originVerts , newPos , transMatL);
	if(type == SinglePlane)
	{
		for(int i = 0; i < mesh.getNumVertices( ); i++)
		{
			mesh.setVertex(i , ofVec3f(newPos[i].x , newPos[i].y));
		}
		return;
	}
	// L part
	for(int i = 0; i < mesh.getNumVertices( ); i++)
	{
		if(i % (gridCol + 1) <= indexMidU)// indexMidU 指向的 vertex 在第一排
		{
			mesh.setVertex(i , ofVec3f(newPos[i].x , newPos[i].y));
		}
	}
	// R part
	cv::perspectiveTransform(originVerts , newPos , transMatR);
	for(int i = 0; i < mesh.getNumVertices( ); i++)
	{
		if(i % (gridCol + 1)>indexMidU)
			mesh.setVertex(i , ofVec3f(newPos[i].x , newPos[i].y));
	}
}

// 未用到
ofPoint ofxFboScreen::transCoord(ofPoint origin)
{
	vector<cv::Point2f> in(1) , out;
	in[0] = cv::Point2f(origin.x , origin.y);
	cv::perspectiveTransform(in , out , transMatL);
	return ofPoint(out[0].x , out[0].y);
}


cv::Mat ofxFboScreen::GetTransformMatrix(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2 , ofPoint dstA1 , ofPoint dstA2 , ofPoint dstB1 , ofPoint dstB2)
{
	cv::Point2f in[] = { cv::Point2f(A1.x,A1.y)
		,cv::Point2f(A2.x,A2.y)
		,cv::Point2f(B1.x,B1.y)
		,cv::Point2f(B2.x,B2.y) };
	cv::Point2f out[] = { cv::Point2f(dstA1.x,dstA1.y)
		,cv::Point2f(dstA2.x,dstA2.y)
		,cv::Point2f(dstB1.x,dstB1.y)
		,cv::Point2f(dstB2.x,dstB2.y) };
	return cv::getPerspectiveTransform(in , out);
}

//cv::Mat ofxFboScreen::calcTransMat()
//{
//	
//	
////	cv::Mat cvTransMat=GetTransformMatrix(A1,)
//	{
//		//此方案有问题 以后再说
////		ofTransMat.getPtr( )[0] = cvTransMat.at<float>(0);
////		ofTransMat.getPtr( )[4] = cvTransMat.at<float>(1);
////		ofTransMat.getPtr( )[12] = cvTransMat.at<float>(2);
////	
////		ofTransMat.getPtr( )[1] = cvTransMat.at<float>(3);
////		ofTransMat.getPtr( )[5] = cvTransMat.at<float>(4);
////		ofTransMat.getPtr( )[13] = cvTransMat.at<float>(5);
////	
////		ofTransMat.getPtr( )[3] = cvTransMat.at<float>(6);
////		ofTransMat.getPtr( )[7] = cvTransMat.at<float>(7);
////		ofTransMat.getPtr( )[15] = cvTransMat.at<float>(8);
//	}
//
//	return cvTransMat;
//}

void ofxFboScreen::setup(int width , int height , int gridRow , int gridCol , ScreenType type , ofPoint pos)
{
	ofDisableArbTex( );
	screenPos = pos;
	this->type = type;
	fbo.allocate(width , height);
	fbo2.allocate(width , height);
	this->gridRow = gridRow;
	this->gridCol = gridCol;
	int gridSizeH = height / gridRow;
	int gridSizeW = width / gridCol;
	//add vertices
	for(int row = 0; row < gridRow + 1; row++)
	{
		for(int col = 0; col < gridCol + 1; col++)
		{
			auto vert = ofVec3f(col*gridSizeW , row*gridSizeH , 0);
			mesh.addVertex(vert);//  colNum*row+col=index
			originVerts.push_back(cv::Point2f(vert.x , vert.y));
			mesh.addTexCoord(ofVec2f(static_cast<float>(col) / gridCol
									 , static_cast<float>(row) / gridRow
			));//使用 normalized TexCoord 
		}
	}
	//add indices
	for(int row = 0; row<gridRow; row++)
	{
		for(int col = 0; col<gridCol; col++)
		{
			mesh.addTriangle(col + row*(gridCol + 1) ,
							 col + 1 + row*(gridCol + 1) ,
							 col + (row + 1)*(gridCol + 1));
			//lower right
			mesh.addTriangle(col + 1 + row*(gridCol + 1) ,
							 col + 1 + (row + 1)*(gridCol + 1) ,
							 col + (row + 1)*(gridCol + 1));
		}
	}

	indexA1 = 0;
	indexA2 = gridCol;
	indexB1 = (gridCol + 1)*gridRow;
	indexB2 = (gridCol + 1)*(gridRow + 1) - 1;
	indexMidU = indexA2;
	indexMidD = indexB2;
	A1 = mesh.getVertex(indexA1);
	A2 = mesh.getVertex(indexA2);
	B1 = mesh.getVertex(indexB1);
	B2 = mesh.getVertex(indexB2);
	MidU = mesh.getVertex(indexMidU);
	MidD = mesh.getVertex(indexMidD);
}

void ofxFboScreen::drawDebug( )
{
	ofPushMatrix( );
	ofPushStyle( );
	ofTranslate(screenPos);
	ofSetColor(255);
	mesh.drawWireframe( );
	ofSetColor(ofColor::fromHsb(233 , 255 , 255));
	ofDrawCircle(mesh.getVertex(indexA1) , 22);
	ofDrawCircle(mesh.getVertex(indexA2) , 22);
	ofDrawCircle(mesh.getVertex(indexB1) , 22);
	ofDrawCircle(mesh.getVertex(indexB2) , 22);
	ofDrawCircle(mesh.getVertex(indexMidD) , 22);
	ofDrawCircle(mesh.getVertex(indexMidU) , 22);
	ofPopStyle( );
	ofPopMatrix( );
}

void ofxFboScreen::setScreenPos(ofPoint pos)
{
	screenPos = pos;
}

void ofxFboScreen::setXflip(bool flag)
{
	screenFlip.x = flag ? -1 : 1;
}

void ofxFboScreen::setYflip(bool flag)
{
	screenFlip.y = flag ? -1 : 1;
}

int ofxFboScreen::getScreenWidth( )
{
	return fbo.getWidth();
}

int ofxFboScreen::getScreenHeight( )
{
	return fbo.getHeight( );
}

ofPoint ofxFboScreen::getScreenUL( )
{
	return mesh.getVertex(indexA1);
}

ofPoint ofxFboScreen::getScreenUR( )
{
	return mesh.getVertex(indexA2);
}

ofPoint ofxFboScreen::getScreenDL( )
{
	return mesh.getVertex(indexB1);
}

ofPoint ofxFboScreen::getScreenDR( )
{
	return mesh.getVertex(indexB2);
}

void ofxFboScreen::loadAABB(ofPoint A1 , ofPoint A2 , ofPoint B1 , ofPoint B2)
{
	mesh.setVertex(indexA1 , A1);
	mesh.setVertex(indexA2 , A2);
	mesh.setVertex(indexB1 , B1);
	mesh.setVertex(indexB2 , B2);
	dragAble = true;
	onMouseUp(0 , 0 , 0);
}

void ofxFboScreen::begin( )
{
	fbo2.begin( );
	ofClear(0);
}

void ofxFboScreen::end( )
{
	fbo2.end( );
	fbo.begin( );
		ofClear(0);
		ofPoint trans = ofPoint(0 , 0);
		if(screenFlip.x == -1)
		{
			trans.x = fbo.getWidth( );
		}
		if(screenFlip.y == -1)
		{
			trans.y = fbo.getHeight( );
		}
		ofTranslate(trans);
		ofScale(screenFlip);
		fbo2.getTexture( ).draw(0 , 0);
	fbo.end( );
}

void ofxFboScreen::draw()
{
	ofPushMatrix( );
	ofTranslate(screenPos);
		fbo.getTexture( ).bind( );
			mesh.draw();
		fbo.getTexture( ).unbind( );
	ofPopMatrix( );
	//	ofVbo vbo;
	//	vbo.setMesh(mesh , GL_DYNAMIC_DRAW);
	//	vbo.draw(GL_QUADS , 0 , 8);
	//	glBegin(GL_QUADS);
	//	glNormal3f(0.0f , 0.0f , 1.0f);
	//	glTexCoord2f(0 , 0);
	//	glVertex3f(100,0,0);
	//
	//	glTexCoord2f(1 , 0);
	//	glVertex3f(200 , 0 , 0);
	//
	//	glTexCoord2f(1 , 1);
	//	glVertex3f(200 , 200 , 0);
	//
	//	glTexCoord2f(0 , 1);
	//	glVertex3f(100 , 150 , 0);
	//	glEnd( );

}

void ofxFboScreen::onMouseDown(int x , int y , int affectRadius , int btn)
{
	x -= screenPos.x;
	y -= screenPos.y;
	dragVertIndex = GetClosestVertexIndex(x , y , mesh.getVertices( ));
	auto vert = mesh.getVertex(dragVertIndex);
	if(ofDist(x , y , vert.x , vert.y) <= affectRadius)
	{
		if(type==SinglePlane && btn == 2)// right mouse
		{
			int col = dragVertIndex % (gridCol + 1);
			indexMidU = col;
			indexMidD = col + (gridCol + 1)*gridRow;
			MidU = toOf(originVerts[indexMidU]);
			MidD = toOf(originVerts[indexMidD]);
			return;
		}
		dragAble = true;
	}
}

void ofxFboScreen::onMouseUp(int x , int y , int btn)
{
	//x -= screenPos.x;
	//y -= screenPos.y;
	if(dragAble)
	{
		ofPoint dstA1 , dstA2 , dstB1 , dstB2 , dstMidU , dstMidD;
		dstA1 = mesh.getVertex(indexA1);
		dstA2 = mesh.getVertex(indexA2);
		dstB1 = mesh.getVertex(indexB1);
		dstB2 = mesh.getVertex(indexB2);
		dstMidD = mesh.getVertex(indexMidD);
		dstMidU = mesh.getVertex(indexMidU);
		if(type == SinglePlane)
		{
			transMatL = GetTransformMatrix(A1 , A2 , B1 , B2 , dstA1 , dstA2 , dstB1 , dstB2);
		}
		else if(type == DoublePlane)
		{
			transMatL = GetTransformMatrix(A1 , MidU , B1 , MidD
										   , dstA1 , dstMidU , dstB1 , dstMidD);
			transMatR = GetTransformMatrix(MidU , A2 , MidD , B2
										   , dstMidU , dstA2 , dstMidD , dstB2);
		}
		moveVertices( );
	}
	dragAble = false;
}

void ofxFboScreen::onMouseDragged(int x , int y , int btn)
{
	x -= screenPos.x;
	y -= screenPos.y;
	if(dragAble)
	{
		mesh.getVertices( )[dragVertIndex].x = x;
		mesh.getVertices( )[dragVertIndex].y = y;
	}
}

void ofxFboScreen::resetOutputSize(int w , int h)
{
	mesh.clear( );
	originVerts.clear( );
	setup(w , h , gridRow , gridCol , type,screenPos);
}

int ofxFboScreen::GetClosestVertexIndex(int x , int y , vector<ofVec3f> & vec)
{
	int squareDist = 1000000;
	int d;
	int index = 0;
	for(int i = 0; i<vec.size( ); i++)
	{
		d = ofDistSquared(x , y , vec[i].x , vec[i].y);
		if(d<squareDist)
		{
			index = i;
			squareDist = d;
		}
	}
	return index;
}
