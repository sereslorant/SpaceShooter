#ifndef LJRPG_HERO_H
#define LJRPG_HERO_H

#include "../lGM_Agents/lGM_Fighter.h"

class lJRPG_Hero : public lGM_Fighter
{
public:
	class liAbility
	{
	public:
		virtual float GetCooldown() const = 0;
		virtual void Update(float dt) = 0;
		virtual void Execute(float dt) = 0;

		//TMP!!!!!!!!!!!!!!
		virtual bool IsExecuted() = 0;
		//TMP!!!!!!!!!!!!!!

		liAbility(){}
		virtual ~liAbility(){}
	};

protected:

	static constexpr unsigned int NUM_ABILITIES = 3;
	unsigned int CurrentAbility = 0;

	liAbility *Abilities[NUM_ABILITIES];

	bool c_ExecuteAbility = false;

	virtual void ApplyControls(float dt)
	{
		lGM_Fighter::ApplyControls(dt);

		if(Alive)
		{
			if(c_ExecuteAbility && Abilities[CurrentAbility] != nullptr)
			{
				Abilities[CurrentAbility]->Execute(dt);
			}

			for(int i=0;i < NUM_ABILITIES;i++)
			{
				if(Abilities[CurrentAbility] != nullptr)
				{
					Abilities[CurrentAbility]->Update(dt);
				}
			}
		}
	}

public:

	const liAbility *GetAbility(unsigned int ability_id)
	{
		if(ability_id < NUM_ABILITIES)
		{
			return Abilities[ability_id];
		}
		else
		{
			return Abilities[NUM_ABILITIES - 1];
		}
	}

	void SetExecuteAbility(bool execute_ability)
	{
		c_ExecuteAbility = execute_ability;
	}

	bool GetExecuteAbility()
	{
		return c_ExecuteAbility;
	}

	void SetCurrentAbility(unsigned int ability)
	{
		if(ability < NUM_ABILITIES)
		{
			CurrentAbility = ability;
		}
		else
		{
			CurrentAbility = NUM_ABILITIES - 1;
		}
	}

	lJRPG_Hero(	lprotoAnimObject *anim_object,lprotoObject *melee_reach,int species,bool alive,bool invincible,bool targetable,
				float max_hp,float hp_regen_rate,float speed,float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,liGM_ProjectileFactory *projectile_factory)
				:lGM_Fighter(anim_object,melee_reach,species,alive,invincible,targetable,
				max_hp,hp_regen_rate,speed,melee_recharge_speed,melee_damage,ranged_recharge_speed,ranged_damage,projectile_factory)
	{
		for(unsigned int i=0;i < NUM_ABILITIES;i++)
		{
			Abilities[i] = nullptr;
		}
	}

	virtual ~lJRPG_Hero() override
	{
		for(int i=0;i < NUM_ABILITIES;i++)
		{
			delete Abilities[i];
		}
	}
};

/*
 * Ami képességeket neki szánok:
 * -Egy ultranagy csapás, ami az előtte lévő valamekkora szögben elhelyezkedő embereket megcsapja.
 * -Egy térsebző képesség, ami egy bizonyos időtartamon keresztül a körülötte lévő ellenségek közül
 *  random választ egyet, és megcsapja, ezt csinálja N ütésen keresztül
 * -Hiperszuperenergiapajzs
 *
 */
class lJRPG_Raiden : public lJRPG_Hero
{
protected:

	class lJRPG_SuperAttack : public liAbility
	{
	private:
		lJRPG_Hero *Hero;
		lGM_Sensor *Range;

		float Cooldown = 1.0;
		float CooldownSpeed = 0.5;

		float Damage = 300.0;

		//TMP!!!!!!!!!!!!!!!
		unsigned int Counter = 0;
		bool Executed = false;
		//TMP!!!!!!!!!!!!!!!

	public:

		virtual float GetCooldown() const override
		{
			return Cooldown;
		}

		virtual void Update(float dt) override
		{
			if(Cooldown < 1.0)
			{
				Cooldown += CooldownSpeed * dt;
			}

			if(Counter == 0)
			{
				Executed = false;
			}
			else
			{
				Counter--;
			}

			Range->Targets.clear();
			Range->SetPosition(Hero->GetPosition());
		}

