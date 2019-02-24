#ifndef L_JRPG_H
#define L_JRPG_H

#include "../../lGame/lGame.h"

#include <list>
#include <string>

#include <iostream>
#include <fstream>

#include "../../lGame/lRect.h"

#include "../lprotoGameMode.h"

#include "lJRPG_Player.h"
#include "lJRPG_Enemy.h"
#include "lJRPG_Companion.h"

#include "../lGM_Agents/lGM_Fighter.h"

#include "../lGM_Agents/lGM_BulletFactory.h"

class lJRPG_PlayerCamera
{
private:
	lprotoObject *Object;

	lprotoCamera *Camera;
	float CamRadius;
	float CamSpeed;

public:

	const lmVector2D &GetObjPosition()
	{
		return Object->GetPosition();
	}

	const lmVector2D &GetCamPosition()
	{
		return Camera->GetPosition();
	}

	void SetCamRadius(float cam_radius)
	{
		CamRadius = cam_radius;
	}

	float GetCamRadius()
	{
		return CamRadius;
	}

	void SetCamSpeed(float cam_speed)
	{
		CamSpeed = cam_speed;
	}

	void UpdateCamera(float dt)
	{
		lprotoViewport &CameraViewport = Camera->GetViewport();

		lmVector2D CenterToObject = (Object->GetPosition() - Camera->GetPosition());
		lmVector2D CamVelocity;
		if(CenterToObject.LengthSquared() > (CamRadius * CamRadius))
		{
			CenterToObject.Normalize();
			CamVelocity = CenterToObject * CamSpeed;
		}
		else if(CenterToObject.LengthSquared() > ((CamRadius * CamRadius)/4.0))
		{
			CenterToObject.Normalize();
			CamVelocity = CenterToObject * CamSpeed * (0.5);
		}

		Camera->SetPosition(Camera->GetPosition() + CamVelocity * dt);
	}

	lJRPG_PlayerCamera(lprotoObject *object,lprotoCamera *camera,float cam_radius,float cam_speed)
		:Object(object),Camera(camera),CamRadius(cam_radius),CamSpeed(cam_speed)
	{

	}
};

class lJRPG_PlayerHUD
{
private:
	lJRPG_Hero *Player;

	const float HealthBarSize;

	lRect *PlayerPortrait;
	lRect *PlayerHealthBar;

	static constexpr unsigned int NUM_ABILITIES = 1;

	lRect *AbilityIcon[NUM_ABILITIES];

public:

	lRect *GetPortrait()
	{
		return PlayerPortrait;
	}

	lRect *GetHealthBar()
	{
		return PlayerHealthBar;
	}

	unsigned int GetNumAbilities()
	{
		return NUM_ABILITIES;
	}

	lRect *GetAbilityIcon(unsigned int ability_id)
	{
		if(ability_id < NUM_ABILITIES)
		{
			return AbilityIcon[ability_id];
		}
		else
		{
			return AbilityIcon[NUM_ABILITIES - 1];
		}
	}

	void Update()
	{
		//Player1Portrait->SetPosition(Player1Camera.GetPosition() + lmVector2D(30.0 - (CameraViewport.GetWidth()/2.0),30.0 - (CameraViewport.GetHeight()/2.0)));
		//PlayerHealthBar->SetPosition(Player1Camera.GetPosition() + lmVector2D(85.0 - (CameraViewport.GetWidth()/2.0),30.0 - (CameraViewport.GetHeight()/2.0)));

		float HealthPercentage = Player->GetHP() / Player->GetMaxHP();
		PlayerHealthBar->SetWidth(HealthPercentage * HealthBarSize);

		for(int i=0;i < NUM_ABILITIES;i++)
		{
			float Cooldown = std::max(std::min(Player->GetAbility(i)->GetCooldown(),1.0f),0.0f);

			lColor AbilityColor = AbilityIcon[i]->GetColor();

			AbilityColor.Alpha = (1.0 - Cooldown) * 0.25 + Cooldown * 1.0;

			AbilityIcon[i]->SetColor(AbilityColor);
		}
	}

