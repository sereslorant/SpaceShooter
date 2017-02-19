#ifndef LJRPG_COMPANION_H
#define LJRPG_COMPANION_H

#include "../lGM_Agents/AI/lGM_AI.h"
#include "../lGM_Agents/lGM_Fighter.h"

class lJRPG_Companion : public lGM_AI
{
protected:

	/*TMP!!!!!!!!!!!!!!!!!!*/
	lprotoMap &proto_map;
	/*TMP!!!!!!!!!!!!!!!!!!*/

	//lGM_Fighter *Avatar;

	lGM_Fighter *Player;
	float PlayerDist;

	liAgent *Target;

	bool Enabled;

public:

	virtual void Control(float dt) override
	{
		if(Enabled)
		{
			//for(liAgent *Agent : agents)
			for(lprotoObject *Obj : proto_map.GetObjects())
			{
				liAgent *Agent = (liAgent *)Obj->GetUserData();
				if(Agent == nullptr)
					{continue;}

				liAgent::liAgentProperty *pAgentPosition	= Agent->GetAgentProperty(lGM_Entity::POSITION_KEY);
				liAgent::liAgentProperty *pAgentRadius		= Agent->GetAgentProperty(lGM_Entity::RADIUS_KEY);
				liAgent::liAgentProperty *pAgentSpecies		= Agent->GetAgentProperty(lGM_Entity::SPECIES_KEY);
				liAgent::liAgentProperty *pAgentAlive		= Agent->GetAgentProperty(lGM_Entity::ALIVE_KEY);
				liAgent::liAgentProperty *pAgentTargetable	= Agent->GetAgentProperty(lGM_Entity::TARGETABLE_KEY);
				liAgent::liAgentProperty *pAgentHealth		= Agent->GetAgentProperty(lGM_Entity::HEALTH_KEY);

				if(pAgentPosition == nullptr || pAgentRadius == nullptr || pAgentSpecies == nullptr || pAgentAlive == nullptr || pAgentTargetable == nullptr || pAgentHealth == nullptr)
				{
					continue;
				}

				liAgent::liAgentVectorProperty	*AgentPosition	 = pAgentPosition->ToVector();
				liAgent::liAgentFloatProperty	*AgentRadius	 = pAgentRadius->ToFloat();
				liAgent::liAgentIntProperty		*AgentSpecies	 = pAgentSpecies->ToInt();
				liAgent::liAgentBoolProperty	*AgentAlive		 = pAgentAlive->ToBool();
				liAgent::liAgentBoolProperty	*AgentTargetable = pAgentTargetable->ToBool();
				liAgent::liAgentFloatProperty	*AgentHealth	 = pAgentHealth->ToFloat();

				if(AgentPosition == nullptr || AgentRadius == nullptr || AgentSpecies == nullptr || AgentAlive == nullptr || AgentTargetable == nullptr || AgentHealth == nullptr)
				{
					continue;
				}

				/*
				 * A legközelebbit támadják be
				 */
				if(AgentTargetable->Get() && AgentSpecies->Get() != Avatar->GetSpecies() && (AgentSpecies->Get() != 0) && AgentAlive->Get())
				{
					if(Target != nullptr)
					{
						liAgent::liAgentProperty		*pTargetPosition	= Target->GetAgentProperty(lGM_Entity::POSITION_KEY);
						liAgent::liAgentVectorProperty	*TargetPosition		= pTargetPosition->ToVector();

						lmScalar TargetDistSqr	= (TargetPosition->Get() - Avatar->GetObject()->GetPosition()).LengthSquared();
						lmScalar ObjDistSqr		= (AgentPosition->Get()  - Avatar->GetObject()->GetPosition()).LengthSquared();

						if(ObjDistSqr < TargetDistSqr)
						{
							Target = Agent;
						}
					}
					else
					{
						Target = Agent;
					}
				}
			}

			if(Target != nullptr)
			{
				liAgent::liAgentProperty *pTargetPosition	= Target->GetAgentProperty(lGM_Entity::POSITION_KEY);
				liAgent::liAgentProperty *pTargetRadius		= Target->GetAgentProperty(lGM_Entity::RADIUS_KEY);
				liAgent::liAgentProperty *pTargetAlive		= Target->GetAgentProperty(lGM_Entity::ALIVE_KEY);

				liAgent::liAgentVectorProperty	*TargetPosition = pTargetPosition->ToVector();
				liAgent::liAgentFloatProperty	*TargetRadius	= pTargetRadius->ToFloat();
				liAgent::liAgentBoolProperty	*TargetAlive	= pTargetAlive->ToBool();

				lmVector2D TargetVector = TargetPosition->Get() - Avatar->GetObject()->GetPosition();
				if(!lmCircleCircleIntersection(TargetPosition->Get(),TargetRadius->Get(),Avatar->GetObject()->GetPosition(),Avatar->GetReach()))
				{
					if(Avatar->AtkModeMelee())
					{
						Avatar->SwitchAttackMode();
					}

					if(TargetVector.X > 0.0)
					{
						Avatar->SetRight(true);
						Avatar->SetLeft(false);
					}
					else if(TargetVector.X < 0.0)
					{
						Avatar->SetRight(false);
						Avatar->SetLeft(true);
					}
					else
					{
						Avatar->SetRight(false);
						Avatar->SetLeft(false);
					}

					if(TargetVector.Y > 0.0)
					{
						Avatar->SetUp(false);
						Avatar->SetDown(true);
					}
					else if(TargetVector.Y < 0.0)
					{
						Avatar->SetUp(true);
						Avatar->SetDown(false);
					}
					else
					{
						Avatar->SetUp(false);
						Avatar->SetDown(false);
					}
				}
				else
				{
					if(Avatar->AtkModeRanged())
					{
						Avatar->SwitchAttackMode();
					}

					Avatar->SetRight(false);
					Avatar->SetLeft(false);
					Avatar->SetUp(false);
					Avatar->SetDown(false);
				}

				Avatar->SetAttack(true);

				TargetVector.Normalize();

				Avatar->SetDirX(TargetVector.X);
				Avatar->SetDirY(TargetVector.Y);

				if(!TargetAlive->Get())
				{
					Target = nullptr;
				}
			}
			else
			{
				Avatar->SetAttack(false);

				lmVector2D PlayerVector = Player->GetObject()->GetPosition() - Avatar->GetObject()->GetPosition();
				if(PlayerVector.LengthSquared() > (PlayerDist * PlayerDist))
				{
					if(PlayerVector.X > 0.0)
					{
						Avatar->SetRight(true);
						Avatar->SetLeft(false);
					}
					else if(PlayerVector.X < 0.0)
					{
						Avatar->SetRight(false);
						Avatar->SetLeft(true);
					}

					if(PlayerVector.Y > 0.0)
					{
						Avatar->SetUp(false);
						Avatar->SetDown(true);
					}
					else if(PlayerVector.Y < 0.0)
					{
						Avatar->SetUp(true);
						Avatar->SetDown(false);
					}

					PlayerVector.Normalize();

					Avatar->SetDirX(PlayerVector.X);
					Avatar->SetDirY(PlayerVector.Y);
				}
				else
				{
					Avatar->SetRight(false);
					Avatar->SetLeft(false);
					Avatar->SetUp(false);
					Avatar->SetDown(false);
				}
			}
		}
	}

	void Enable()
	{
		Enabled = true;
	}

	void Disable()
	{
		Enabled = false;
	}

	lJRPG_Companion(lGM_Fighter *avatar,lGM_Fighter *player,float player_dist,bool enabled,lprotoMap &p_proto_map)
		:lGM_AI(avatar),
		/*PROTO_MAP*/
			proto_map(p_proto_map),
		 /*PROTO_MAP*/
		 Player(player),PlayerDist(player_dist),Target(nullptr),Enabled(enabled)
	{}
};

#endif // LJRPG_COMPANION_H
