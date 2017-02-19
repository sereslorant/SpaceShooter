#ifndef LGM_AI_COMMAND_H
#define LGM_AI_COMMAND_H

class lGM_AI;

class lGM_AICommand
{
protected:
	lGM_AI *AI;

	bool Finished = false;
	bool Interruptable;
	//unsigned int Priority;

public:
	/*
	unsigned int GetPriority()
	{
		return Priority;
	}
	*/

	virtual void FollowCommand() = 0;

	bool IsFinished()
	{
		return Finished;
	}

	void Trigger()
	{
		Finished = false;
	}

	bool IsInterruptable()
	{
		return Interruptable;
	}

	void Interrupt()
	{

	}

	void Suspend()
	{

	}

	void Resume()
	{

	}

	lGM_AICommand(lGM_AI *ai,bool interruptable)
		:AI(ai),Interruptable(interruptable)
	{}

	virtual ~lGM_AICommand()
	{}
};

#endif // LTD_UNIT_COMMAND_H
