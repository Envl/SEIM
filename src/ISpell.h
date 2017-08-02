#pragma once
#include <ofMatrixStack.h>
#include <ofNode.h>

class ISpell:public ofNode
{
public:
	virtual ~ISpell()=default ; 
	virtual ISpell* init(int x,int y,int w ,int h,float deg=0)=0;
	virtual void update() = 0;//每帧调用
	void virtual draw() final;  //这个覆盖掉了 ofNode 的同名函数 
	virtual void finish() ;//外部调用清理  
	string getSpellName() const;
	ofVec2f mapScreenPosToViewPort(int x , int y);
	bool checkPosInViewport( int x,int y);
	ofRectangle getViewport( );
	void Activate( );
	void Deactivate( );
protected:	
	void customDraw( ) override = 0;// 同上 覆盖 ofNode ....
	bool bIsActive = true;
	virtual void commonInit(int x , int y , int w , int h , int deg) final;
	static int AMOUNT;
	string spellName;
	ofRectangle viewPort = ofRectangle(0 , 0 , 100 , 60);
	int width , height;
};
