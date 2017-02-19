#ifndef LJRPG_PROJECTILE
#define LJRPG_PROJECTILE

#include "lGM_Entity.h"
#include "../lprotoCollisionCallback.h"



class lGM_Projectile : public lGM_Entity
{
private:

	class lGM_ProjectileCollisionCallback : public lprotoCollisionCallback
	{
	private:
		lGM_Projectile *Projectile;

	public:
		lGM_ProjectileCollisionCallback(lGM_Projectile *projectile)
			:Projectile(projectile)
		{

		}

		virtual void Callback(lprotoObject *obj)
		{
			liAgent *Target = (liAgent *)obj->GetUserData();

			liAgentProperty *pTargetAlive	= nullptr;
			liAgentProperty *pTargetSpecies	= nullptr;
			liAgentProperty *pTargetHealth	= nullptr;

			if(Target != nullptr)
			{
				pTargetAlive	= Target->GetAgentProperty(lGM_Entity::ALIVE_KEY);
				pTargetSpecies	= Target->GetAgentProperty(lGM_Entity::SPECIES_KEY);
				pTargetHealth	= Target->GetAgentProperty(lGM_Entity::HEALTH_KEY);
			}

			bool ProjectileDies = false;
			if(Target != nullptr && pTargetAlive != nullptr && pTargetSpecies != nullptr && pTargetHealth != nullptr)
			{
				liAgentBoolProperty		*TargetAlive	= pTargetAlive->ToBool();
				liAgentIntProperty		*TargetSpecies	= pTargetSpecies->ToInt();
				liAgentFloatProperty	*TargetHealth	= pTargetHealth->ToFloat();

				if(Projectile->Alive && (TargetSpecies->Get() != Projectile->Species && TargetAlive->Get()))
				{
					if(TargetSpecies->Get() != 0)
					{
						TargetHealth->Set(TargetHealth->Get() - Projectile->GetDamage());
						Projectile->Alive = false;
					}

					if(obj->GetCollisionResponse() == lprotoObject::MOVABLE)
					{
						lmVector2D ProjectileDirection = Projectile->Object->GetVelocity();
						ProjectileDirection.Normalize();

						obj->AddVelocity(ProjectileDirection * Projectile->GetKick());

						Projectile->Alive = false;
					}

					if(obj->GetCollisionResponse() == lprotoObject::IMMOVABLE)
					{
						Projectile->Alive = false;
					}
				}
			}
			else
			{

				if(obj->GetCollisionResponse() == lprotoObject::MOVABLE)
				{
					lmVector2D ProjectileDirection = Projectile->Object->GetVelocity();
					ProjectileDirection.Normalize();

					obj->AddVelocity(ProjectileDirection * Projectile->GetKick());

					Projectile->Alive = false;
				}

				if(obj->GetCollisionResponse() == lprotoObject::IMMOVABLE)
				{
					Projectile->Alive = false;
				}
			}
			/*
			if(ProjectileDies)
			{
				Projectile->Alive = false;
				//Projectile->Object->SetRemovable(true);
				//Projectile->Removable = true;
			}
			*/
		}
	};

protected:
	lmVector2D ProjVelocity;

	float Damage;
	float Kick;

public:

	float GetDamage()
	{
		return Damage;
	}

	float GetKick()
	{
		return Kick;
	}

	virtual void ExecuteBehavior(float dt) override
	{
		if(Alive)
		{
			Object->SetVelocity(ProjVelocity);
		}
		else
		{
			Object->SetVelocity(lmVector2D::NULL_VECTOR);
			Object->SetRemovable(true);

			Removable = true;
		}
	}

	lGM_Projectile(lprotoObject *object,const lmVector2D &proj_velocity,int species,float damage,float kick)
		:lGM_Entity(object,species,true,false,false,0.0),ProjVelocity(proj_velocity),Damage(damage),Kick(kick)
	{
		Object->SetCollisionCallback(new lGM_ProjectileCollisionCallback(this));
	}

	virtual ~lGM_Projectile() override
	{

	}
};

class liGM_ProjectileFactory
{
public:
	virtual lGM_Projectile *CreateProjectile(const lmVector2D &position,float proj_size,const lmVector2D &velocity,int species,float damage,float kick) = 0;

	liGM_ProjectileFactory(){}
	virtual ~liGM_ProjectileFactory(){}
};

#endif // LJRPG_PROJECTILE