	lJRPG_PlayerHUD(lJRPG_Hero *player,const lmVector2D &position,float profile_size,float health_bar_height,float health_bar_size
					/*lRect *player_portrait,lRect *player_health_bar*/
					)
		:Player(player),HealthBarSize(health_bar_size)//,PlayerPortrait(player_portrait),PlayerHealthBar(player_health_bar)
	{
		PlayerPortrait	= new lRect(position,profile_size,profile_size,lRect::UP,lColor(1.0,1.0,1.0,1.0),Player->GetPortrait());
        PlayerHealthBar	= new lRect(position + lmVector2D(profile_size + 5.0,0.0),health_bar_size,health_bar_height,lRect::UP,lColor(0.0,1.0,0.0,1.0),0);

		for(int i=0;i < NUM_ABILITIES;i++)
		{
			lmVector2D AbilityPos = position + lmVector2D((profile_size + 5.0) + i*(health_bar_height + 5.0),health_bar_height + 5.0);

			AbilityIcon[i] = new lRect(AbilityPos,health_bar_height,health_bar_height,lRect::UP,lColor(1.0,1.0,1.0,1.0),0);
		}
	}

	~lJRPG_PlayerHUD()
	{
		/*
		delete PlayerPortrait;
		delete PlayerHealthBar;

		for(int i=0;i < NUM_ABILITIES;i++)
		{
			delete AbilityIcon[i];
		}
		*/
	}
};

class lJRPG_HUD : public lprotoHUD
{
protected:

	std::list<lJRPG_PlayerHUD *> PlayerHUDs;

public:

	void AddPlayerHUD(lJRPG_PlayerHUD *player_hud)
	{
		PlayerHUDs.push_back(player_hud);

		HUD_Elements.push_back(player_hud->GetPortrait());
		HUD_Elements.push_back(player_hud->GetHealthBar());

		for(int i=0;i < player_hud->GetNumAbilities();i++)
		{
			HUD_Elements.push_back(player_hud->GetAbilityIcon(i));
		}
	}

	virtual void Logic() override
	{
		for(lJRPG_PlayerHUD *PlayerHUD : PlayerHUDs)
		{
			PlayerHUD->Update();
		}
	}

	lJRPG_HUD(float width,float height)
		:lprotoHUD(width,height)
	{

	}

	virtual ~lJRPG_HUD() override
	{
		for(lJRPG_PlayerHUD *PlayerHUD : PlayerHUDs)
		{
			delete PlayerHUD;
		}
	}
};

class lJRPG : public lprotoGameMode
{
protected:

	const float HEALTH_BAR_SIZE = 80.0;
	const float CAM_RADIUS = 250.0;

	lprotoCamera Player1Camera;
	lprotoCamera Player2Camera;

	lJRPG_PlayerCamera *PlayerCameraControl_1;
	lJRPG_PlayerCamera *PlayerCameraControl_2;

	float Width;
	float Height;

	lJRPG_Player *Player1;
	lJRPG_Player *Player2;

	lJRPG_Hero *PlayerChar2;

	lJRPG_Companion *Player2_AI;
	lJRPG_Companion *Player3_AI;

	lGM_BulletFactory *BulletFactory;

	lGM_Fighter *NewFighter(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,const std::string &textures,float radius,
					int species,bool alive,bool invincible,bool targetable,float max_hp,float hp_regen_rate,float speed,float reach,
					float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage)
	{
		GLuint Textures[3][10];

		LoadCharacterAnim(ResourceManager,textures,Textures);

        lprotoAnimObject *Object = new lprotoAnimObject(position,center,width,height,color,(GLuint *)&Textures[0],3*10,radius,lprotoObject::MOVABLE);
        Map->AddObject(Object);

        lprotoObject *Reach = new lprotoObject(position,center,width,height,lColor(0.0,0.0,0.0,0.0),0,reach,lprotoObject::GHOST);
        Map->AddObject(Reach);

        lGM_Fighter *Fighter = new lGM_Fighter(	Object,Reach,/*Textures,*/species,alive,invincible,targetable,max_hp,hp_regen_rate,speed,
													melee_recharge_speed,melee_damage,ranged_recharge_speed,ranged_damage,BulletFactory);

        return Fighter;
	}

