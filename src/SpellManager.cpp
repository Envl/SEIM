#include "SpellManager.h"
#include <iostream>

void SpellManager::addSpell(ISpell* spell)
{
	if (pool.find(spell->getSpellName())==pool.end())
	{
		pool[spell->getSpellName()] = spell;
	}
	else
	{
		std::cout << "spell name already exists" << std::endl;
		throw "ERROR spell name already exists";
	}
}

void SpellManager::destroySpell(std::string name)
{
	if (pool.find(name)!=pool.end())
	{
		pool[name]->finish();
		pool.erase(name);
	}
}

void SpellManager::batchUpdate()
{
	if (!pool.empty())
	{
		for (auto e :pool)
		{
			e.second->update();
		}
	}

}

void SpellManager::batchDraw()
{
	if (!pool.empty())
	{
		for (auto e :pool)
		{
			e.second->draw();
		}
	}
}

ISpell* SpellManager::getSpell(std::string name)
{
	if(pool.find(name)!=pool.end())
	{
		return pool[name];
	}
	return nullptr;
}

SpellManager::SpellManager()
{
}

SpellManager::~SpellManager()
{
	if (!pool.empty() ){
		for (auto entry : pool)
		{
			entry.second->finish();
		}
		pool.clear();
	}
}