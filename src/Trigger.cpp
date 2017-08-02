#include "Trigger.h"

void Trigger::setReady( )
{
	bReady = true;
}

void Trigger::reActivate()
{
	bUsed = false;
	bReady = false;
}

bool Trigger::check( )
{
	if (!bUsed&&bReady) {
		bUsed = true;
		return true;
	}
	return false;
}

bool Trigger::checkTime(float t)
{
	if(t >= triggerTime && !bUsed)
	{
		bUsed = true;
		return true;
	}
	return false;
}

float Trigger::getTriggerTime( )
{
	return triggerTime;
}
std::string Trigger::getName( )
{
	return name;
}
//
//void TimeScheduler::update(float t)
//{
//	time = t;
//	for(auto &trigger:pool)
//	{
//		if(trigger.checkTime(t))
//		{
//			funcMap[trigger.getTriggerTime( )](
//				phaseMap[trigger.getTriggerTime( )]
//				);
//		}
//	}
//}
//
//void TimeScheduler::addPhaseTask(pf func ,Phase pha, float triggerTime)
//{
//	pool.push_back(Trigger(triggerTime));
//	funcMap[triggerTime] = func;
//	phaseMap[triggerTime] = pha;
//}
