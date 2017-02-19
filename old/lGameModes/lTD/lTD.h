#ifndef L_TD_H
#define L_TD_H

#include "../lprotoGameMode.h"

#include "lTD_Base.h"

#include "../../lMath/lmGeometry2D.h"



#include "../lGM_Agents/lGM_BulletFactory.h"



#include "../lGM_Agents/lGM_Fighter.h"
#include "../lGM_Agents/AI/lGM_AI.h"
#include "../lGM_Agents/AI/lGM_AICommand/lGM_AICommand_Goto.h"

class liTD_UnitFactory
{
public:
	virtual lGM_Fighter *PlaceUnit(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,const std::string &textures,float radius,int species,float max_hp,float melee_range,float hp_regen_rate,float speed,
							float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,liGM_ProjectileFactory *projectile_factory) = 0;

	virtual lGM_AI *CreateAI(lGM_Fighter *unit/*,lTD_Base *base*/) = 0;

	liTD_UnitFactory(){}
	virtual ~liTD_UnitFactory(){}
};

class lTD_AgentFactory : public liTD_UnitFactory
{
private:
	lprotoMap &Map;
	std::list<liAgent *> &Agents;
	RM &ResourceManager;

public:

	virtual lGM_Fighter *PlaceUnit(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,const std::string &textures,float radius,int species,float max_hp,float melee_range,float hp_regen_rate,float speed,
							float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,liGM_ProjectileFactory *projectile_factory) override
	{
		GLuint Textures[3][10];

        LoadCharacterAnim(ResourceManager,textures,Textures);

        lprotoAnimObject *Object = new lprotoAnimObject(position,center,width,height,color,(GLuint *)&Textures[0],3*10,radius,lprotoObject::MOVABLE);
        Map.AddObject(Object);

        lprotoObject *MeleeRange = new lprotoObject(position,lmVector2D(melee_range/2.0,melee_range/2.0),melee_range,melee_range,lColor(0.0,0.0,0.0,0.0),0,melee_range,lprotoObject::GHOST);
        Map.AddObject(MeleeRange);

        lGM_Fighter *Unit = new lGM_Fighter(Object,MeleeRange,species,true,false,true,max_hp,hp_regen_rate,speed,melee_recharge_speed,melee_damage,ranged_recharge_speed,ranged_damage,projectile_factory);
        //lprotoAnimObject *anim_object,lprotoObject *melee_reach,int species,bool alive,bool invincible,bool targetable,float max_hp,float hp_regen_rate,float speed,float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,liGM_ProjectileFactory *projectile_factory
        Agents.push_back(Unit);

        return Unit;
	}

	virtual lGM_AI *CreateAI(lGM_Fighter *unit/*,lTD_Base *base*/) override
	{
        lGM_AI *AI = new lGM_AI(unit/*,base*/);//new lTD_DemoAI(Unit,Szercsy);

        /*if(Szercsy)
		{
			lTD_UnitCommand *Command = new lTD_AICommand_Goto(Unit,lmVector2D(100,500));
			AI->SetCommand(Command);
		}
		else
		{
			lmVector2D AI_Path[] = {{700,500},{650,450},{600,450},{450,600},{700,500}};

			lTD_UnitCommand *Command = new lTD_AICommand_FollowPath(Unit,AI_Path,5);
			AI->SetCommand(Command);
		}*/

        //Agents.push_back(AI);

        return AI;
	}

	lTD_AgentFactory(lprotoMap &p_map,std::list<liAgent *> &agents,RM &resource_manager)
		:Map(p_map),Agents(agents),ResourceManager(resource_manager)
	{}

	~lTD_AgentFactory()
	{}
};

#include "../lGame/lTimer.h"

class lTD_EnemySpawnPoint : public liAgent
{
private:
	lmVector2D Position;
	const int ENEMY_SPECIES = 2;
	lTD_Base *Base;

	liTD_UnitFactory *UnitFactory;

	lTimer *Timer;

	class lTD_PlaceUnitCallback : public liEventCallback
	{
	private:
		lTD_EnemySpawnPoint *SpawnPoint;

