
#include "lGM_Entity.h"

const std::string lGM_Entity::POSITION_KEY		= "Position";
const std::string lGM_Entity::RADIUS_KEY		= "Radius";
const std::string lGM_Entity::SPECIES_KEY		= "Species";
const std::string lGM_Entity::ALIVE_KEY			= "Alive";
const std::string lGM_Entity::INVINCIBLE_KEY	= "Invincible";
const std::string lGM_Entity::TARGETABLE_KEY	= "Targetable";
const std::string lGM_Entity::HEALTH_KEY		= "Health";

class lGM_EntityPositionProperty : public liAgent::liAgentVectorProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return nullptr;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return nullptr;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return nullptr;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return this;}


	virtual void Set(const lmVector2D &param)override
	{
		Entity->GetObject()->SetPosition(param);
	}

	virtual const lmVector2D &Get()
	{
		return Entity->GetObject()->GetPosition();
	}

	lGM_EntityPositionProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntityPositionProperty() override
	{

	}
};

class lGM_EntityRadiusProperty : public liAgent::liAgentFloatProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return nullptr;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return nullptr;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return this;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return nullptr;}

	virtual void Set(float param) override
	{
		if(param >= 0.0)
		{
			Entity->GetObject()->SetRadius(param);
		}
	}

	virtual float Get() override
	{
		return Entity->GetObject()->GetRadius();
	}

	lGM_EntityRadiusProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntityRadiusProperty() override
	{

	}
};

class lGM_EntityAliveProperty : public liAgent::liAgentBoolProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return this;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return nullptr;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return nullptr;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return nullptr;}

	virtual void Set(bool param) override
	{
		Entity->SetAlive(param);
	}

	virtual bool Get()override
	{
		return Entity->IsAlive();
	}

	lGM_EntityAliveProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntityAliveProperty() override
	{

	}
};

class lGM_EntityInvincibleProperty : public liAgent::liAgentBoolProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return this;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return nullptr;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return nullptr;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return nullptr;}

	virtual void Set(bool param)override
	{
		Entity->SetInvincible(param);
	}

	virtual bool Get()override
	{
		return Entity->IsInvincible();
	}

	lGM_EntityInvincibleProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntityInvincibleProperty() override
	{

	}
};

class lGM_EntityTargetableProperty : public liAgent::liAgentBoolProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return this;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return nullptr;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return nullptr;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return nullptr;}

	virtual void Set(bool param)override
	{
		Entity->SetTargetable(param);
	}

	virtual bool Get()override
	{
		return Entity->IsTargetable();
	}

	lGM_EntityTargetableProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntityTargetableProperty() override
	{

	}
};

class lGM_EntitySpeciesProperty : public liAgent::liAgentIntProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return nullptr;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return this;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return nullptr;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return nullptr;}

	virtual void Set(int param) override
	{
		Entity->SetSpecies(param);
	}

	virtual int Get() override
	{
		return Entity->GetSpecies();
	}

	lGM_EntitySpeciesProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntitySpeciesProperty() override
	{

	}
};

class lGM_EntityHealthProperty : public liAgent::liAgentFloatProperty
{
private:
	lGM_Entity *Entity;

public:

	virtual liAgent::liAgentBoolProperty *ToBool() override {return nullptr;}
	virtual liAgent::liAgentIntProperty *ToInt() override {return nullptr;}
	virtual liAgent::liAgentFloatProperty *ToFloat() override {return this;}
	virtual liAgent::liAgentVectorProperty *ToVector() override {return nullptr;}

	virtual void Set(float param) override
	{
		Entity->SetHealth(param);
	}

	virtual float Get() override
	{
		return Entity->GetHealth();
	}

	lGM_EntityHealthProperty(lGM_Entity *entity)
		:Entity(entity)
	{

	}

	virtual ~lGM_EntityHealthProperty() override
	{

	}
};

lGM_Entity::lGM_Entity(lprotoObject *object,int species,bool alive,bool invincible,bool targetable,float max_hp)
	:Object(object),Species(species),Alive(alive),Invincible(invincible),Targetable(targetable),MaxHP(max_hp),HP(max_hp)
{
	Object->SetUserData(this);

	Properties[POSITION_KEY]	= new lGM_EntityPositionProperty(this);
	Properties[RADIUS_KEY]		= new lGM_EntityRadiusProperty(this);
	Properties[SPECIES_KEY]		= new lGM_EntitySpeciesProperty(this);
	Properties[ALIVE_KEY]		= new lGM_EntityAliveProperty(this);
	Properties[INVINCIBLE_KEY]	= new lGM_EntityInvincibleProperty(this);
	Properties[TARGETABLE_KEY]	= new lGM_EntityTargetableProperty(this);
	Properties[HEALTH_KEY]		= new lGM_EntityHealthProperty(this);
}

lGM_Entity::~lGM_Entity()
{
	for(auto I : Properties)
	{
		delete I.second;
	}
}
