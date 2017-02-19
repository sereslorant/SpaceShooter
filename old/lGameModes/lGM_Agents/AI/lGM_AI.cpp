
#include "lGM_AI.h"

#include "../lGM_Fighter.h"
#include "lGM_AICommand/lGM_AICommand.h"

void lGM_AI::SetCommand(lGM_AICommand *command)
{
	//Avatar->SetCommand(command);
	if(CurrentCommand != nullptr)
	{
		if(CurrentCommand->IsInterruptable())
		{
			CurrentCommand->Interrupt();
		}
		else
		{
			CurrentCommand->Suspend();
			CommandQueue.push_back(CurrentCommand);
		}
	}

	CurrentCommand = command;
	if(CurrentCommand != nullptr)
	{
		CurrentCommand->Trigger();
	}
}

void lGM_AI::Control(float dt)
{
	if(CurrentCommand != nullptr)
	{
		CurrentCommand->FollowCommand();

		if(CurrentCommand->IsFinished())
		{
			delete CurrentCommand;

			if(CommandQueue.size() != 0)
			{
				CurrentCommand = CommandQueue.back();
				CommandQueue.pop_back();
			}
			else
			{
				CurrentCommand = nullptr;

				Avatar->SetUp(false);
				Avatar->SetDown(false);
				Avatar->SetLeft(false);
				Avatar->SetRight(false);
			}
		}
	}
}