	public:

		virtual void Callback() override
		{
			float UnitSize = 32.0;
			lmVector2D UnitCenter(16.0,16.0);
			float UnitRadius = 16.0;

			float UnitMeleeRange = 26.0;

			float UnitHp = 200.0;
			float UnitHpRegenRate = 20.0;
			float UnitSpeed = 100.0;

			float UnitMeleeRechargeSpeed = 2.0;
			float UnitMeleeDamage = 40.0;
			float UnitRangedRechargeSpeed = 4.0;
			float UnitRangedDamage = 20.0;

			lGM_Fighter *Unit = SpawnPoint->UnitFactory->PlaceUnit(SpawnPoint->Position,UnitCenter,UnitSize,UnitSize,lColor(1.0,1.0,1.0,1.0),"Enemy",UnitRadius,
																	SpawnPoint->ENEMY_SPECIES,UnitHp,UnitMeleeRange,UnitHpRegenRate,UnitSpeed,
																	UnitMeleeRechargeSpeed,UnitMeleeDamage,UnitRangedRechargeSpeed,UnitRangedDamage,nullptr);

			Unit->SwitchAttackMode();

			lGM_AI *AI = SpawnPoint->UnitFactory->CreateAI(Unit);

			lGM_AICommand *AttackCommand = new lGM_AICommand_Attack(AI,false,SpawnPoint->Base,120.0);
			AI->SetCommand(AttackCommand);

			lmVector2D AI_Path[] = {{700,700},{550,500},{450,450}};
			lGM_AICommand *FollowPathCommand = new lGM_AICommand_FollowPath(AI,false,AI_Path,3);
			AI->SetCommand(FollowPathCommand);

			Unit->SetAI(AI);
		}

		lTD_PlaceUnitCallback(lTD_EnemySpawnPoint *spawn_point)
			:SpawnPoint(spawn_point)
		{}

		virtual ~lTD_PlaceUnitCallback() override{}
	};

	lTD_PlaceUnitCallback *PlaceUnitCallback;

public:

	virtual bool IsRemovable() override
	{
		return false;
	}

	virtual liAgentProperty *GetAgentProperty(const std::string &key) override
	{
		return nullptr;
	}

	virtual void ExecuteBehavior(float dt) override
	{
		Timer->Step(dt);
	}

	lTD_EnemySpawnPoint(const lmVector2D &position,lTD_Base *base,liTD_UnitFactory *unit_factory)
		:Position(position),Base(base),UnitFactory(unit_factory)
	{
		Timer = new lTimer(1.0,true);
		PlaceUnitCallback = new lTD_PlaceUnitCallback(this);

		Timer->GetLimitReachedEvent()->AddCallback(PlaceUnitCallback);

		Timer->Enable();
	}

	virtual ~lTD_EnemySpawnPoint() override
	{
		delete Timer;
		delete PlaceUnitCallback;
	}
};


#include "../lGM_Agents/lGM_Sensor.h"

class lTD_ObjectPicker : public liAgent
{
public:
	class liObjectPickerCommand
	{
	public:
		virtual void AddObject(lprotoObject *object) = 0;

		liObjectPickerCommand() {}
		virtual ~liObjectPickerCommand() {}
	};

private:

	enum lTD_ObjectPickerState
	{
		LTD_ACTIVATED,
		LTD_WORKING,
		LTD_IDLE,
	};

	lprotoObject *Object;

	class lTD_ObjectPickerCallback : public lprotoCollisionCallback
	{
	public:

		lTD_ObjectPickerState &ObjectPickerState;
		std::list<lprotoObject *> &Objects;

		virtual void Callback(lprotoObject *obj) override
		{
			if(ObjectPickerState == LTD_ACTIVATED)
			{
				Objects.push_back(obj);
				ObjectPickerState = LTD_WORKING;
			}
		}

		lTD_ObjectPickerCallback(lTD_ObjectPickerState &object_picker_state,std::list<lprotoObject *> &objects)
			:ObjectPickerState(object_picker_state),Objects(objects)
		{}

