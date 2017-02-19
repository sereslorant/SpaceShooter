#ifndef LJRPG_PLAYER
#define LJRPG_PLAYER

#include "../lGM_Agents/AI/lGM_AI.h"
#include "lJRPG_Hero.h"

class lJRPG_Player : public lGM_AI
{
protected:

	lJRPG_Hero *Character;

	bool Up;
	bool Down;
	bool Left;
	bool Right;

	lmVector2D Dir;

	bool Sprint;

	bool AttackMode;
	bool Attack;

	bool ExecuteAbility;

public:

	virtual void Control(float dt) override
	{
		if(Up)
		{Character->SetUp(true);}
		else
		{Character->SetUp(false);}

		if(Down)
		{Character->SetDown(true);}
		else
		{Character->SetDown(false);}

		if(Left)
		{Character->SetLeft(true);}
		else
		{Character->SetLeft(false);}

		if(Right)
		{Character->SetRight(true);}
		else
		{Character->SetRight(false);}

		if(Sprint)
		{Character->SetSprint(true);}
		else
		{Character->SetSprint(false);}

		if(AttackMode)
		{Character->SwitchAttackMode();}

		if(Attack)
		{Character->SetAttack(true);}
		else
		{Character->SetAttack(false);}

		Character->SetDirX(Dir.X);
		Character->SetDirY(Dir.Y);

		if(ExecuteAbility)
		{
			Character->SetCurrentAbility(0);
			Character->SetExecuteAbility(true);
		}
		else
		{Character->SetExecuteAbility(false);}
	}

	void SetUp(bool up)
	{
		Up = up;
	}

	void SetDown(bool down)
	{
		Down = down;
	}

	void SetLeft(bool left)
	{
		Left = left;
	}

	void SetRight(bool right)
	{
		Right = right;
	}

	void SetDirX(float x,bool ScreenSpace)
	{
		if(ScreenSpace)
		{
			Dir.X = x - Avatar->GetObject()->GetPosition().X;
		}
		else
		{
			Dir.X = x;
		}
	}

	void SetDirY(float y,bool ScreenSpace)
	{
		if(ScreenSpace)
		{
			Dir.Y = y - Avatar->GetObject()->GetPosition().Y;
		}
		else
		{
			Dir.Y = y;
		}
	}

	void SetSprint(bool sprint)
	{
		Sprint = sprint;
	}

	void SetAttackMode(bool attack_mode)
	{
		AttackMode = attack_mode;
	}

	void SetAttack(bool attack)
	{
		Attack = attack;
	}

	void SetExecuteAbility(bool execute_ability)
	{
		ExecuteAbility = execute_ability;
	}

	lGM_Fighter *GetCharacter()
	{
		return Avatar;
	}

	lJRPG_Player(lJRPG_Hero *character)
		:lGM_AI(character),Character(character)
	{
		Up      = false;
		Down    = false;
		Left    = false;
		Right   = false;

		AttackMode	= false;
		Attack		= false;
	}
};

#endif
