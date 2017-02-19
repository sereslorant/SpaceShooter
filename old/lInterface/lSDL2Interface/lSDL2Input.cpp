
#include "lSDL2Input.h"

/*
int main()
{
  SDL_Init(SDL_INIT_JOYSTICK);
  atexit(SDL_Quit);

  int num_joysticks = SDL_NumJoysticks();
  int i;
  for(i = 0; i < num_joysticks; ++i)
  {
    SDL_Joystick* js = SDL_JoystickOpen(i);
    if (js)
    {
      SDL_JoystickGUID guid = SDL_JoystickGetGUID(js);
      char guid_str[1024];
      SDL_JoystickGetGUIDString(guid, guid_str, sizeof(guid_str));
      const char* name = SDL_JoystickName(js);

      int num_axes = SDL_JoystickNumAxes(js);
      int num_buttons = SDL_JoystickNumButtons(js);
      int num_hats = SDL_JoystickNumHats(js);
      int num_balls = SDL_JoystickNumBalls(js);

      printf("%s \"%s\" axes:%d buttons:%d hats:%d balls:%d\n",
             guid_str, name,
             num_axes, num_buttons, num_hats, num_balls);

      SDL_JoystickClose(js);
    }
  }

  return 0;
}
*/

void lSDL2Input::CheckInput()
{
    SDL_Event event;
    //lMouseState &MouseState = Input.GetMouseState();

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            SetQuit(true);
        }

        if(event.type == SDL_MOUSEMOTION)
        {
            int X = event.motion.x;
            int Y = event.motion.y;

            SetMX(X);
            SetMY(Y);
        }

        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {SetLmb(true);}
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {SetMmb(true);}
            if(event.button.button == SDL_BUTTON_RIGHT)
            {SetRmb(true);}
        }
        if(event.type == SDL_MOUSEBUTTONUP)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {SetLmb(false);}
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {SetMmb(false);}
            if(event.button.button == SDL_BUTTON_RIGHT)
            {SetRmb(false);}
        }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {SetEscape(true);}


            if(event.key.keysym.sym == SDLK_RETURN)
            {SetEnter(true);}

            if(event.key.keysym.sym == SDLK_LSHIFT)
            {SetLShift(true);}

            if(event.key.keysym.sym == SDLK_RSHIFT)
            {SetRShift(true);}

            if(event.key.keysym.sym == SDLK_LCTRL)
            {SetLCtrl(true);}

            if(event.key.keysym.sym == SDLK_RCTRL)
            {SetRCtrl(true);}

            if(event.key.keysym.sym == SDLK_SPACE)
            {SetSpace(true);}


            if(event.key.keysym.sym == SDLK_UP)
            {SetUp(true);}

            if(event.key.keysym.sym == SDLK_DOWN)
            {SetDown(true);}

            if(event.key.keysym.sym == SDLK_LEFT)
            {SetLeft(true);}

            if(event.key.keysym.sym == SDLK_RIGHT)
            {SetRight(true);}


            if(event.key.keysym.sym == SDLK_w)
            {SetW(true);}

            if(event.key.keysym.sym == SDLK_a)
            {SetA(true);}

            if(event.key.keysym.sym == SDLK_s)
            {SetS(true);}

            if(event.key.keysym.sym == SDLK_d)
            {SetD(true);}
        }

        if(event.type == SDL_KEYUP)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {SetEscape(false);}


            if(event.key.keysym.sym == SDLK_RETURN)
            {SetEnter(false);}

            if(event.key.keysym.sym == SDLK_LSHIFT)
            {SetLShift(false);}

            if(event.key.keysym.sym == SDLK_RSHIFT)
            {SetRShift(false);}

            if(event.key.keysym.sym == SDLK_LCTRL)
            {SetLCtrl(false);}

            if(event.key.keysym.sym == SDLK_RCTRL)
            {SetRCtrl(false);}

            if(event.key.keysym.sym == SDLK_SPACE)
            {SetSpace(false);}


            if(event.key.keysym.sym == SDLK_UP)
            {SetUp(false);}

            if(event.key.keysym.sym == SDLK_DOWN)
            {SetDown(false);}

            if(event.key.keysym.sym == SDLK_LEFT)
            {SetLeft(false);}

            if(event.key.keysym.sym == SDLK_RIGHT)
            {SetRight(false);}


            if(event.key.keysym.sym == SDLK_w)
            {SetW(false);}

            if(event.key.keysym.sym == SDLK_a)
            {SetA(false);}

            if(event.key.keysym.sym == SDLK_s)
            {SetS(false);}

            if(event.key.keysym.sym == SDLK_d)
            {SetD(false);}
        }

        if(event.type == SDL_JOYAXISMOTION)
		{
			int ControllerIndex = event.jaxis.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}

		if(event.type == SDL_JOYBUTTONDOWN)
		{
			int ControllerIndex = event.jbutton.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}

		if(event.type == SDL_JOYBUTTONUP)
		{
			int ControllerIndex = event.jbutton.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}

		if(event.type == SDL_JOYHATMOTION)
		{
			int ControllerIndex = event.jhat.which;

			Controllers[ControllerIndex]->RefreshState(&event);
		}
    }
}