		virtual ~lTD_ObjectPickerCallback() override
		{

		}
	};

	lTD_ObjectPickerState ObjectPickerState;
	std::list<lprotoObject *> Objects;

	lTD_ObjectPickerCallback *ObjectPickerCallback = nullptr;
	liObjectPickerCommand *ObjectPickerCommand = nullptr;

public:

	void SetObjectPickerCommand(liObjectPickerCommand *object_picker_command)
	{
		ObjectPickerCommand = object_picker_command;
	}

	void SetPosition(const lmVector2D &position)
	{
		Object->SetPosition(position);
	}

	void Activate()
	{
		ObjectPickerState = LTD_ACTIVATED;
	}

	virtual bool IsRemovable() override
	{
		return false;
	}

	virtual liAgentProperty *GetAgentProperty(const std::string &key) override
	{
		return nullptr;
	}

	virtual void ExecuteBehavior(float dt) override
	{
		if(ObjectPickerState == LTD_WORKING)
		{
			for(lprotoObject *PickedObject : Objects)
			{
				if(ObjectPickerCommand != nullptr)
				{
					ObjectPickerCommand->AddObject(PickedObject);
				}
			}
			Objects.clear();

			ObjectPickerState = LTD_IDLE;
		}
	}

	lTD_ObjectPicker(lprotoObject *object)
		:Object(object),ObjectPickerState(LTD_IDLE)//,ObjectPickerCallback(ObjectPickerState,Objects)
	{
		ObjectPickerCallback = new lTD_ObjectPickerCallback(ObjectPickerState,Objects);
		Object->SetCollisionCallback(ObjectPickerCallback);
	}

	virtual ~lTD_ObjectPicker() override
	{}
};

class lTD_Tower : public lGM_Entity
{
protected:
	lGM_Sensor *Sensor;

	float BulletSize = 10.0;
	float BulletSpeed = 200.0;
	float BulletDamage = 50.0;
	float BulletKick = 5.0;

	float RangedRechargeSpeed = 4.0;
	float RangedReady = 1.0;

	lTD_Base *Base;
	liGM_ProjectileFactory *ProjectileFactory;

	void RangedAttack(const lmVector2D &direction)
	{
		if(RangedReady >= 1.0)
		{
			ProjectileFactory->CreateProjectile(GetPosition(),BulletSize,direction * BulletSpeed,Species,BulletDamage,BulletKick);
			RangedReady = 0.0;
		}
	}

public:

	virtual void ExecuteBehavior(float dt) override
	{
		if(RangedReady < 1.0)
		{
			RangedReady += RangedRechargeSpeed * dt;
		}

		if(Sensor->Targets.size() != 0)
		{
			//std::cout << Sensor->Targets.size() << std::endl;

			liAgent *Target = nullptr;

			lmVector2D	TargetDirection;
			lmScalar	TargetDistance = 0.0;

			for(liAgent *Agent : Sensor->Targets)
			{
				liAgentProperty *pAgentPosition = Agent->GetAgentProperty(lGM_Entity::POSITION_KEY);
				liAgentProperty *pAgentAlive	= Agent->GetAgentProperty(lGM_Entity::ALIVE_KEY);

				if(pAgentPosition == nullptr || pAgentAlive == nullptr)
				{
					continue;
				}

				liAgentVectorProperty	*AgentPosition 	= pAgentPosition->ToVector();
				liAgentBoolProperty		*AgentAlive		= pAgentAlive->ToBool();

				if(AgentPosition == nullptr || AgentAlive == nullptr)
				{
					continue;
				}

				if(!AgentAlive->Get())
				{
					continue;
				}

				lmVector2D	Direction = AgentPosition->Get() - GetPosition();

				lmVector2D	BaseDist  = AgentPosition->Get() - Base->GetPosition();
				lmScalar	Distance  = BaseDist.LengthSquared();
				if(Target == nullptr || Distance < TargetDistance)
				{
					Target = Agent;
					TargetDirection = Direction;
					TargetDistance  = Distance;
				}
			}

			if(Target != nullptr)
			{
				TargetDirection.Normalize();
				RangedAttack(TargetDirection);
			}

			Sensor->Targets.clear();
		}

		Sensor->SetPosition(Object->GetPosition());
	}

