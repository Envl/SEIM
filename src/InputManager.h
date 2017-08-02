#pragma once
#include "ISimuInputReceiver.h"
#include <map>
#include "ofMain.h"

//class ISimuInputReceiver;
class InputManager
{
public:
	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}
	void sendSoundSpectrum(float* spectrum);
	//供 touch 事件发生者调用 , 此函数通知 touch 事件接收者
	void sendTouch(InputParam p);
	void sendMeanMove(float meanmove);
	void sendMovement(vector<float> &movement);
	void sendTouch(int x, int y,float force=1.0f,InputType type=InputType::TOUCH);// force range   0~ +++
	void sendTouches(vector<ofVec3f>& touches);
	void addReceiver(ISimuInputReceiver* receiver);
	void update();//这个要放在 ofApp 的 update 第一行, 用作每一帧的状态初始化
	const InputParam* getTouches() const;
	const InputParam* getClosestTouch(int x,int y) const;
	
	static const short MAXTOUCH = 30;
private:
	InputManager() = default;
	~InputManager() ;
	InputManager(const InputManager&);
	InputManager& operator=(const InputManager&);
	void clearStatus();

	std::map<std::string, ISimuInputReceiver*> pool;
	InputParam touches[MAXTOUCH];//touches使用fifo存储方式,暂且手动模拟
	int nextIndex=0;//下一个用来存储新 touch 的位置
};
