#pragma once
#include <string>
#include <vector>

enum InputType
{
	TOUCH,
	LIGHT
};

struct InputParam
{
	int x , y;
	float force;
	InputType type;
	bool isActive;// 逻辑上控制该 Input 是否存在, false 就失效了
	InputParam( )
	{
		x = 0; y = 0; force = 1; isActive = false; type = InputType::TOUCH;
	}
};
//-------模拟输入的接口------
//要接受此事件的类,需要实现本接口
//-------------------------------------
class ISimuInputReceiver
{
public:
	//force range  0~ +++
	virtual void onTouch(InputParam p) ;
	virtual void onSoundSpectrum(float *spectrum);
	virtual void onMovement(std::vector<float>& movement);
	virtual void onMeanMove(float meanmove);
	virtual ~ISimuInputReceiver() = default;
	std::string getReceiverName() const;

protected:
	std::string receiverName;
};