	lTD_Tower(int species,float max_health,bool invincible,lprotoObject *object,lprotoObject *range,lTD_Base *base,liGM_ProjectileFactory *projectile_factory)
		:lGM_Entity(object,species,true,invincible,true,max_health),Base(base),ProjectileFactory(projectile_factory)
	{
		//Object->SetUserData(this);

		//TowerRangeCallback = new lTD_TowerRangeCallback(this);
		//Range->SetCollisionCallback(TowerRangeCallback);

		lGM_Sensor::lGM_SensorPredicate *SensorPredicate = new lGM_EnemyDetectorPredicate(this);
		Sensor = new lGM_Sensor(range,SensorPredicate);
	}

	virtual ~lTD_Tower() override
	{
		//Range->SetCollisionCallback(nullptr);
		//delete TowerRangeCallback;

		delete Sensor;
	}
};

class liTD_TowerFactory
{
public:
	virtual lTD_Tower *PlaceTower(const lmVector2D &position,unsigned int species,lTD_Base *base,liGM_ProjectileFactory *projectile_factory) = 0;

	liTD_TowerFactory(){}
	virtual ~liTD_TowerFactory(){}
};

class lTD_TowerFactory : public liTD_TowerFactory
{
private:
	const int TOWER_PRICE = 200;

	int Points = 600;

	lprotoMap *Map;
	std::list<liAgent *> &Agents;
	RM &ResourceManager;

public:

	virtual lTD_Tower *PlaceTower(const lmVector2D &position,unsigned int species,lTD_Base *base,liGM_ProjectileFactory *projectile_factory) override
	{
		if((Points - TOWER_PRICE) < 0)
		{
			return nullptr;
		}

		float TowerHealth = 100.0;
		bool Invincible = false;

		float TowerRadius	= 50.0;
		float TowerRange	= 150.0;
		float TowerSize		= 100.0;

		for(lprotoObject *Obj : Map->GetObjects())
		{
			if((Obj->GetCollisionResponse() != lprotoObject::GHOST) && lmCircleCircleIntersection(position,TowerRadius,Obj->GetPosition(),Obj->GetRadius()))
			{
				return nullptr;
			}
		}

		GLuint TowerTexture = ResourceManager.GetImage("Content/Images/Tunderfaszcsy.bmp");

		lprotoObject *NewTowerObj = new lprotoObject(position,lmVector2D(TowerSize/2.0,TowerSize/2.0),TowerSize,TowerSize,lColor(1.0,1.0,1.0,1.0),TowerTexture,TowerRadius,lprotoObject::IMMOVABLE);
		Map->AddObject(NewTowerObj);

		lprotoObject *NewTowerRange = new lprotoObject(position,lmVector2D(TowerRange/2.0,TowerRange/2.0),TowerRange,TowerRange,lColor(0.0,0.0,0.0,0.0),0,TowerRange,lprotoObject::GHOST);
		Map->AddObject(NewTowerRange);

		lTD_Tower *NewTower = new lTD_Tower(species,TowerHealth,Invincible,NewTowerObj,NewTowerRange,base,projectile_factory);
		Agents.push_back(NewTower);

		Points -= TOWER_PRICE;
		std::cout << "Amount of points: " << Points << std::endl;

		return NewTower;
	}

	lTD_TowerFactory(lprotoMap *map,std::list<liAgent *> &agents,RM &resource_manager)
		:Map(map),Agents(agents),ResourceManager(resource_manager)
	{}

	virtual ~lTD_TowerFactory(){}
};

class lTD_Player : public liAgent
{
private:
	const unsigned int Species;

	lTD_Base *Base;
	std::list<lTD_Tower *> Towers;

	lInput *Input;

	class lTD_PlayerObjectPickerCommand : public lTD_ObjectPicker::liObjectPickerCommand
	{
	private:
		lTD_Player *Player;