		virtual void Execute(float dt) override
		{
			if(Cooldown >= 1.0)
			{
				for(liAgent *Agent : Range->Targets)
				{
					if(Agent == nullptr)
						{continue;}

					liAgentProperty *pAgentPosition	= Agent->GetAgentProperty(lGM_Entity::POSITION_KEY);
					//liAgentProperty *AgentRadius	= Agent->GetAgentProperty(lGM_Entity::RADIUS_KEY);
					liAgentProperty *pAgentSpecies	= Agent->GetAgentProperty(lGM_Entity::SPECIES_KEY);
					liAgentProperty *pAgentAlive	= Agent->GetAgentProperty(lGM_Entity::ALIVE_KEY);
					liAgentProperty *pAgentHealth	= Agent->GetAgentProperty(lGM_Entity::HEALTH_KEY);

					if(/*AgentPosition == nullptr ||* / AgentRadius == nullptr ||*/ pAgentSpecies == nullptr || pAgentAlive == nullptr || pAgentHealth == nullptr)
					{
						continue;
					}

					liAgentVectorProperty	*AgentPosition	= pAgentPosition->ToVector();
					liAgentIntProperty		*AgentSpecies	= pAgentSpecies->ToInt();
					liAgentBoolProperty		*AgentAlive		= pAgentAlive->ToBool();
					liAgentFloatProperty	*AgentHealth	= pAgentHealth->ToFloat();

					if(AgentSpecies->Get() == Hero->GetSpecies() || AgentSpecies->Get() == 0)
					{
						continue;
					}

					lmVector2D Pos_I_Rel =  AgentPosition->Get() - Hero->GetObject()->GetPosition();
					Pos_I_Rel.Normalize();

					lmScalar Dot = lmDot(Pos_I_Rel,Hero->GetDir());

					if(Dot > (1.0/2.0)) //Ideiglenesen a sebzés szögét 45 fokra állítjuk.
					{
						AgentHealth->Set(AgentHealth->Get() - Damage);
					}
				}

				Cooldown = 0.0;

				Counter = 20;
				Executed = true;
			}
		}

		virtual bool IsExecuted() override
		{
			return Executed;
		}

		lJRPG_SuperAttack(lJRPG_Hero *hero,lprotoObject *range)
			:Hero(hero)
		{
			lGM_Sensor::lGM_SensorPredicate *SensorPredicate = new lGM_EnemyDetectorPredicate(hero);
			Range = new lGM_Sensor(range,SensorPredicate);
		}

		virtual ~lJRPG_SuperAttack() override
		{

		}

	};




	/*TMP!!!!!!!!!!!!!!!!
	*/
	float SuperAttackRange;
	/*TMP!!!!!!!!!!!!!!!!
	*/

public:

	lJRPG_Raiden(lprotoAnimObject *anim_object,lprotoObject *melee_reach,int species,bool alive,bool invincible,bool targetable,
				 float max_hp,float hp_regen_rate,float speed,float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,
				 float ranged_damage,liGM_ProjectileFactory *projectile_factory,
				 lprotoObject *super_attack_range)
		:lJRPG_Hero(anim_object,melee_reach,species,alive,invincible,targetable,
				max_hp,hp_regen_rate,speed,melee_recharge_speed,melee_damage,ranged_recharge_speed,ranged_damage,projectile_factory)
	{
		Abilities[0] = new lJRPG_SuperAttack(this,super_attack_range);
		SuperAttackRange = super_attack_range->GetRadius();
	}

	virtual ~lJRPG_Raiden()
	{}

	virtual void DBG_Draw() override
	{
		lGM_Fighter::DBG_Draw();

		if(Alive && Abilities[0]->IsExecuted())
		{
			glColor4f(0.0,0.0,1.0,1.0);

			glBegin(GL_LINE_LOOP);

			float Angle = acos(lmDot(c_Dir,lmVector2D(1.0,0.0)));

			int SgnY;
			if(c_Dir.Y < 0){SgnY = -1;}else{SgnY = 1;}

			float PI = 3.14159265359;

			float Rad45 = 45.0 * PI / 180.0;

			const unsigned int N = 8;
			float dAngle = 2.0*Rad45 / N;

			glVertex2f(Object->GetPosition().X,Object->GetPosition().Y);

			for(int i=0;i <= N;i++)
			{
				glVertex2f(Object->GetPosition().X + SuperAttackRange * cos(Angle + Rad45 - i*dAngle),Object->GetPosition().Y + SgnY * SuperAttackRange * sin(Angle + Rad45 - i*dAngle));
			}

			glEnd();
		}
	}
};

#endif // LJRPG_HERO_H
