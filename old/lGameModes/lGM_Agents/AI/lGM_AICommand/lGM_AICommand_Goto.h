#ifndef LTD_AI_COMMAND_GOTO_H
#define LTD_AI_COMMAND_GOTO_H

#include "lGM_AICommand.h"

#include "../lGM_AI.h"
#include "../../lGM_Fighter.h"

bool lTD_UnitGoto(lGM_Fighter *unit,const lmVector2D &destination,float epsilon);

class lGM_AICommand_Goto : public lGM_AICommand
{
protected:
	lmVector2D Destination;
	const float EPSILON = 10.0;

public:

	virtual void FollowCommand() override
	{
		/*
		const lmVector2D &AvatarPos = Avatar->GetPosition();

		bool Arrived = true;
		float Tmp;

		Tmp = Destination.Y - AvatarPos.Y;
		if(std::abs(Tmp) > EPSILON)
		{
			if(Tmp < 0.0)
			{
				Avatar->c_MoveUp();
			}
			else
			{
				Avatar->c_MoveDown();
			}

			Arrived = false;
		}
		else
		{
			Avatar->SetUp(false);
			Avatar->SetDown(false);
		}

		Tmp = Destination.X - AvatarPos.X;
		if(std::abs(Tmp) > EPSILON)
		{
			if(Tmp < 0.0)
			{
				Avatar->c_MoveLeft();
			}
			else
			{
				Avatar->c_MoveRight();
			}

			Arrived = false;
		}
		else
		{
			Avatar->SetLeft(false);
			Avatar->SetRight(false);
		}
		*/
		bool Arrived = lTD_UnitGoto(AI->GetAvatar(),Destination,EPSILON);

		if(Arrived == true)
		{
			Finished = true;
		}
	}

	lGM_AICommand_Goto(lGM_AI *ai,bool interruptable,const lmVector2D &destination)
		:lGM_AICommand(ai,interruptable),Destination(destination)
	{}

	virtual ~lGM_AICommand_Goto() override
	{}
};

class lGM_AICommand_LookAt : public lGM_AICommand
{
protected:
	lmVector2D Target;

public:

	virtual void FollowCommand() override
	{
		lmVector2D Direction = Target - AI->GetAvatar()->GetPosition();

		AI->GetAvatar()->SetDirX(Direction.X);
		AI->GetAvatar()->SetDirY(Direction.Y);
	}

	lGM_AICommand_LookAt(lGM_AI *ai,bool interruptable,const lmVector2D &target)
		:lGM_AICommand(ai,interruptable),Target(target)
	{}

	virtual ~lGM_AICommand_LookAt() override
	{}

};

class lGM_AICommand_FollowPath : public lGM_AICommand
{
protected:
	std::vector<lmVector2D> Path;
	unsigned int NextTarget = 0;

	const float Epsilon;

public:

	virtual void FollowCommand() override
	{
		if(!Finished)
		{
			if(NextTarget < Path.size())
			{
				/*
				const lmVector2D &AvatarPos = Avatar->GetPosition();

				bool Arrived = true;
				float Tmp;

				Tmp = Path[NextTarget].Y - AvatarPos.Y;
				if(std::abs(Tmp) > EPSILON)
				{
					if(Tmp < 0.0)
					{
						Avatar->c_MoveUp();
					}
					else
					{
						Avatar->c_MoveDown();
					}

					Arrived = false;
				}
				else
				{
					Avatar->SetUp(false);
					Avatar->SetDown(false);
				}

				Tmp = Path[NextTarget].X - AvatarPos.X;
				if(std::abs(Tmp) > EPSILON)
				{
					if(Tmp < 0.0)
					{
						Avatar->c_MoveLeft();
					}
					else
					{
						Avatar->c_MoveRight();
					}

					Arrived = false;
				}
				else
				{
					Avatar->SetLeft(false);
					Avatar->SetRight(false);
				}
				*/
				bool Arrived = lTD_UnitGoto(AI->GetAvatar(),Path[NextTarget],Epsilon);

				if(Arrived == true)
				{
					NextTarget++;
				}
			}
			else
			{
				Finished = true;
			}
		}
	}

	lGM_AICommand_FollowPath(lGM_AI *ai,bool interruptable,const lmVector2D *path,unsigned int path_length,float epsilon = 10.0)
		:lGM_AICommand(ai,interruptable),Path(path_length),Epsilon(epsilon)
	{
		for(int i=0;i < Path.size();i++)
		{
			Path[i] = path[i];
		}
	}

	virtual ~lGM_AICommand_FollowPath() override
	{}

};


class lGM_AICommand_Attack : public lGM_AICommand
{
private:
	lGM_Entity *Enemy;
	float AttackDistance;

	float EPSILON = 1.0;

public:

	virtual void FollowCommand() override
	{
		if(!Enemy->IsAlive())
		{
			Finished = true;
			return;
		}

		lmVector2D Direction = Enemy->GetPosition() - AI->GetAvatar()->GetPosition();

		AI->GetAvatar()->SetDirX(Direction.X);
		AI->GetAvatar()->SetDirY(Direction.Y);

		lTD_UnitGoto(AI->GetAvatar(),Enemy->GetPosition(),EPSILON);

		if(Direction.LengthSquared() < AttackDistance*AttackDistance)
		{
			AI->GetAvatar()->SetAttack(true);
		}
	}

	lGM_AICommand_Attack(lGM_AI *ai,bool interruptable,lGM_Entity *enemy,lmScalar attack_distance)
		:lGM_AICommand(ai,interruptable),Enemy(enemy),AttackDistance(attack_distance)
	{}

	virtual ~lGM_AICommand_Attack() override
	{}

};


#endif // LTD_AI_COMMAND_GOTO_H