		bool CheckBase(liAgent *picked_agent)
		{
			if(picked_agent == Player->Base)
			{
				std::cout << "Base chosen!" << std::endl;
				return true;
			}

			return false;
		}

		bool CheckTower(liAgent *picked_agent)
		{
			unsigned int i = 0;
			for(lTD_Tower *Tower : Player->Towers)
			{
				if(picked_agent == Tower)
				{
					std::cout << "Tower " << i << " chosen!" << std::endl;
					return true;
				}
				i++;
			}

			return false;
		}

	public:

		virtual void AddObject(lprotoObject *object) override
		{
			liAgent *PickedAgent = (liAgent *)object->GetUserData();

			if(PickedAgent != nullptr)
			{
				if(CheckBase(PickedAgent))
				{
					return;
				}

				if(CheckTower(PickedAgent))
				{
					return;
				}
			}
		}

		lTD_PlayerObjectPickerCommand(lTD_Player *player)
			:Player(player)
		{}

		virtual ~lTD_PlayerObjectPickerCommand() override {}
	};

	liGM_ProjectileFactory *ProjectileFactory;

	liTD_TowerFactory *TowerFactory;

	lTD_ObjectPicker *ObjectPicker;
	lTD_PlayerObjectPickerCommand PlayerObjectPickerCommand;

public:

	virtual bool IsRemovable() override
	{
		return false;
	}

	virtual liAgentProperty *GetAgentProperty(const std::string &key) override
	{
		return nullptr;
	}

	virtual void ExecuteBehavior(float dt) override
	{
		if(Input->GetRmb())
		{
			float X = Input->GetX();
			float Y = Input->GetY();

			lTD_Tower *Tower = TowerFactory->PlaceTower(lmVector2D(X,Y),Species,Base,ProjectileFactory);

			if(Tower != nullptr)
			{
				Towers.push_back(Tower);
			}
		}
		if(Input->GetLmb())
		{
			float X = Input->GetX();
			float Y = Input->GetY();

			ObjectPicker->SetPosition({X,Y});
			ObjectPicker->Activate();

			SDL_Delay(150);
		}
	}

	lTD_Player(const unsigned int species,lTD_Base *base,lInput *input,liTD_TowerFactory *tower_factory,liGM_ProjectileFactory *projectile_factory,lTD_ObjectPicker *object_picker)
		:Species(species),Base(base),Input(input),TowerFactory(tower_factory),ProjectileFactory(projectile_factory),ObjectPicker(object_picker),PlayerObjectPickerCommand(this)
	{
		ObjectPicker->SetObjectPickerCommand(&PlayerObjectPickerCommand);
	}

	virtual ~lTD_Player() override
	{}
};

class lTD_GameMode : public lprotoGameMode
{
protected:
	const int TOWER_PRICE = 200;

	int Points = 600;

	const int PLAYER_SPECIES = 1;

	lprotoCamera PlayerCamera;

	liGM_ProjectileFactory *ProjectileFactory;

	liTD_UnitFactory *AgentFactory;
	liTD_TowerFactory *TowerFactory;

	lTD_ObjectPicker *ObjectPicker;
	lTD_Player *Player;

	lTD_Base *Base;

	virtual void Logic() override
	{
		/*
		if(Input.GetRmb())
		{
			float X = Input.GetX();
			float Y = Input.GetY();

			TowerFactory->PlaceTower(lmVector2D(X,Y),PLAYER_SPECIES,ProjectileFactory);
		}
		if(Input.GetLmb())
		{
			float X = Input.GetX();
			float Y = Input.GetY();

			ObjectPicker->SetPosition({X,Y});
			ObjectPicker->Activate();

			SDL_Delay(150);
			/*
			float UnitSize = 32.0;
			lmVector2D UnitCenter(16.0,16.0);
			float UnitRadius = 16.0;

			float UnitMeleeRange = 26.0;

			float UnitHp = 200.0;
			float UnitHpRegenRate = 20.0;
			float UnitSpeed = 100.0;

			float UnitMeleeRechargeSpeed = 2.0;
			float UnitMeleeDamage = 40.0;
			float UnitRangedRechargeSpeed = 4.0;
			float UnitRangedDamage = 20.0;

			PlaceUnit(lmVector2D(X,Y),UnitCenter,UnitSize,UnitSize,lColor(1.0,1.0,1.0,1.0),"Raiden",UnitRadius,PLAYER_SPECIES,UnitHp,UnitMeleeRange,UnitHpRegenRate,UnitSpeed,
						UnitMeleeRechargeSpeed,UnitMeleeDamage,UnitRangedRechargeSpeed,UnitRangedDamage,nullptr);
			* /
			//SDL_Delay(200);


		}*/
	}

