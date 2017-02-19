
#include "lGM_Fighter.h"

#include "AI/lGM_AI.h"

void lGM_Fighter::ExecuteBehavior(float dt)
{
	if(Alive)
	{
		if(AI != nullptr)
		{
			AI->Control(dt);
		}
	}

	c_Dir.Normalize();

	SetAnimation();
	ApplyControls(dt);

	if(Alive)
	{
		/*
		if(c_Dir.Y > (1.0/2.0))
		{
			anim_Up = true;
		}
		else
		{
			anim_Up = false;
		}

		if(c_Dir.Y < (-1.0/2.0))
		{
			anim_Down = true;
		}
		else
		{
			anim_Down = false;
		}

		if(c_Dir.X > (1.0/2.0))
		{
			anim_Right = true;
		}
		else
		{
			anim_Right = false;
		}

		if(c_Dir.X < (-1.0/2.0))
		{
			anim_Left = true;
		}
		else
		{
			anim_Left = false;
		}
		*/
/*
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

		Object->AddVelocity(Velocity);*/
/*
		if((c_Up == false && c_Down == false && c_Left == false && c_Right == false) ||
		   (c_Up == true  && c_Down == true  && c_Left == false && c_Right == false) ||
		   (c_Up == false && c_Down == false && c_Left == true  && c_Right == true)
		  )
		{
			anim_Moving = false;
		}
*//*
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
*/
		if(HP < MaxHP)
		{
			HP += HPRegenRate * dt;
		}

		if(AtkModeReady < 1.0)
		{
			AtkModeReady += AtkModeRechargeSpeed * dt;
		}

		if(MeleeReady < 1.0)
		{
			MeleeReady += MeleeRechargeSpeed * dt;
		}

		if(RangedReady < 1.0)
		{
			RangedReady += RangedRechargeSpeed * dt;
		}
	}
	else
	{
		//anim_Dead = true;
		Alive = false;

		HP = 0.0;
	}

	MeleeReach->Targets.clear();

	MeleeReach->SetPosition(Object->GetPosition());

	Animate(dt);
}














void LoadCharacterAnim(RM &resource_manager,const std::string &name,GLuint Textures[3][10])
{
        Textures[0][0] = resource_manager.GetImage("Content/Images/"+name+"/front_step1.bmp");
        Textures[1][0] = resource_manager.GetImage("Content/Images/"+name+"/front_standing.bmp");
        Textures[2][0] = resource_manager.GetImage("Content/Images/"+name+"/front_step2.bmp");

        Textures[0][1] = resource_manager.GetImage("Content/Images/"+name+"/back_step1.bmp");
        Textures[1][1] = resource_manager.GetImage("Content/Images/"+name+"/back_standing.bmp");
        Textures[2][1] = resource_manager.GetImage("Content/Images/"+name+"/back_step2.bmp");

        Textures[0][2] = resource_manager.GetImage("Content/Images/"+name+"/left_step1.bmp");
        Textures[1][2] = resource_manager.GetImage("Content/Images/"+name+"/left_standing.bmp");
        Textures[2][2] = resource_manager.GetImage("Content/Images/"+name+"/left_step2.bmp");

        Textures[0][3] = resource_manager.GetImage("Content/Images/"+name+"/right_step1.bmp");
        Textures[1][3] = resource_manager.GetImage("Content/Images/"+name+"/right_standing.bmp");
        Textures[2][3] = resource_manager.GetImage("Content/Images/"+name+"/right_step2.bmp");

        Textures[0][4] = resource_manager.GetImage("Content/Images/"+name+"/left_dead.bmp");
        Textures[1][4] = resource_manager.GetImage("Content/Images/"+name+"/back_dead.bmp");
        Textures[2][4] = resource_manager.GetImage("Content/Images/"+name+"/right_dead.bmp");

        Textures[0][5] = resource_manager.GetImage("Content/Images/"+name+"/Portrait.bmp");
        Textures[1][5] = resource_manager.GetImage("Content/Images/"+name+"/Portrait.bmp");
        Textures[2][5] = resource_manager.GetImage("Content/Images/"+name+"/Portrait.bmp");

        Textures[0][6] = resource_manager.GetImage("Content/Images/"+name+"/front_atk0.bmp");
        Textures[1][6] = resource_manager.GetImage("Content/Images/"+name+"/front_atk1.bmp");
        Textures[2][6] = resource_manager.GetImage("Content/Images/"+name+"/front_atk.bmp");

        Textures[0][7] = resource_manager.GetImage("Content/Images/"+name+"/back_atk0.bmp");
        Textures[1][7] = resource_manager.GetImage("Content/Images/"+name+"/back_atk1.bmp");
        Textures[2][7] = resource_manager.GetImage("Content/Images/"+name+"/back_atk.bmp");

        Textures[0][8] = resource_manager.GetImage("Content/Images/"+name+"/left_atk0.bmp");
        Textures[1][8] = resource_manager.GetImage("Content/Images/"+name+"/left_atk1.bmp");
        Textures[2][8] = resource_manager.GetImage("Content/Images/"+name+"/left_atk.bmp");

        Textures[0][9] = resource_manager.GetImage("Content/Images/"+name+"/right_atk0.bmp");
        Textures[1][9] = resource_manager.GetImage("Content/Images/"+name+"/right_atk1.bmp");
        Textures[2][9] = resource_manager.GetImage("Content/Images/"+name+"/right_atk.bmp");
}







