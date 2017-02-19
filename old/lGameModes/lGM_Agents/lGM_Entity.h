#ifndef L_JRPG_ENTITY_H
#define L_JRPG_ENTITY_H

#include "../lGame/liAgent.h"

class lGM_Entity : public liAgent
{
public:
	static const std::string POSITION_KEY;
	static const std::string RADIUS_KEY;
	static const std::string SPECIES_KEY;
	static const std::string ALIVE_KEY;
	static const std::string INVINCIBLE_KEY;
	static const std::string TARGETABLE_KEY;
	static const std::string HEALTH_KEY;

protected:

	bool Removable;

	lprotoObject *Object;

	int Species;

	bool Alive;
	bool Invincible;
	bool Targetable;

	float MaxHP;
	float HP;

	std::map<std::string,liAgentProperty *> Properties;

public:

	bool IsRemovable()
	{
		return Removable;
	}

	virtual liAgentProperty *GetAgentProperty(const std::string &key) override
	{
		auto I = Properties.find(key);

		if(I != Properties.end())
		{
			return I->second;
		}
		else
		{
			return nullptr;
		}
	}

	lprotoObject *GetObject()
	{
		return Object;
	}

	int GetSpecies()
	{
		return Species;
	}

	void SetSpecies(int species)
	{
		Species = species;
	}

	const lmVector2D &GetPosition()
	{
		return Object->GetPosition();
	}

	bool IsAlive()
	{
		return Alive;
	}

	void SetAlive(bool alive)
	{
		Alive = alive;
	}

	bool IsInvincible()
	{
		return Invincible;
	}

	void SetInvincible(bool invincible)
	{
		Invincible = invincible;
	}

	bool IsTargetable()
	{
		return Targetable;
	}

	void SetTargetable(bool targetable)
	{
		Targetable = targetable;
	}

	float GetHealth()
	{
		return HP;
	}

	void SetHealth(float param)
	{
		if(param < 0.0)
		{
			if(!Invincible)
			{
				HP = param;
				if(HP < 0.0)
				{
					Alive = false;
				}
			}
		}
		else
		{
			if((param) > MaxHP)
			{
				HP = MaxHP;
			}
			else
			{
				HP = param;
			}
		}
	}

	lGM_Entity(lprotoObject *object,int species,bool alive,bool invincible,bool targetable,float max_hp);
	virtual ~lGM_Entity();
};

#endif // L_JRPG_ENTITY_H
