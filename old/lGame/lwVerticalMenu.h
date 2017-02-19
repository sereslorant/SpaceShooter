#ifndef LW_VERTICAL_MENU
#define LW_VERTICAL_MENU

#include "lRect.h"

class lwMenu
{
protected:
    lMenu *Menu;
    unsigned int PrevMenuOption;

    lColor ButtonColor;
    lColor ActiveColor;

    std::vector<lRect *> MenuButtons;

public:
    lwMenu(lMenu *menu,const lColor &button_color,const lColor &active_color)
        :Menu(menu),ButtonColor(button_color),ActiveColor(active_color),MenuButtons(menu->GetSize())
    {
        PrevMenuOption = Menu->GetActive();
    }

    virtual ~lwMenu()
    {
        for(auto I : MenuButtons)
        {
            delete I;
        }
    }

    virtual void Resize(lmVector2D upper_left,float width,float height,float dist) = 0;

    void Update()
    {
        if(PrevMenuOption != Menu->GetActive())
        {
            MenuButtons[PrevMenuOption]->SetColor(ButtonColor);
            MenuButtons[Menu->GetActive()]->SetColor(ActiveColor);
            PrevMenuOption = Menu->GetActive();
        }
    }

    void Draw()
    {
        for(auto I : MenuButtons)
        {
            I->Draw();
        }
    }
};

class lwVerticalMenu : public lwMenu
{

public:
    lwVerticalMenu(lMenu *menu,lmVector2D upper_left,float width,float height,float dist,const lColor &button_color,const lColor &active_color)
        :lwMenu(menu,button_color,active_color)
    {
        lmVector2D TmpUpperLeft = upper_left;

        for(auto &I : MenuButtons)
        {
            I = new lRect(TmpUpperLeft,width,height,lRect::UP,ButtonColor,0);
            TmpUpperLeft.Y += height + dist;
        }

        MenuButtons[Menu->GetActive()]->SetColor(ActiveColor);
    }

    ~lwVerticalMenu()
    {

    }

    virtual void Resize(lmVector2D upper_left,float width,float height,float dist) override
    {
        lmVector2D TmpUpperLeft = upper_left;

        for(auto I : MenuButtons)
        {
            I->SetPosition(TmpUpperLeft);// = new lRect(TmpUpperLeft,width,height,lRect::UP,ButtonColor,0);
            I->SetWidth(width);
            I->SetHeight(height);
            TmpUpperLeft.Y += height + dist;
        }
    }

};

class lwHorizontalMenu : public lwMenu
{

public:
    lwHorizontalMenu(lMenu *menu,lmVector2D upper_left,float width,float height,float dist,const lColor &button_color,const lColor &active_color)
        :lwMenu(menu,button_color,active_color)
    {
        lmVector2D TmpUpperLeft = upper_left;

        for(auto &I : MenuButtons)
        {
            I = new lRect(TmpUpperLeft,width,height,lRect::UP,ButtonColor,0);
            TmpUpperLeft.X += width + dist;
        }

        MenuButtons[Menu->GetActive()]->SetColor(ActiveColor);
    }

    ~lwHorizontalMenu()
    {

    }

    virtual void Resize(lmVector2D upper_left,float width,float height,float dist) override
    {
        lmVector2D TmpUpperLeft = upper_left;

        for(auto I : MenuButtons)
        {
            I->SetPosition(TmpUpperLeft);// = new lRect(TmpUpperLeft,width,height,lRect::UP,ButtonColor,0);
            I->SetWidth(width);
            I->SetHeight(height);
            TmpUpperLeft.X += height + dist;
        }
    }

};

class lwWeirdMenu
{
private:
    lMenu *Menu;

    lRect *UpArrow;
    lRect *DownArrow;

    lRect *MenuOption;

    //lColor MenuOptionColor;
    //lColor ActiveMenuOptionColor;

    lColor ArrowColor;
    lColor ActiveArrowColor;

public:
    lwWeirdMenu(lMenu *menu,lmVector2D upper_left,float menu_option_width,float menu_option_height,float arrow_width,float arrow_height, float dist,
                const lColor &menu_option_color,/*const lColor &active_menu_option_color,*/const lColor &arrow_color,const lColor &active_arrow_color)
                    :Menu(menu),/*MenuOptionColor(menu_option_color),ActiveMenuOptionColor(active_menu_option_color),*/ArrowColor(arrow_color),ActiveArrowColor(active_arrow_color)
    {
        lmVector2D TmpUpperLeft = upper_left;

        UpArrow = new lRect(TmpUpperLeft,arrow_width,arrow_height,lRect::UP,ArrowColor,0);

        TmpUpperLeft.X += arrow_width + dist;

        MenuOption = new lRect(TmpUpperLeft,menu_option_width,menu_option_height,lRect::UP,menu_option_color,0);

        TmpUpperLeft.X += menu_option_width + dist;

        DownArrow = new lRect(TmpUpperLeft,arrow_width,arrow_height,lRect::UP,ArrowColor,0);
    }

    ~lwWeirdMenu()
    {
        delete UpArrow;
        delete DownArrow;

        delete MenuOption;
    }

    void Update()
    {
        if(Menu->GetActive() == 0)
        {
            UpArrow->SetColor(ArrowColor);
        }
        else
        {
            UpArrow->SetColor(ActiveArrowColor);
        }

        if(Menu->GetActive() == (Menu->GetSize()-1))
        {
            DownArrow->SetColor(ArrowColor);
        }
        else
        {
            DownArrow->SetColor(ActiveArrowColor);
        }
    }

    void Draw()
    {
        UpArrow->Draw();

        MenuOption->Draw();

        DownArrow->Draw();
    }
};

#endif
