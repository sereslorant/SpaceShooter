#ifndef LJRPG_FIGHTER
#define LJRPG_FIGHTER

#include "lGM_Character.h"

#include "lGM_Sensor.h"
#include "lGM_Projectile.h"

class lGM_AI;

class lGM_Fighter : public lGM_Character
{
protected:

	lGM_AI *AI = nullptr;

	bool c_Up;
	bool c_Down;
	bool c_Left;
	bool c_Right;

	bool c_Sprint;

	bool c_AttackMode;
	bool c_Attack;

	float Speed;
	float SprintRatio = 2.0;

	lmVector2D c_Dir;

	float AtkModeReady;
	float AtkModeRechargeSpeed = 1.5;

	//const float MaxHP;
	float HPRegenRate;
	//float HP;

	//lprotoObject *MeleeReach;
	lGM_Sensor *MeleeReach;

	float MeleeDamage;
	float MeleeReady;
	float MeleeRechargeSpeed;

	float RangedDamage;
	float RangedKick = 100;
	float RangedReady;
	float RangedRechargeSpeed;

	liGM_ProjectileFactory *ProjectileFactory;

	void MeleeAttack()
	{
		if(MeleeReady >= 1.0)
		{
			//TriggerMeleeAnim();

			for(liAgent *Target : MeleeReach->Targets)
			{
				if(Target == nullptr)
					{continue;}

				liAgentProperty *pAgentPosition	= Target->GetAgentProperty(lGM_Entity::POSITION_KEY);
				//liAgentProperty *AgentRadius	= Agent->GetAgentProperty(lGM_Entity::RADIUS_KEY);
				liAgentProperty *pAgentSpecies	= Target->GetAgentProperty(lGM_Entity::SPECIES_KEY);
				liAgentProperty *pAgentAlive	= Target->GetAgentProperty(lGM_Entity::ALIVE_KEY);
				liAgentProperty *pAgentHealth	= Target->GetAgentProperty(lGM_Entity::HEALTH_KEY);

				if(/*AgentPosition == nullptr ||* / AgentRadius == nullptr ||*/ pAgentSpecies == nullptr || pAgentAlive == nullptr || pAgentHealth == nullptr)
				{
					continue;
				}

				liAgentVectorProperty	*AgentPosition	= pAgentPosition->ToVector();
				liAgentIntProperty		*AgentSpecies	= pAgentSpecies->ToInt();
				liAgentBoolProperty		*AgentAlive		= pAgentAlive->ToBool();
				liAgentFloatProperty	*AgentHealth	= pAgentHealth->ToFloat();

				if(AgentSpecies->Get() == Species || AgentSpecies->Get() == 0)
				{
					continue;
				}

				lmVector2D Pos_I_Rel =  AgentPosition->Get() - Object->GetPosition();
				Pos_I_Rel.Normalize();

				lmScalar Dot = lmDot(Pos_I_Rel,c_Dir);

				if(Dot > (1.0/2.0)) //Ideiglenesen a sebzés szögét 45 fokra állítjuk.
				{
					AgentHealth->Set(AgentHealth->Get() - MeleeDamage);
				}
			}

			MeleeReady = 0.0;
		}
	}

	void RangedAttack()
	{
		if(RangedReady >= 1.0)
		{
			if(ProjectileFactory != nullptr)
			{
				float ProjectileSpeed = 400.0;
				lmVector2D ProjVelocity = c_Dir * ProjectileSpeed;

				float ProjectileSize = 8.0;

				ProjectileFactory->CreateProjectile(Object->GetPosition(),ProjectileSize,ProjVelocity,Species,RangedDamage,RangedKick);

				RangedReady = 0.0;
			}
		}
	}

	virtual void SetAnimation()
	{
		if(Alive)
		{
			if(c_Dir.Y > (1.0/2.0))
				{anim_Up = true;}
			else
				{anim_Up = false;}

			if(c_Dir.Y < (-1.0/2.0))
				{anim_Down = true;}
			else
				{anim_Down = false;}

			if(c_Dir.X > (1.0/2.0))
				{anim_Right = true;}
			else
				{anim_Right = false;}

			if(c_Dir.X < (-1.0/2.0))
				{anim_Left = true;}
			else
				{anim_Left = false;}

			if(c_Up)
				{anim_Moving = true;}

			if(c_Down)
				{anim_Moving = true;}

			if(c_Left)
				{anim_Moving = true;}

			if(c_Right)
				{anim_Moving = true;}

			if((c_Up == false && c_Down == false && c_Left == false && c_Right == false) ||
			   (c_Up == true  && c_Down == true  && c_Left == false && c_Right == false) ||
			   (c_Up == false && c_Down == false && c_Left == true  && c_Right == true)
			  )
			{anim_Moving = false;}

			if(c_Attack)
			{
				if(c_AttackMode)
				{
					if(MeleeReady >= 1.0)
						{TriggerMeleeAnim();}
				}
				else
				{
					//RangedAttack();
				}
			}
			else
			{
				if(c_AttackMode)
				{
					//anim_MeleeAttack = false;
				}
				else
				{

				}
			}
		}
		else
		{
			anim_Dead = true;
		}
	}

