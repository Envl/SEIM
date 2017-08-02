#include "ISimuInputReceiver.h"

void ISimuInputReceiver::onTouch(InputParam p)
{
	//这个函数不是必须实现,  默认什么也不做
}

void ISimuInputReceiver::onSoundSpectrum(float* spectrum)
{
	// 这个函数不是必须实现,  默认什么也不做
}

void ISimuInputReceiver::onMovement(std::vector<float>& movement)
{
	//这个函数不是必须实现,  默认什么也不做
}

void ISimuInputReceiver::onMeanMove(float meanmove)
{
	//这个函数不是必须实现,  默认什么也不做
}

std::string ISimuInputReceiver::getReceiverName() const
{
	return receiverName;
}