	virtual void TMP_Render() override
	{
		glLoadIdentity();

		/*

		float PI = 3.14159265359;
		const unsigned int N = 32;
		float dAngle = 2.0*PI / N;

		glColor4f(0.0,1.0,0.0,1.0);

		glBegin(GL_LINE_LOOP);

		for(int i=0;i < N;i++)
		{
			glVertex2f(PlayerCameraControl_1->GetCamPosition().X + PlayerCameraControl_1->GetCamRadius() * cos(i*dAngle),PlayerCameraControl_1->GetCamPosition().Y + PlayerCameraControl_1->GetCamRadius() * sin(i*dAngle));
		}

		glEnd();

		if(Coop)
		{
			glBegin(GL_LINE_LOOP);

			for(int i=0;i < N;i++)
			{
				glVertex2f(PlayerCameraControl_2->GetCamPosition().X + PlayerCameraControl_1->GetCamRadius() * cos(i*dAngle),PlayerCameraControl_2->GetCamPosition().Y + PlayerCameraControl_1->GetCamRadius() * sin(i*dAngle));
			}

			glEnd();
		}
		*/

		for(auto I : Agents)
		{
			I->DBG_Draw();
		}
	}


/*
	bool Szercsy = true;

	lGM_Fighter *PlaceUnit(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,const std::string &textures,float radius,int species,float max_hp,float melee_range,float hp_regen_rate,float speed,
							float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,liGM_ProjectileFactory *projectile_factory)
	{
		GLuint Textures[3][10];

        LoadCharacterAnim(ResourceManager,textures,Textures);

        lprotoAnimObject *Object = new lprotoAnimObject(position,center,width,height,color,(GLuint *)&Textures[0],3*10,radius,lprotoObject::MOVABLE);
        Map->AddObject(Object);

        lprotoObject *MeleeRange = new lprotoObject(position,lmVector2D(melee_range/2.0,melee_range/2.0),melee_range,melee_range,lColor(0.0,0.0,0.0,0.0),0,melee_range,lprotoObject::GHOST);
        Map->AddObject(MeleeRange);

        lGM_Fighter *Unit = new lGM_Fighter(Object,MeleeRange,species,true,false,true,max_hp,hp_regen_rate,speed,melee_recharge_speed,melee_damage,ranged_recharge_speed,ranged_damage,projectile_factory);
        Agents.push_back(Unit);

        lGM_AI *AI = new lGM_AI(Unit);//new lTD_DemoAI(Unit,Szercsy);
        Unit->SetAI(AI);

        if(Szercsy)
		{
			lGM_AICommand *Command = new lGM_AICommand_Goto(AI,false,lmVector2D(100,500));
			AI->SetCommand(Command);
		}
		else
		{
			lmVector2D AI_Path[] = {{700,500},{650,450},{600,450},{450,600},{700,500}};

			lGM_AICommand *Command = new lGM_AICommand_FollowPath(AI,false,AI_Path,5);
			AI->SetCommand(Command);
		}


        Szercsy = !Szercsy;

        return Unit;
	}
/*
	void PlaceTower(const lmVector2D &position)
	{
		if((Points - TOWER_PRICE) < 0)
		{
			return;
		}

		float TowerHealth = 100.0;
		bool Invincible = false;

		float TowerRadius	= 50.0;
		float TowerRange	= 150.0;
		float TowerSize		= 100.0;

		for(lprotoObject *Obj : Map->GetObjects())
		{
			if((Obj->GetCollisionResponse() != lprotoObject::GHOST) && lmCircleCircleIntersection(position,TowerRadius,Obj->GetPosition(),Obj->GetRadius()))
			{
				return;
			}
		}

		GLuint TowerTexture = ResourceManager.GetImage("Content/Images/Tunderfaszcsy.bmp");

		lprotoObject *NewTowerObj = new lprotoObject(position,lmVector2D(TowerSize/2.0,TowerSize/2.0),TowerSize,TowerSize,lColor(1.0,1.0,1.0,1.0),TowerTexture,TowerRadius,lprotoObject::IMMOVABLE);
		Map->AddObject(NewTowerObj);

		lprotoObject *NewTowerRange = new lprotoObject(position,lmVector2D(TowerRange/2.0,TowerRange/2.0),TowerRange,TowerRange,lColor(0.0,0.0,0.0,0.0),0,TowerRange,lprotoObject::GHOST);
		Map->AddObject(NewTowerRange);

		lTD_Tower *NewTower = new lTD_Tower(PLAYER_SPECIES,TowerHealth,Invincible,NewTowerObj,NewTowerRange,ProjectileFactory);
		Agents.push_back(NewTower);

		Points -= TOWER_PRICE;
		std::cout << "Amount of points: " << Points << std::endl;
	}
*/


public:

