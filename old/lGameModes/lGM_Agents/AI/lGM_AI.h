#ifndef LGM_AI_H
#define LGM_AI_H

#include <list>

class lGM_Fighter;
class lGM_AICommand;

class lGM_AI
{
protected:
	lGM_Fighter *Avatar;

	std::list<lGM_AICommand *> CommandQueue;
	lGM_AICommand *CurrentCommand = nullptr;

public:

	lGM_Fighter *GetAvatar()
	{
		return Avatar;
	}

	void SetCommand(lGM_AICommand *command);

	virtual void Control(float dt);

	lGM_AI(lGM_Fighter *avatar)
		:Avatar(avatar)
	{

	}

	virtual ~lGM_AI()
	{

	}
};

#endif // LGM_AI_H