	lJRPG_Hero *NewRaiden(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,const std::string &textures,float radius,
					int species,bool alive,bool invincible,bool targetable,float max_hp,float hp_regen_rate,float speed,float reach,
					float melee_recharge_speed,float melee_damage,float ranged_recharge_speed,float ranged_damage,
					float super_attack_reach)
	{
		GLuint Textures[3][10];

		LoadCharacterAnim(ResourceManager,textures,Textures);

        lprotoAnimObject *Object = new lprotoAnimObject(position,center,width,height,color,(GLuint *)&Textures[0],3*10,radius,lprotoObject::MOVABLE);
        Map->AddObject(Object);

        lprotoObject *Reach = new lprotoObject(position,center,width,height,lColor(0.0,0.0,0.0,0.0),0,reach,lprotoObject::GHOST);
        Map->AddObject(Reach);

        lprotoObject *SuperAttackReach = new lprotoObject(position,lmVector2D(super_attack_reach/2.0,super_attack_reach/2.0),super_attack_reach,super_attack_reach,lColor(0.0,0.0,0.0,0.0),0,super_attack_reach,lprotoObject::GHOST);
        Map->AddObject(SuperAttackReach);

        lJRPG_Raiden *Fighter = new lJRPG_Raiden(Object,Reach,/*Textures,*/species,alive,invincible,targetable,max_hp,hp_regen_rate,speed,
											 melee_recharge_speed,melee_damage,ranged_recharge_speed,ranged_damage,BulletFactory,
											 SuperAttackReach);

        return Fighter;
	}

	bool Coop;

	bool IsCoop()
	{
		return Coop;
	}

	void ToggleCoop()
	{
		if(!Coop)
		{
			Player2_AI->Disable();
			PlayerChar2->SetAI(Player2);

			lprotoViewport &Viewport = Player1Camera.GetViewport();

			Viewport.SetY(Height/2);
			Viewport.SetHeight(Height/2);

			PlayerCameraControl_1->SetCamRadius(CAM_RADIUS/2.0);

			Player2Camera.Enable();

			Coop = true;
		}
		else
		{
			Player2_AI->Enable();
			PlayerChar2->SetAI(Player2_AI);

			lprotoViewport &Viewport = Player1Camera.GetViewport();

			Viewport.SetY(0);
			Viewport.SetHeight(Height);

			PlayerCameraControl_1->SetCamRadius(CAM_RADIUS);

			Player2Camera.Disable();

			Coop = false;
		}
	}

