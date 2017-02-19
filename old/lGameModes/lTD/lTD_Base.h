#ifndef LTD_BASE_H
#define LTD_BASE_H

#include "../lGM_Agents/lGM_Entity.h"

class lTD_Base : public lGM_Entity
{
public:

	virtual void ExecuteBehavior(float dt) override
	{

	}

	lTD_Base(int species,float max_health,bool invincible,lprotoObject *object)
		:lGM_Entity(object,species,true,invincible,true,max_health)//Object(object),Species(species),MaxHealth(max_health),Health(max_health)
	{
		//Object->SetUserData(this);
	}

	virtual ~lTD_Base() override
	{}
};

#endif // LTD_BASE_H
