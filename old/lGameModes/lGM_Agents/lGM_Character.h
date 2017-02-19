#ifndef LJRPG_CHARACTER
#define LJRPG_CHARACTER

#include "lGM_Entity.h"

class lGM_Character : public lGM_Entity
{
protected:
	lprotoAnimObject *AnimObject;

	bool anim_Up;
	bool anim_Down;
	bool anim_Left;
	bool anim_Right;

	enum DIRECTION
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
	};

	enum ANIM
	{
		STEP_LEFT	= 0,
		STANDING	= 1,
		STEP_RIGHT	= 2,
		MELEE0_ATK	= 3,
		MELEE1_ATK	= 4,
		MELEE_ATK	= 5,
	};

	enum DEAD_ANIM
	{
		DEAD_LEFT,
		DEAD_BACK,
		DEAD_RIGHT,
	};

	static const int DEAD_ANIM_ID = 4;

	bool anim_Moving;
	bool anim_MeleeAttack;
	bool anim_Dead;

	bool AnimLeft;

	DIRECTION	Direction;
	ANIM		Anim;
	DEAD_ANIM	DeadAnim;

	float AnimDuration = 0.1;
	float AnimTimer = 0.0;

	//GLuint Textures[4][10];

	void TriggerMeleeAnim()
	{
		anim_MeleeAttack = true;
		Anim = MELEE0_ATK;
	}

	void Animate(float dt)
	{
		if(!anim_Dead)
		{
			if(anim_Up == true)
			{
				Direction = FRONT;
			}
			if(anim_Down == true)
			{
				Direction = BACK;
			}
			if(anim_Left == true)
			{
				Direction = LEFT;
			}
			if(anim_Right == true)
			{
				Direction = RIGHT;
			}
		}

		if(anim_Dead)
		{
			switch(Direction)
			{
			case LEFT:
				DeadAnim = DEAD_LEFT;
				break;
			case RIGHT:
				DeadAnim = DEAD_RIGHT;
				break;
			default:
				DeadAnim = DEAD_BACK;
			}
		}
		else
		{
			if(anim_Moving)
			{
				if(AnimTimer > AnimDuration)
				{
					switch(Anim)
					{
					case STEP_LEFT:
						AnimLeft = true;
						Anim = STANDING;
						break;
					case STEP_RIGHT:
						AnimLeft = false;
						Anim = STANDING;
						break;
					case STANDING:
						if(AnimLeft)
						{
							Anim = STEP_RIGHT;
						}
						else
						{
							Anim = STEP_LEFT;
						}
						break;
					case MELEE0_ATK:
						Anim = MELEE1_ATK;
						break;
					case MELEE1_ATK:
						Anim = MELEE_ATK;
						break;
					case MELEE_ATK:
						Anim = STANDING;
						anim_MeleeAttack = false;
						break;

					default:
						Anim = STANDING;
					}
					AnimTimer = 0.0;
				}
			}
			else
			{
				if(anim_MeleeAttack)
				{
					if(AnimTimer > AnimDuration)
					{
						switch(Anim)
						{
						case MELEE0_ATK:
							Anim = MELEE1_ATK;
							break;
						case MELEE1_ATK:
							Anim = MELEE_ATK;
							break;
						case MELEE_ATK:
							Anim = STANDING;
							anim_MeleeAttack = false;
							break;
						default:
							Anim = MELEE0_ATK;
						}
						AnimTimer = 0.0;
					}
				}
				else
				{
					Anim = STANDING;
				}
			}

			AnimTimer += dt;
		}

		if(anim_Dead)
		{
			AnimObject->SetTextureIndex(DeadAnim * 10 + DEAD_ANIM_ID);
		}
		else
		{
			if(anim_MeleeAttack)
			{
				/*A 3-nak a kivonása azé van, mert a támadási animáció 3-5-ig van.*/
				AnimObject->SetTextureIndex((Anim-3)*10 + Direction + 6);
			}
			else
			{
				//Object->GetRect()->SetTexture(Textures[Anim][Direction]);
				AnimObject->SetTextureIndex(Anim*10 + Direction);
			}
		}
		//std::cout << AnimTimer << std::endl;

	}

public:

	GLuint GetPortrait()
	{
		return AnimObject->GetTexture(5);//Textures[0][5];
	}

	lGM_Character(lprotoAnimObject *anim_object/*,GLuint textures[][10]*/,int species,bool alive,bool invincible,bool targetable,float max_hp)
		:lGM_Entity(anim_object,species,alive,invincible,targetable,max_hp),AnimObject(anim_object)
	{
		anim_Up      = false;
		anim_Down    = false;
		anim_Left    = false;
		anim_Right   = false;

		anim_Moving		 = false;
		anim_MeleeAttack = false;
		anim_Dead 		 = false;

		AnimLeft	= true;

		Direction	= FRONT;
		Anim		= STANDING;
		DeadAnim 	= DEAD_BACK;
		/*
		for(int i=0;i < 3;i++)
		{
			for(int j=0;j < 10;j++)
			{
				Textures[i][j] = textures[i][j];
			}
		}
		*/
	}
};

#endif