	virtual void Logic() override
	{
		#ifdef KET_CONTROLLER

		lInput::liController *Cntrl0 = Input.GetController(0);
		if(Cntrl0 != nullptr)
		{
			if((Cntrl0->GetAxis(3)->GetValue() / (float)Cntrl0->GetAxis(3)->GetMaxValue()) > 0.01)
			{
				Player1->SetUp(false);
				Player1->SetDown(true);
			}
			else if((Cntrl0->GetAxis(3)->GetValue() / (float)Cntrl0->GetAxis(3)->GetMaxValue()) < -0.01)
			{
				Player1->SetUp(true);
				Player1->SetDown(false);
			}
			else
			{
				Player1->SetDown(false);
				Player1->SetUp(false);
			}

			if((Cntrl0->GetAxis(0)->GetValue() / (float)Cntrl0->GetAxis(0)->GetMaxValue()) > 0.01)
			{
				Player1->SetLeft(false);
				Player1->SetRight(true);
			}
			else if((Cntrl0->GetAxis(0)->GetValue() / (float)Cntrl0->GetAxis(0)->GetMaxValue()) < -0.01)
			{
				Player1->SetLeft(true);
				Player1->SetRight(false);
			}
			else
			{
				Player1->SetLeft(false);
				Player1->SetRight(false);
			}

			float DirX = Cntrl0->GetAxis(1)->GetValue() / (float)Cntrl0->GetAxis(1)->GetMaxValue();
			float DirY = Cntrl0->GetAxis(2)->GetValue() / (float)Cntrl0->GetAxis(2)->GetMaxValue();

			float Len = DirX*DirX + DirY*DirY;

			if((Len) > 0.05)
			{
				Player1->SetDirX(DirX,false);
				Player1->SetDirY(DirY,false);
			}

			if(Cntrl0->GetHat(0)->GetHatState() == lInput::liController::L_CNTRL_UP)
			{
				Player1->SetAttackMode(true);
			}
			else
			{
				Player1->SetAttackMode(false);
			}

			if(Cntrl0->GetButton(5)->IsPressed())
			{
				Player1->SetExecuteAbility(true);
			}
			else
			{
				Player1->SetExecuteAbility(false);
			}

			Player1->SetSprint(Cntrl0->GetButton(4)->IsPressed());
			Player1->SetAttack(Cntrl0->GetButton(7)->IsPressed());
		}

		#else

		Player1->SetUp(Input.GetUp());
		Player1->SetDown(Input.GetDown());
		Player1->SetLeft(Input.GetLeft());
		Player1->SetRight(Input.GetRight());
		Player1->SetSprint(Input.GetRShift());
		Player1->SetAttackMode(Input.GetRCtrl());
		Player1->SetAttack(Input.GetLmb());

		Player1->SetDirX(Input.GetX() + (Player1Camera.GetPosition().X - Player1Camera.GetViewport().GetWidth()/2.0),true);
		Player1->SetDirY(Input.GetY() + (Player1Camera.GetPosition().Y - Player1Camera.GetViewport().GetHeight()/2.0),true);

		Player1->SetExecuteAbility(Input.GetRmb());

		#endif

		#ifdef KET_CONTROLLER

		lInput::liController *Cntrl = Input.GetController(1);

		#else

		lInput::liController *Cntrl = Input.GetController(0);

		#endif

		if(Cntrl != nullptr && Cntrl->GetButton(8)->IsPressed())
		{
			ToggleCoop();
			SDL_Delay(200);
		}

		if(Coop)
		{
			if(Cntrl != nullptr)
			{
				if((Cntrl->GetAxis(1)->GetValue() / (float)Cntrl->GetAxis(1)->GetMaxValue()) > 0.01)
				{
					Player2->SetUp(false);
					Player2->SetDown(true);
				}
				else if((Cntrl->GetAxis(1)->GetValue() / (float)Cntrl->GetAxis(1)->GetMaxValue()) < -0.01)
				{
					Player2->SetUp(true);
					Player2->SetDown(false);
				}
				else
				{
					Player2->SetDown(false);
					Player2->SetUp(false);
				}

				if((Cntrl->GetAxis(2)->GetValue() / (float)Cntrl->GetAxis(2)->GetMaxValue()) > 0.01)
				{
					Player2->SetLeft(false);
					Player2->SetRight(true);
				}
				else if((Cntrl->GetAxis(2)->GetValue() / (float)Cntrl->GetAxis(2)->GetMaxValue()) < -0.01)
				{
					Player2->SetLeft(true);
					Player2->SetRight(false);
				}
				else
				{
					Player2->SetLeft(false);
					Player2->SetRight(false);
				}

				float DirX = Cntrl->GetAxis(3)->GetValue() / (float)Cntrl->GetAxis(3)->GetMaxValue();
				float DirY = Cntrl->GetAxis(4)->GetValue() / (float)Cntrl->GetAxis(4)->GetMaxValue();

				float Len = DirX*DirX + DirY*DirY;

				if((Len) > 0.05)
				{
					Player2->SetDirX(DirX,false);
					Player2->SetDirY(DirY,false);
				}

				if(Cntrl->GetHat(0)->GetHatState() == lInput::liController::L_CNTRL_UP)
				{
					Player2->SetAttackMode(true);
				}
				else
				{
					Player2->SetAttackMode(false);
				}

				if(Cntrl->GetButton(5)->IsPressed())
				{
					Player2->SetExecuteAbility(true);
				}
				else
				{
					Player2->SetExecuteAbility(false);
				}

				Player2->SetSprint(Cntrl->GetButton(4)->IsPressed());
				Player2->SetAttack(Cntrl->GetButton(7)->IsPressed());
			}

		}

		PlayerCameraControl_1->UpdateCamera(dt);
		PlayerCameraControl_2->UpdateCamera(dt);
	}

	virtual void TMP_Render() override
	{
		glLoadIdentity();

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

		for(auto I : Agents)
		{
			I->DBG_Draw();
		}
	}

public:

