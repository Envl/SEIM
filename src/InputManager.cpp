#include "InputManager.h"
#include <iostream>

void InputManager::sendSoundSpectrum(float* spectrum)
{
	if(pool.empty( ))
	{
		return;
	}
	for(auto &p :pool)
	{
		p.second->onSoundSpectrum(spectrum);
	}
}

void InputManager::sendTouch(InputParam p)
{
	sendTouch(p.x , p.y , p.force , p.type);
}

void InputManager::sendMeanMove(float meanmove)
{
	for(auto &p : pool)
	{
		p.second->onMeanMove(meanmove);
	}
}

void InputManager::sendMovement(vector<float>& movement)
{
	for(auto &p:pool){
		p.second->onMovement(movement);
	}
}

/* 两部分组成
	1  保存到 touches 里面  
	2  pool 非空时,分发 ontouch 事件
*/
void InputManager::sendTouch(int x, int y,float force,InputType type)
{
	//part 1
	touches[nextIndex].x = x;
	touches[nextIndex].y = y;
	touches[nextIndex].force = force;
	touches[nextIndex].type = type;
	
	touches[nextIndex].isActive = true;
	nextIndex = ofWrap(++nextIndex, 0, MAXTOUCH-1);
	//part 2
	if (pool.empty()) 
	{
		return;
	}
	for (auto &receiver:pool)
	{
		receiver.second->onTouch(touches[nextIndex]);
//		printf("% d% d% s", x, y, "onTouch called\n");
	}
}

void InputManager::sendTouches(vector<ofVec3f>& touches)
{
	for(auto &t : touches)
	{
		sendTouch(t.x , t.y , t.z);
	}
}

void InputManager::addReceiver(ISimuInputReceiver* receiver)
{
	if(pool.find(receiver->getReceiverName())==pool.end())
	{
		pool[receiver->getReceiverName()] = receiver;
	}
	else
	{
		std::cout << "receiver name already exists" << std::endl;
		throw "ERROR receiver name already exists";
	}
}

void InputManager::update()
{
	clearStatus();
}

const InputParam* InputManager::getTouches() const
{
	return touches;
}

const InputParam* InputManager::getClosestTouch(int x, int y) const
{
	InputParam t;
	float dist = 100000001;
	float d;
	int index = -1;
	for(int i=0;i<MAXTOUCH;i++)
	{
		if(touches[i].isActive && touches[i].type == InputType::TOUCH)
		{
			d = ofDist(x, y, touches[i].x, touches[i].y);
			if(d<dist)
			{
				index = i;
				dist = d;
			}
		}
	}
	return dist == 100000001 ? nullptr : touches+index;
}

InputManager::~InputManager()
{
}

void InputManager::clearStatus()
{
	nextIndex = 0;//touches 手动模拟队列,将其逻辑上置空
	for(int i=0;i<MAXTOUCH;i++)
	{
		touches[i].isActive = false;
	}
}
