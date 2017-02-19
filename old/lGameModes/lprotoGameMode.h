#ifndef L_PROTO_GAME_MODE
#define L_PROTO_GAME_MODE

#include "../lInterface/lSDL2Interface/lSDL2Input.h"
#include "../lInterface/lSDL2Interface/lSDL2FrameLimiter.h"

#include "../lGame/lGame.h"

#include "lGame/liAgent.h"
#include "lprotoMap.h"

class lprotoViewport
{
private:
	int X;
	int Y;
	int Width;
	int Height;

public:

	int GetX()
	{
		return X;
	}

	int GetY()
	{
		return Y;
	}

	void SetX(int x)
	{
		X = x;
	}

	void SetY(int y)
	{
		Y = y;
	}

	int GetWidth()
	{
		return Width;
	}

	int GetHeight()
	{
		return Height;
	}

	void SetWidth(int width)
	{
		Width = width;
	}

	void SetHeight(int height)
	{
		Height = height;
	}

	lprotoViewport(int x,int y,int width,int height)
		:X(x),Y(y),Width(width),Height(height)
	{

	}
};

class lprotoCamera
{
private:
	lmVector2D		Position;
	lprotoViewport	Viewport;

	bool Enabled;

public:
	const lmVector2D &GetPosition()
	{
		return Position;
	}

	void SetPosition(const lmVector2D &position)
	{
		Position = position;
	}

	void SetX(int x)
	{
		Position.X = x;
	}

	void SetY(int y)
	{
		Position.Y = y;
	}

	lprotoViewport &GetViewport()
	{
		return Viewport;
	}

	const lprotoViewport &GetViewport() const
	{
		return Viewport;
	}

	bool IsEnabled()
	{
		return Enabled;
	}

	void Enable()
	{
		Enabled = true;
	}

	void Disable()
	{
		Enabled = false;
	}

	lprotoCamera(const lmVector2D &position,int vp_x,int vp_y,int vp_width,int vp_height)
		:Position(position),Viewport(vp_x,vp_y,vp_width,vp_height),Enabled(false)
	{

	}
};

class lprotoHUD
{
protected:
	float Width;
	float Height;

	std::list<lRect *> HUD_Elements;

public:

	virtual void Logic() = 0;

	void Draw()
	{
		glViewport(0,0,800,600);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0.0,Width,Height,0.0,-1,1);

		glMatrixMode(GL_MODELVIEW);

		for(lRect *Rect : HUD_Elements)
		{
			Rect->Draw();
		}
	}

	lprotoHUD(float width,float height)
		:Width(width),Height(height)
	{

	}

	virtual ~lprotoHUD()
	{
		for(lRect * Rect : HUD_Elements)
		{
			delete Rect;
		}
	}
};

class lprotoGameMode : public lGame
{
protected:
	RM &ResourceManager;

	std::string &MapName;

	lSDL2Input Input;
	lSDL2FrameLimiter FrameLimiter;

	//lmVector2D ViewPos;
	/*lmVector2D CamVelocity;

	float Width;
	float Height;*/

	//lprotoCamera Player1Viewport;
	//lprotoCamera Player2Viewport;

	std::list<lprotoCamera *> Cameras;

	lprotoMap *Map;
    std::list<liAgent *> Agents;

    lprotoHUD *HUD;

    float dt;

    virtual void Logic() = 0;

    virtual void TMP_Render() = 0;

public:
	lprotoGameMode(RM &resource_manager,std::string &map_name/*,float width,float height*/)
        :ResourceManager(resource_manager),MapName(map_name),FrameLimiter(60),HUD(nullptr)//,ViewPos(lmVector2D::NULL_VECTOR),Width(width),Height(height)
		 //Player1Viewport(lmVector2D::NULL_VECTOR,0,0,Width,Height/2),Player2Viewport(lmVector2D::NULL_VECTOR,0,Height/2,Width,Height/2)
    {
        Map = new lprotoMap(ResourceManager,map_name);

        dt = 1.0/60.0;
    }

    virtual ~lprotoGameMode()
    {
        delete Map;

        if(HUD != nullptr)
		{
			delete HUD;
		}

        for(auto I : Agents)
		{
			delete I;
		}
    }

    virtual int MainLoop() override
    {
        //Ha ki akarunk lépni, akkor (-1)-be kell állítani.
        int RetValue = 0;
        bool IsRunning = true;
        while(IsRunning)
        {
			FrameLimiter.StartFrameLimiter();
            //Input
            Input.CheckInput();

            //Logic
            if(Input.GetEscape())
            {
                IsRunning = false;

                MapName = "MainMenu";
                RetValue = 0;
            }
            if(Input.GetQuit())
            {
                IsRunning = false;

                MapName = "MainMenu";
                RetValue = -1;
            }

            Logic();

            if(HUD != nullptr)
			{
				HUD->Logic();
			}

            for(auto I : Agents)
            {
                I->ExecuteBehavior(dt);
            }

            Map->Step(dt);

            //Rendering

			glClear(GL_COLOR_BUFFER_BIT);

			for(lprotoCamera *Camera : Cameras)
			{
				if(Camera->IsEnabled())
				{
					lprotoViewport &CameraViewport = Camera->GetViewport();

					glViewport(CameraViewport.GetX()    ,CameraViewport.GetY(),
							   CameraViewport.GetWidth(),CameraViewport.GetHeight());

					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();

					glOrtho(Camera->GetPosition().X - (CameraViewport.GetWidth()/2.0),
							Camera->GetPosition().X + (CameraViewport.GetWidth()/2.0),
							Camera->GetPosition().Y + (CameraViewport.GetHeight()/2.0),
							Camera->GetPosition().Y - (CameraViewport.GetHeight()/2.0),
							-1,1);

					glMatrixMode(GL_MODELVIEW);

					Map->Draw(Camera->GetPosition());
					TMP_Render();
				}
			}

			if(HUD != nullptr)
			{
				HUD->Draw();
			}

            SDL_GL_SwapBuffers();

			FrameLimiter.CheckFrameLimiter();
        }

        return RetValue;
    }
};

#endif // L_PROTO_GAME_MODE