	virtual void ApplyControls(float dt)
	{
		if(Alive)
		{
			lmVector2D Velocity;

			if(c_Up)
			{
				Velocity.Y -= Speed;
			}
			if(c_Down)
			{
				Velocity.Y += Speed;
			}
			if(c_Left)
			{
				Velocity.X -= Speed;
			}
			if(c_Right)
			{
				Velocity.X += Speed;
			}

			if(c_Sprint)
			{
				Velocity *= SprintRatio;
			}

			Object->AddVelocity(Velocity);

			if(c_Attack)
			{
				if(c_AttackMode)
					{MeleeAttack();}
				else
					{RangedAttack();}
			}
			else
			{
				if(c_AttackMode)
				{}
				else
				{}
			}
		}
	}

public:

	void SetAI(lGM_AI *ai)
	{
		AI = ai;
	}

	virtual void ExecuteBehavior(float dt) override;

	//Vezérlő függvények

	void SetUp(bool up)
	{
		c_Up = up;
	}

	void SetDown(bool down)
	{
		c_Down = down;
	}

	void SetLeft(bool left)
	{
		c_Left = left;
	}

	void SetRight(bool right)
	{
		c_Right = right;
	}

	bool GetUp()
	{
		return c_Up;
	}

	bool GetDown()
	{
		return c_Down;
	}

	bool GetLeft()
	{
		return c_Left;
	}

	bool GetRight()
	{
		return c_Right;
	}

	void SetSprint(bool sprint)
	{
		c_Sprint = sprint;
	}

	void SwitchAttackMode()
	{
		if(AtkModeReady >= 1.0)
		{
			c_AttackMode = !c_AttackMode;
			AtkModeReady = 0.0;
		}
	}

	bool AtkModeRanged()
	{
		return c_AttackMode == false;
	}

	bool AtkModeMelee()
	{
		return c_AttackMode == true;
	}

	void SetAttack(bool attack)
	{
		c_Attack = attack;
	}

	void SetDirX(float x)
	{
		c_Dir.X = x;
	}

	void SetDirY(float y)
	{
		c_Dir.Y = y;
	}

	const lmVector2D &GetDir()
	{
		return c_Dir;
	}

	//Getterek/setterek

	float GetSpeed()
	{
		if(c_Sprint)
		{
			return Speed * SprintRatio;
		}
		else
		{
			return Speed;
		}
	}

	float GetMaxHP()
	{
		return MaxHP;
	}

	float GetHP()
	{
		return HP;
	}

	float GetReach()
	{
		return MeleeReach->GetRange()->GetRadius();
	}

	lGM_Fighter(lprotoAnimObject *anim_object,lprotoObject *melee_reach,int species,bool alive,bool invincible,bool targetable,
				  float max_hp,float hp_regen_rate,float speed,float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,liGM_ProjectileFactory *projectile_factory)
		:lGM_Character(anim_object,species,alive,invincible,targetable,max_hp),Speed(speed),
		 HPRegenRate(hp_regen_rate),MeleeRechargeSpeed(melee_recharge_speed),MeleeDamage(melee_damage),RangedRechargeSpeed(ranged_recharge_speed),RangedDamage(ranged_damage),ProjectileFactory(projectile_factory)
	{
		c_Up	= false;
		c_Down	= false;
		c_Left	= false;
		c_Right	= false;

		c_Sprint = false;

		c_AttackMode	= false;
		c_Attack		= false;

		c_Dir = lmVector2D(0.0,1.0);

		anim_Dead = !Alive;

		AtkModeReady = 1.0;
		MeleeReady = 1.0;
		RangedReady = 1.0;

		//MeleeCallback = new lGM_MeleeAttackCallback(this);
		//MeleeReach->SetCollisionCallback(MeleeCallback);

		lGM_Sensor::lGM_SensorPredicate *SensorPredicate = new lGM_EnemyDetectorPredicate(this);
		MeleeReach = new lGM_Sensor(melee_reach,SensorPredicate);
	}

	virtual ~lGM_Fighter() override
	{
		delete AI;
	}

	virtual void DBG_Draw() override
	{
		glColor4f(0.0,1.0,0.0,1.0);

		glBegin(GL_LINE_LOOP);

		float PI = 3.14159265359;
		const unsigned int N = 8;
		float dAngle = 2.0*PI / N;

		for(int i=0;i < N;i++)
		{
			glVertex2f(Object->GetPosition().X + GetReach() * cos(i*dAngle),Object->GetPosition().Y + GetReach() * sin(i*dAngle));
		}

		glEnd();

		const float DIR_LEN = 40;

		c_Dir.Normalize();

		glBegin(GL_LINES);

		glVertex2f( Object->GetPosition().X,
					Object->GetPosition().Y);

		glVertex2f(Object->GetPosition().X + DIR_LEN*c_Dir.X,
					Object->GetPosition().Y + DIR_LEN*c_Dir.Y);

		glEnd();
	}
};

void LoadCharacterAnim(RM &resource_manager,const std::string &name,GLuint Textures[3][10]);

#endif // LJRPG_FIGHTER
