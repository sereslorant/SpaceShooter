#ifndef L_SDL2_INPUT_H
#define L_SDL2_INPUT_H

#include "../lInput.h"

#include <SDL/SDL.h>

#include <iostream>

#include <string>
#include <vector>

class lSDL2Input final : public lInput
{
private:
	class lSDL2Controller : public liController
	{
	private:

		class lSDL2Hat : public liHat
		{
		public:
			Uint8 State = 0;

			virtual L_HAT_STATE GetHatState() override
			{
				switch(State)
				{
				case 0:
					return L_CNTRL_NORMAL;

				case 1:
					return L_CNTRL_UP;

				case 2:
					return L_CNTRL_RIGHT;

				case 4:
					return L_CNTRL_DOWN;

				case 8:
					return L_CNTRL_LEFT;

				case 3:
					return L_CNTRL_UP_RIGHT;

				case 6:
					return L_CNTRL_DOWN_RIGHT;

				case 12:
					return L_CNTRL_DOWN_LEFT;

				case 9:
					return L_CNTRL_UP_LEFT;

				default:
					return L_CNTRL_UNHANDLED;
				}
			}

			lSDL2Hat(){}
			virtual ~lSDL2Hat(){}
		};

		class lSDL2Axis : public liAxis
		{
		public:
			Uint16 DeadZone	= 0;
			Sint16 Value	= 0;

			static const Sint16 MAX_VALUE = 32767;

			virtual unsigned int GetDeadzone() override
			{
				return DeadZone;
			}

			virtual void SetDeadzone(unsigned int value) override
			{
				DeadZone = value;
			}

			virtual signed int GetValue() override
			{
				return Value;
			}

			virtual signed int GetMaxValue() override
			{
				return MAX_VALUE;
			}

			lSDL2Axis(){}
			virtual ~lSDL2Axis(){}
		};

		class lSDL2Button : public liButton
		{
		public:
			int State = 0;

			virtual bool IsPressed() override
			{
				return (State != 0);
			}

			lSDL2Button(){}
			virtual ~lSDL2Button(){}
		};

		SDL_Joystick *Joystick;

		std::string Name;

		std::vector<lSDL2Axis> Axes;
		std::vector<lSDL2Button> Buttons;
		std::vector<lSDL2Hat> Hats;

	public:

		virtual unsigned int GetNumHats() override
		{
			return Hats.size();
		}

		virtual unsigned int GetNumAxes() override
		{
			return Axes.size();
		}

		virtual unsigned int GetNumButtons() override
		{
			return Buttons.size();
		}

		virtual liHat *GetHat(unsigned int i) override
		{
			if(i < Hats.size())
			{
				return &Hats[i];
			}
			else
			{
				return nullptr;
			}
		}

		virtual liAxis *GetAxis(unsigned int i) override
		{
			if(i < Axes.size())
			{
				return &Axes[i];
			}
			else
			{
				return nullptr;
			}
		}

		virtual liButton *GetButton(unsigned int i) override
		{
			if(i < Buttons.size())
			{
				return &Buttons[i];
			}
			else
			{
				return nullptr;
			}
		}

		void RefreshState(SDL_Event *event)
		{
			if(event->type == SDL_JOYAXISMOTION)
			{
				int AxisId = event->jaxis.axis;
				Axes[AxisId].Value = event->jaxis.value;

				std::cout << "Axis: " << (int)event->jaxis.axis << " Value: " << event->jaxis.value << std::endl;
			}

			if(event->type == SDL_JOYBUTTONDOWN)
			{
				int ButtonId = event->jbutton.button;
				Buttons[ButtonId].State = 1;

				std::cout << "Button pressed: " << (int)event->jbutton.button << std::endl;
			}

			if(event->type == SDL_JOYBUTTONUP)
			{
				int ButtonId = event->jbutton.button;

				Buttons[ButtonId].State = 0;

				std::cout << "Button released: " << (int)event->jbutton.button << std::endl;
			}

			if(event->type == SDL_JOYHATMOTION)
			{
				int HatId = event->jhat.hat;

				Hats[HatId].State = event->jhat.value;

				std::cout << "Hat: " << (int)event->jhat.hat << " Value: " << (int)event->jhat.value << std::endl;
			}
		}

		lSDL2Controller(SDL_Joystick *joystick)
			:Joystick(joystick),
			Axes(SDL_JoystickNumAxes(joystick)),
			Buttons(SDL_JoystickNumButtons(joystick)),
			Hats(SDL_JoystickNumHats(joystick)),
			//Balls(SDL_JoystickNumBalls(joystick)),
			Name(SDL_JoystickName(SDL_JoystickIndex(joystick)))
		{
			std::cout << "Axes: " << Axes.size() << " Buttons: " << Buttons.size() << " Hats: " << Hats.size() << std::endl;
		}

		~lSDL2Controller()
		{
			SDL_JoystickClose(Joystick);
		}
	};

protected:

	std::vector<lSDL2Controller *> Controllers;

public:
    void CheckInput() final override;

    liController *GetController(unsigned int i)
    {
    	if(i < Controllers.size())
		{
			return Controllers[i];
		}
		else
		{
			return nullptr;
		}
    }

    lSDL2Input()
    {
    	int NumJoysticks = SDL_NumJoysticks();
    	Controllers.resize(NumJoysticks);

    	for(int i=0;i < NumJoysticks;i++)
		{
			SDL_Joystick *Joystick = SDL_JoystickOpen(i);

			if(Joystick != nullptr)
			{
				Controllers[i] = new lSDL2Controller(Joystick);
			}
		}
    }

    ~lSDL2Input()
    {
    	for(auto I : Controllers)
		{
			delete I;
		}
    }
};

#endif
