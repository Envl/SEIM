#pragma once
#include "ISpell.h"
#include <map>

//class Spell;
class SpellManager
{
public:
	static SpellManager& Instance()
	{
		static SpellManager instance;
		return instance;
	}
	void addSpell(ISpell*);
	void destroySpell(std::string name);
	void batchUpdate();
	void batchDraw();
	ISpell* getSpell(std::string name);

private:
	SpellManager();
	~SpellManager();
	SpellManager(const SpellManager&);
	SpellManager& operator=(const SpellManager&);

	map<std::string, ISpell*> pool;

};



