#ifndef LGM_BULLET_FACTORY_H
#define LGM_BULLET_FACTORY_H

#include "lGM_Projectile.h"

#include "../lprotoGameMode.h"

class lGM_BulletFactory : public liGM_ProjectileFactory
{
private:
	lprotoMap &Map;
    std::list<liAgent *> &Agents;

public:

	virtual lGM_Projectile *CreateProjectile(const lmVector2D &position,float proj_size,const lmVector2D &velocity,int species,float damage,float kick)
	{
		lprotoObject *ProjObject = new lprotoObject(position,lmVector2D(proj_size/2.0,proj_size/2.0),proj_size,proj_size,lColor(1.0,1.0,1.0,1.0),0,4.0,lprotoObject::GHOST);
		lGM_Projectile *Projectile = new lGM_Projectile(ProjObject,velocity,species,damage,kick);

		Map.AddObject(ProjObject);
		Agents.push_back(Projectile);
		
		return Projectile;
	}

	lGM_BulletFactory(lprotoMap &p_map,std::list<liAgent *> &agents)
		:Map(p_map),Agents(agents)
	{}
	virtual ~lGM_BulletFactory() override{}
};

#endif // LGM_BULLET_FACTORY_H
