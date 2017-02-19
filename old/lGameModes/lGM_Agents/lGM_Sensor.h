#ifndef LTD_SENSOR_H
#define LTD_SENSOR_H

#include "lGM_Entity.h"

class lGM_Sensor
{
public:

	class lGM_SensorPredicate
	{
	public:
		virtual bool operator()(liAgent *agent) = 0;

		lGM_SensorPredicate(){}
		virtual ~lGM_SensorPredicate(){}
	};

private:
	lprotoObject *Range;

	class lGM_SensorRangeCallback : public lprotoCollisionCallback
	{
	private:
		lGM_Sensor *Sensor;
		lGM_SensorPredicate *SensorPredicate;

	public:

		lGM_SensorRangeCallback(lGM_Sensor *sensor,lGM_SensorPredicate *sensor_predicate)
			:Sensor(sensor),SensorPredicate(sensor_predicate)
		{}

		virtual ~lGM_SensorRangeCallback() override
		{
			delete SensorPredicate;
		}

		virtual void Callback(lprotoObject *obj) override
		{
			void *UserData = obj->GetUserData();

			if(UserData != nullptr)
			{
				liAgent *Agent = (liAgent *)UserData;

				if((*SensorPredicate)(Agent))
				{
					Sensor->Targets.push_back(Agent);
				}
			}
		}
	};

	lGM_SensorRangeCallback *SensorRangeCallback;

public:

	std::list<liAgent *> Targets;

	void SetPosition(const lmVector2D &position)
	{
		Range->SetPosition(position);
	}

	lprotoObject *GetRange()
	{
		return Range;
	}

	lGM_Sensor(lprotoObject *range,lGM_SensorPredicate *sensor_predicate)
		:Range(range)
	{
		SensorRangeCallback = new lGM_SensorRangeCallback(this,sensor_predicate);
		Range->SetCollisionCallback(SensorRangeCallback);
	}

	~lGM_Sensor()
	{

	}
};

class lGM_EnemyDetectorPredicate : public lGM_Sensor::lGM_SensorPredicate
{
private:
	lGM_Entity *Parent;

public:
	virtual bool operator()(liAgent *agent) override
	{
		liAgent::liAgentProperty	*pSpecies = agent->GetAgentProperty(lGM_Entity::SPECIES_KEY);
		liAgent::liAgentIntProperty	*Species = pSpecies->ToInt();

		return Species->Get() != Parent->GetSpecies();
	}

	lGM_EnemyDetectorPredicate(lGM_Entity *parent)
		:Parent(parent)
	{}

	virtual ~lGM_EnemyDetectorPredicate() override
	{}
};

#endif // LTD_SENSOR_H
