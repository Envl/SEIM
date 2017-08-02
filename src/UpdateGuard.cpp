#include "UpdateGuard.h"

UpdateGuard::UpdateGuard(int interval)
{
	this->interval = interval;
}

void UpdateGuard::setInterval(int interval)
{
	interval = interval > 0 ? interval : 1;
	this->interval = interval;
}

bool UpdateGuard::canUpdate()
{
	counter++;
	if (counter>=interval)
	{
		counter = 0;
		return true;
	}
	else
	{
		return  false;
	}
}