	lTD_GameMode(RM &resource_manager,std::string &map_name,float width,float height)
		:lprotoGameMode(resource_manager,map_name),PlayerCamera(lmVector2D(width/2.0,height/2.0),0,0,width,height),Base(nullptr)
    {
    	Cameras.push_back(&PlayerCamera);
		PlayerCamera.Enable();

		AgentFactory = new lTD_AgentFactory(*Map,Agents,ResourceManager);
		TowerFactory = new lTD_TowerFactory(Map,Agents,ResourceManager);

		ProjectileFactory = new lGM_BulletFactory(*Map,Agents);

		lmVector2D BasePosition(300.0,300.0);
		float BaseWidth  = 150.0;
		float BaseHeight = 380.0;
		float BaseRadius = 100.0;

		float BaseHealth = 600;
		bool BaseInvincible = false;

		GLuint BaseTexture = ResourceManager.GetImage("Content/Images/Objects/Marker.bmp");

		lprotoObject *BaseObject = new lprotoObject(BasePosition,lmVector2D(BaseWidth/2.0,BaseHeight-90.0),BaseWidth,BaseHeight,lColor(1.0,1.0,1.0,1.0),BaseTexture,BaseRadius,lprotoObject::IMMOVABLE);
		Map->AddObject(BaseObject);

		Base = new lTD_Base(PLAYER_SPECIES,BaseHealth,BaseInvincible,BaseObject);
		Agents.push_back(Base);

		lTD_EnemySpawnPoint *SpawnPoint = new lTD_EnemySpawnPoint(lmVector2D(700.0,700.0),Base,AgentFactory);
		Agents.push_back(SpawnPoint);

		const lmVector2D OBJ_PICKER_POS(300.0,300.0);
		const float OBJ_PICKER_SIZE = 10.0;

		lprotoObject *ObjPickerObject = new lprotoObject(OBJ_PICKER_POS,lmVector2D(OBJ_PICKER_SIZE/2.0,OBJ_PICKER_SIZE/2.0),OBJ_PICKER_SIZE,OBJ_PICKER_SIZE,lColor(1.0,1.0,1.0,1.0),0,OBJ_PICKER_SIZE/2.0,lprotoObject::GHOST);
		Map->AddObject(ObjPickerObject);

		ObjectPicker = new lTD_ObjectPicker(ObjPickerObject);
		Agents.push_back(ObjectPicker);

		Player = new lTD_Player(PLAYER_SPECIES,Base,&Input,TowerFactory,ProjectileFactory,ObjectPicker);
		Agents.push_back(Player);

		//PlaceTower(lmVector2D(500.0,500.0));
    }

    virtual ~lTD_GameMode() override
    {
    	delete ProjectileFactory;
    }
};

#endif // L_TD_H
