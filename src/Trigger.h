#pragma once
#include <vector>
#include <map>
#include <string>

class Trigger
{
public:
	Trigger( ) = default;
	Trigger( float time,std::string name) :bUsed(false)
	{
		triggerTime = time;
		this->name = name;
	}
	~Trigger( ) = default;
	
	void setReady( );
	void reActivate();
	bool check( );
	bool checkTime(float t);
	float getTriggerTime( );
	std::string getName( );
private:
	std::string name;
	float triggerTime;
	bool bReady = false;
	bool bUsed ;
};