    lJRPG(RM &resource_manager,std::string &map_name,float width,float height)
        :lprotoGameMode(resource_manager,map_name),Width(width),Height(height),
         Player1Camera(lmVector2D::NULL_VECTOR,0,0,width,height),Player2Camera(lmVector2D::NULL_VECTOR,0,0,width,height/2)
    {
    	Cameras.push_back(&Player1Camera);
    	Cameras.push_back(&Player2Camera);

    	lJRPG_HUD *JRPG_HUD = new lJRPG_HUD(Width,Height);
    	HUD = JRPG_HUD;

    	Coop = false;

    	Player1Camera.Enable();
    	Player2Camera.Disable();

    	BulletFactory = new lGM_BulletFactory(*Map,Agents);

        GLuint MarkerTexture = ResourceManager.GetImage("Content/Images/Objects/Marker.bmp");

        GLuint BedTexture = ResourceManager.GetImage("Content/Images/Objects/Bed.bmp");
        GLuint Bed2Texture = ResourceManager.GetImage("Content/Images/Objects/Bed2.bmp");
        GLuint BedMikuTexture = ResourceManager.GetImage("Content/Images/Objects/BedMiku.bmp");

        GLuint AegisThinWallTexture	= ResourceManager.GetImage("Content/Images/Objects/Walls/AegisThinWall.bmp");
        GLuint AegisWallUpTexture	= ResourceManager.GetImage("Content/Images/Objects/Walls/AegisWallUp.bmp");
        GLuint AegisWallDownTexture	= ResourceManager.GetImage("Content/Images/Objects/Walls/AegisWallDown.bmp");
        GLuint AegisWallLeftTexture	= ResourceManager.GetImage("Content/Images/Objects/Walls/AegisWallLeft.bmp");
        GLuint AegisWallRightTexture= ResourceManager.GetImage("Content/Images/Objects/Walls/AegisWallRight.bmp");

        lmVector2D Player1Pos(50,350);
        lmVector2D Player2Pos(50,450);
        lmVector2D Player3Pos(150,350);

        float PlayerSize = 36.0;
        float PlayerRadius = 18.0;
        float PlayerReach = 36.0;

        float MikuSize = 50.0;
        float MikuRadius = 25.0;
        float MikuReach = 50.0;

        float PlayerHP = 350.0;
        float PlayerRegen = 100.0;
        float PlayerSpeed = 200.0;
        float PlayerRangedDamage = 20.0;
        float PlayerRangedRecharge = 14.0;
        float PlayerMeleeDamage = 80.0;
        float PlayerMeleeRecharge = 3.0;

        float PlayerDist = 100.0;

        float SuperAttackRange = 100.0;

        int PlayerSpecies = 1;

       /* lprotoObject *PlayerObj1 = new lprotoObject(lmVector2D(50,350),lmVector2D(17,17),34,34,lColor(1.0,1.0,1.0,1.0),0,15.0,lprotoObject::MOVABLE);
        Map->AddObject(PlayerObj1);*/

        lJRPG_Hero *PlayerChar1 = NewRaiden(Player1Pos,lmVector2D(PlayerSize/2.0,PlayerSize/2.0),PlayerSize,PlayerSize,lColor(1.0,1.0,1.0,1.0),"Raiden",PlayerRadius,PlayerSpecies,
											true,false,true,PlayerHP,PlayerRegen,PlayerSpeed,PlayerReach,PlayerMeleeRecharge,PlayerMeleeDamage,PlayerRangedRecharge,PlayerRangedDamage,
											SuperAttackRange);
        Agents.push_back(PlayerChar1);

        PlayerCameraControl_1 = new lJRPG_PlayerCamera(PlayerChar1->GetObject(),&Player1Camera,CAM_RADIUS,PlayerChar1->GetSpeed() * 2.0);

        //lRect *Player1Portrait = new lRect(lmVector2D(30,30),50,50,lRect::UP,lColor(1.0,1.0,1.0,1.0),PlayerChar1->GetPortrait());
        //lRect *Player1HealthBar = new lRect(lmVector2D(85,30),HEALTH_BAR_SIZE,20,lRect::UP,lColor(0.0,1.0,0.0,1.0),0);

        JRPG_HUD->AddPlayerHUD(new lJRPG_PlayerHUD(PlayerChar1,lmVector2D(30,30),50.0,20.0,HEALTH_BAR_SIZE/*,Player1Portrait,Player1HealthBar*/));

        Player1 = new lJRPG_Player(PlayerChar1);
        //Agents.push_back(Player1);
        PlayerChar1->SetAI(Player1);

        //2. játékos hozzáadása
        /*lprotoObject *PlayerObj2 = new lprotoObject(lmVector2D(50,450),lmVector2D(18,18),36,36,lColor(1.0,1.0,1.0,1.0),0,15.0,lprotoObject::MOVABLE);
        Map->AddObject(PlayerObj2);*/

        PlayerChar2 = NewRaiden(Player2Pos,lmVector2D(PlayerSize/2.0,PlayerSize/2.0),PlayerSize,PlayerSize,lColor(1.0,1.0,1.0,1.0),"Isaac",PlayerRadius,PlayerSpecies,
								true,false,true,PlayerHP,PlayerRegen,PlayerSpeed,PlayerReach,PlayerMeleeRecharge,PlayerMeleeDamage,PlayerRangedRecharge,PlayerRangedDamage,
								SuperAttackRange);
        Agents.push_back(PlayerChar2);

        //lRect *Player2Portrait = new lRect(lmVector2D(30,100),50,50,lRect::UP,lColor(1.0,1.0,1.0,1.0),PlayerChar2->GetPortrait());
        //lRect *Player2HealthBar = new lRect(lmVector2D(85,100),HEALTH_BAR_SIZE,20,lRect::UP,lColor(0.0,1.0,0.0,1.0),0);

        JRPG_HUD->AddPlayerHUD(new lJRPG_PlayerHUD(PlayerChar2,lmVector2D(30,100),50.0,20.0,HEALTH_BAR_SIZE/*,Player2Portrait,Player2HealthBar*/));

        Player2 = new lJRPG_Player(PlayerChar2);
        //Agents.push_back(Player2);
        //PlayerChar2->SetAI(Player2);

        Player2_AI = new lJRPG_Companion(PlayerChar2,PlayerChar1,PlayerDist,true,*Map);
		//Agents.push_back(Player2_AI);
        PlayerChar2->SetAI(Player2_AI);

        PlayerCameraControl_2 = new lJRPG_PlayerCamera(PlayerChar2->GetObject(),&Player2Camera,CAM_RADIUS/2.0,PlayerChar2->GetSpeed() * 2.0);

        //3. játékos hozzáadása
        /*lprotoObject *PlayerObj3 = new lprotoObject(lmVector2D(150,350),lmVector2D(25,25),50,50,lColor(1.0,1.0,1.0,1.0),0,15.0,lprotoObject::MOVABLE);
        Map->AddObject(PlayerObj3);*/

        /*lGM_Fighter *PlayerChar3 = NewFighter(Player3Pos,lmVector2D(MikuSize/2.0,MikuSize/2.0),MikuSize,MikuSize,lColor(1.0,1.0,1.0,1.0),"Miku",MikuRadius,PlayerSpecies,
												true,false,true,PlayerHP,PlayerRegen,PlayerSpeed,MikuReach,PlayerMeleeRecharge,PlayerMeleeDamage,PlayerRangedRecharge,PlayerRangedDamage);*/
        lJRPG_Hero *PlayerChar3 = NewRaiden(Player3Pos,lmVector2D(MikuSize/2.0,MikuSize/2.0),MikuSize,MikuSize,lColor(1.0,1.0,1.0,1.0),"Miku",MikuRadius,PlayerSpecies,
											true,false,true,PlayerHP,PlayerRegen,PlayerSpeed,MikuReach,PlayerMeleeRecharge,PlayerMeleeDamage,PlayerRangedRecharge,PlayerRangedDamage,
											SuperAttackRange);

        Agents.push_back(PlayerChar3);

        //PlayerChar = new lJRPG_Fighter(lmVector2D(150,350),lmVector2D(25,25),50,50,lColor(1.0,1.0,1.0,1.0),PlayerSpeed,MikuTextures,25.0,PlayerSpecies,true,true,PlayerHP,PlayerRegen,50.0,PlayerMeleeRecharge,PlayerMeleeDamage,PlayerRangedRecharge,PlayerDamage);
        //Map->AddObject(PlayerChar);

        //lRect *Player3Portrait = new lRect(lmVector2D(30,160),50,50,lRect::UP,lColor(1.0,1.0,1.0,1.0),PlayerChar3->GetPortrait());
        //lRect *Player3HealthBar = new lRect(lmVector2D(85,160),HEALTH_BAR_SIZE,20,lRect::UP,lColor(0.0,1.0,0.0,1.0),0);

        JRPG_HUD->AddPlayerHUD(new lJRPG_PlayerHUD(PlayerChar3,lmVector2D(30,160),50.0,20.0,HEALTH_BAR_SIZE/*,Player3Portrait,Player3HealthBar*/));

        //Player3 = new lJRPG_Player(PlayerChar);
        //Actors.push_back(Player3);

        Player3_AI = new lJRPG_Companion(PlayerChar3,PlayerChar1,PlayerDist,true,*Map);
		//Agents.push_back(Player3_AI);
		PlayerChar3->SetAI(Player3_AI);

		float EnemySize = 30.0;
        float EnemyRadius = 15.0;
        float EnemyReach = 30.0;

        float EnemyHP = 500.0;
        float EnemyRegen = 20.0;
        float EnemySpeed = 150.0;
        float EnemyRangedDamage = 20.0;
        float EnemyRangedRecharge = 1.5;
        float EnemyMeleeDamage = 20.0;
        float EnemyMeleeRecharge = 1.0;

        int EnemySpecies = 2;

        #ifdef SIMONYI
			std::string EnemyTexture = "Tita";
        #else
			std::string EnemyTexture = "Enemy";
        #endif // SIMONYI

        for(int j=0;j < 2;j++)
		{
			for(int i=0;i < 6;i++)
			{
				lmVector2D EnemyPos(600 + j*30,50 + i*30);

				lGM_Fighter *EnemyChar = NewFighter(EnemyPos,lmVector2D(EnemySize/2.0,EnemySize/2.0),EnemySize,EnemySize,lColor(0.0,1.0,1.0,1.0),EnemyTexture,EnemyRadius,EnemySpecies,
													  true,false,true,EnemyHP,EnemyRegen,EnemySpeed,EnemyReach,EnemyMeleeRecharge,EnemyMeleeDamage,EnemyRangedRecharge,EnemyRangedDamage);

				Agents.push_back(EnemyChar);
				EnemyChar->SetAI(new lJRPG_Enemy(EnemyChar,*Map));
			}
		}

        for(int j=0;j < 2;j++)
		{
			for(int i=0;i < 6;i++)
			{
				lmVector2D EnemyPos(2000 + j*30,-1000 + i*30);

				lGM_Fighter *EnemyChar = NewFighter(EnemyPos,lmVector2D(EnemySize/2.0,EnemySize/2.0),EnemySize,EnemySize,lColor(1.0,1.0,1.0,1.0),EnemyTexture,EnemyRadius,EnemySpecies,
													  true,false,true,EnemyHP,EnemyRegen,EnemySpeed,EnemyReach,EnemyMeleeRecharge,EnemyMeleeDamage,EnemyRangedRecharge,EnemyRangedDamage);

				Agents.push_back(EnemyChar);
				EnemyChar->SetAI(new lJRPG_Enemy(EnemyChar,*Map));
			}
		}

		for(int j=0;j < 2;j++)
		{
			for(int i=0;i < 6;i++)
			{
				lmVector2D EnemyPos(-3000 + j*30,-1000 + i*30);

				lGM_Fighter *EnemyChar = NewFighter(EnemyPos,lmVector2D(EnemySize/2.0,EnemySize/2.0),EnemySize,EnemySize,lColor(0.0,1.0,1.0,1.0),EnemyTexture,EnemyRadius,EnemySpecies,
													  true,false,true,EnemyHP,EnemyRegen,EnemySpeed,EnemyReach,EnemyMeleeRecharge,EnemyMeleeDamage,EnemyRangedRecharge,EnemyRangedDamage);

				Agents.push_back(EnemyChar);
				EnemyChar->SetAI(new lJRPG_Enemy(EnemyChar,*Map));
			}
		}

		float BossSize = 80.0;
        float BossRadius = 40.0;
        float BossReach = 80.0;

        float BossHP = 5000.0;
        float BossRegen = 20.0;
        float BossSpeed = 60.0;
        float BossRangedDamage = 100.0;
        float BossRangedRecharge = 1.5;
        float BossMeleeDamage = 100.0;
        float BossMeleeRecharge = 1.0;

		lmVector2D BossPos(1500,100);

		lGM_Fighter *EnemyChar = NewFighter(BossPos,lmVector2D(BossSize/2.0,BossSize/2.0),BossSize,BossSize,lColor(1.0,0.0,1.0,1.0),EnemyTexture,BossRadius,EnemySpecies,
											  true,false,true,BossHP,BossRegen,BossSpeed,BossReach,BossMeleeRecharge,BossMeleeDamage,BossRangedRecharge,BossRangedDamage);

		Agents.push_back(EnemyChar);
		EnemyChar->SetAI(new lJRPG_Enemy(EnemyChar,*Map));

		for(int i=0;i < 16;i++)
		{
			if(i%4 == 0 && i != 0)
			{continue;}

			lprotoObject *Wall  = new lprotoObject(lmVector2D(45.0 + i*30.0,15.0),lmVector2D(15.0,45.0),30,60,lColor(1.0,1.0,1.0,1.0),AegisWallDownTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 6;i++)
		{
			lprotoObject *Wall  = new lprotoObject(lmVector2D(45.0 + 4*30.0,-15.0 + i* 30.0),lmVector2D(15.0,15.0),30,30,lColor(1.0,1.0,1.0,1.0),AegisThinWallTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 6;i++)
		{
			lprotoObject *Wall  = new lprotoObject(lmVector2D(45.0 + 8*30.0,-15.0 + i* 30.0),lmVector2D(15.0,15.0),30,30,lColor(1.0,1.0,1.0,1.0),AegisThinWallTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 9;i++)
		{
			lprotoObject *Wall  = new lprotoObject(lmVector2D(15.0,-15.0 + i* 30.0),lmVector2D(15.0,15.0),30,30,lColor(1.0,1.0,1.0,1.0),AegisWallRightTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 9;i++)
		{
			lprotoObject *Wall  = new lprotoObject(lmVector2D(45.0 + 16*30.0,-15.0 + i* 30.0),lmVector2D(15.0,15.0),30,30,lColor(1.0,1.0,1.0,1.0),AegisWallLeftTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 6;i++)
		{
			lprotoObject *Wall  = new lprotoObject(lmVector2D(45.0 + 12*30.0,-15.0 + i* 30.0),lmVector2D(15.0,15.0),30,30,lColor(1.0,1.0,1.0,1.0),AegisThinWallTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 12;i++)
		{
			if(i == 2 || i == 6 || i == 10)
			{continue;}

			lprotoObject *Wall  = new lprotoObject(lmVector2D(45.0 + 4*30 + i*30.0,15.0 + 5*30),lmVector2D(15.0,45.0),30,60,lColor(1.0,1.0,1.0,1.0),AegisWallDownTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

		for(int i=0;i < 18;i++)
		{
			lprotoObject *Wall  = new lprotoObject(lmVector2D(15.0 + i*30.0,15.0 + 8*30),lmVector2D(15.0,45.0),30,60,lColor(1.0,1.0,1.0,1.0),AegisWallUpTexture,15.0,lprotoObject::IMMOVABLE);
			Map->AddObject(Wall);
		}

        lprotoObject *Marker = new lprotoObject(lmVector2D(400-15,800),lmVector2D(155.0/2.0,400-90.0),155,380,lColor(1.0,1.0,1.0,1.0),MarkerTexture,155.0/2.0,lprotoObject::MOVABLE);
        Map->AddObject(Marker);

        lprotoObject *Bed = new lprotoObject(lmVector2D(150+50,100-60),lmVector2D(16.0,25.0),32,50,lColor(1.0,1.0,1.0,1.0),BedTexture,20.0,lprotoObject::IMMOVABLE);
        Map->AddObject(Bed);

        lprotoObject *Bed2 = new lprotoObject(lmVector2D(300+60,100-60),lmVector2D(25.0,16.0),50,32,lColor(1.0,1.0,1.0,1.0),Bed2Texture,20.0,lprotoObject::IMMOVABLE);
        Map->AddObject(Bed2);

        lprotoObject *BedMiku = new lprotoObject(lmVector2D(450,100-60),lmVector2D(25.0,16.0),50,32,lColor(1.0,1.0,1.0,1.0),BedMikuTexture,20.0,lprotoObject::IMMOVABLE);
        Map->AddObject(BedMiku);
    }

    ~lJRPG()
    {
		delete PlayerCameraControl_1;
		delete PlayerCameraControl_2;

		//delete Player1;

		if(Coop)
		{
			delete Player2_AI;
		}
		else
		{
			delete Player2;
		}

		//delete Player3_AI;

    }
};

#endif // L_JRPG_H
