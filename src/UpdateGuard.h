#pragma once

class UpdateGuard
{
public:
	UpdateGuard(int interval=1);
	void setInterval(int interval);
	bool canUpdate( );

private:
	int counter = 0;
	int interval = 1;
};
