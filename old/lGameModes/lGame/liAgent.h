#ifndef LI_AGENT
#define LI_AGENT

#include "../lprotoMap.h"

class liAgent
{
public:

	class liAgentBoolProperty;
	class liAgentIntProperty;
	class liAgentFloatProperty;
	class liAgentVectorProperty;

	class liAgentProperty
	{
	public:
		virtual liAgentBoolProperty		*ToBool()	= 0;
		virtual liAgentIntProperty		*ToInt()	= 0;
		virtual liAgentFloatProperty	*ToFloat()	= 0;
		virtual liAgentVectorProperty	*ToVector()	= 0;

		liAgentProperty()
		{

		}

		virtual ~liAgentProperty()
		{

		}
	};

	class liAgentBoolProperty : public liAgentProperty
	{
	public:
		virtual void Set(bool param) = 0;
		virtual bool Get() = 0;

		liAgentBoolProperty(){}
		virtual ~liAgentBoolProperty()override{}
	};

	class liAgentIntProperty : public liAgentProperty
	{
	public:
		virtual void Set(int param) = 0;
		virtual int Get() = 0;

		liAgentIntProperty(){}
		virtual ~liAgentIntProperty()override{}
	};

	class liAgentFloatProperty : public liAgentProperty
	{
	public:
		virtual void Set(float param) = 0;
		virtual float Get() = 0;

		liAgentFloatProperty(){}
		virtual ~liAgentFloatProperty()override{}
	};

	class liAgentVectorProperty : public liAgentProperty
	{
	public:
		virtual void Set(const lmVector2D &param) = 0;
		virtual const lmVector2D &Get() = 0;

		liAgentVectorProperty(){}
		virtual ~liAgentVectorProperty()override{}
	};

	virtual bool IsRemovable() = 0;
	virtual liAgentProperty *GetAgentProperty(const std::string &key) = 0;

	virtual void ExecuteBehavior(float dt) = 0;

	liAgent()
	{

	}

	virtual ~liAgent()
	{

	}

	//TMP!!!!!!!!!!!!!!!!!
	virtual void DBG_Draw(){};
};

#endif
