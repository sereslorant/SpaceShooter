#ifndef L_MENU_H
#define L_MENU_H

#include "lRect.h"

#include <vector>

class lMenu
{
private:
    unsigned int nMenuOptions;
    unsigned int ActiveMenuOption;
    //std::vector<lRect *> MenuOptions;

    //lColor ButtonColor;
    //lColor CurrentColor;

public:
    unsigned int GetSize()
    {
        return nMenuOptions;
    }

    unsigned int GetActive();
    void SetActive(unsigned int c)
    {
        //int Div = c/nMenuOptions;
        while (c > nMenuOptions)
        {
            c -= nMenuOptions;
        }
        ActiveMenuOption = c;
    }
    //void SetButtonTexture(unsigned int Id,GLuint Texture);

    void GoUp();
    void GoDown();

    //void Draw();

    lMenu(unsigned int n_menu_options/*,lmVector2D UpperLeft,float Width,float Height,const lColor &buttonColor,const lColor &currentColor*/);
    ~lMenu();
};

#endif // L_MENU_H
