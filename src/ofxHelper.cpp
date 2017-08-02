#include "ofxHelper.h"


ofxHelper::ofxHelper()
{
}


ofxHelper::~ofxHelper()
{
}

void ofxHelper::loadFrameColors(char* path, vector<ofColor>& vec)
{
	ofFile file;
	file.open(path);
	char line[20];
	bool t = file.is_open( );
	while(file.getline(line , 20))
	{
		string str = line;
		ofColor c;
		auto color = ofSplitString(line , " ");
		c.set(ofToInt(color[0])
			  , ofToInt(color[1])
			  , ofToInt(color[2]));
		vec.push_back(c);
	}
	file.close( );
}

void ofxHelper::LoadSequence(SequenceImage& sequence, char startName[], int num)
{
	char name[30];
	for(int i = 0; i < num; i++)
	{
		sprintf(name , "%s%.5d.jpg" , startName , i);
		ofImage img;
		img.load(name);
		img.update( );
		sequence.addFrame(img.getTexture());
	}
}

ofMesh ofxHelper::GenMesh(int gridRow, int gridColumn,int gridSize,ofPrimitiveMode mode)
{
	ofMesh mesh;
	//add vertice
	for(int row=0;row<=gridRow;row++)
	{
		for(int col=0;col<=gridColumn;col++)
		{
			mesh.addVertex(ofVec3f(row*gridSize, col*gridSize, 0));
			mesh.addTexCoord(ofVec2f(static_cast<float>(row)/gridRow,static_cast<float>( col)/gridColumn));//使用 normalized TexCoord 坐标
		}
	}
	//add indices
	for(int row=0;row<gridRow;row++)
	{
		for (int col=0;col<gridColumn;col++)
		{
			mesh.addTriangle(col + row*(gridRow + 1),
				col + 1 + row*(gridRow + 1),
				col + (row + 1)*(gridRow + 1));
			//lower right
			mesh.addTriangle(col + 1 + row*(gridRow + 1),
				col + 1 + (row + 1)*(gridRow + 1),
				col + (row + 1)*(gridRow+ 1));
		}
	}

	return mesh;
}

ofVboMesh ofxHelper::GenVboMesh(int gridRow, int gridColumn, int gridSizeW,int gridSizeH, ofPrimitiveMode MODE)
{
	ofVboMesh mesh;
	//add vertice
	for(int row = 0; row <= gridRow; row++)
	{
		for(int col = 0; col <= gridColumn; col++)
		{
			mesh.addVertex(ofVec3f(row*gridSizeH , col*gridSizeW , 0));
			mesh.addTexCoord(ofVec2f(static_cast<float>(row) / gridRow , static_cast<float>(col) / gridColumn));//使用 normalized TexCoord 坐标
		}
	}
	//add indices
	for(int row = 0; row<gridRow; row++)
	{
		for(int col = 0; col<gridColumn; col++)
		{
			mesh.addTriangle(col + row*(gridRow + 1) ,
							 col + 1 + row*(gridRow + 1) ,
							 col + (row + 1)*(gridRow + 1));
			//lower right
			mesh.addTriangle(col + 1 + row*(gridRow + 1) ,
							 col + 1 + (row + 1)*(gridRow + 1) ,
							 col + (row + 1)*(gridRow + 1));

		}
	}

	return mesh;
}

int ofxHelper::Index4FPS(int fps, int frameAmount)
{
	return (int)(
		((float)(ofGetElapsedTimeMillis( ) % 1000)
		 / 1000.0f)
		*fps) % frameAmount;
}

int ofxHelper::GetClosestVertexIndex(int x,int y,vector<ofVec3f> vec)
{
	int squareDist = 1000000;
	int d;
	int index = 0;
	for(int i=0;i<vec.size();i++)
	{
		d = ofDistSquared(x , y , vec[i].x , vec[i].y);
		cout << x << " " << y << " " << vec[i] << " dist "<<d<<endl;
		if(d<squareDist)
		{
			index = i;
			squareDist = d;
		}
	}
	return index;
}

void ofxHelper::DrawVertices(vector<ofPoint> vertices,int radius,ofColor color)
{
	ofPushStyle( );
	ofSetColor(color);
	for(auto &v:vertices)
	{
		ofCircle(v.x , v.y , radius);
	}
	ofPopStyle( );
}

float ofxHelper::HeartBeat()
{
	return  ofNoise(ofWrap(ofGetElapsedTimef( ) , 0 , 1));
}

cv::Mat ofxHelper::GetTransformMatrix(ofPoint A1, ofPoint A2, ofPoint B1, ofPoint B2, ofPoint dstA1, ofPoint dstA2, ofPoint dstB1, ofPoint dstB2)
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

ofVec3f ofxHelper::transCoord(cv::Mat& transMat, ofVec3f origin)
{
	vector<cv::Point2f> in(1) , out;
	in[0] = cv::Point2f(origin.x , origin.y);
	cv::perspectiveTransform(in , out , transMat);
	return ofPoint(out[0].x , out[0].y);
}
