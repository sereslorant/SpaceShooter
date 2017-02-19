
#include "lMenu.h"

unsigned int lMenu::GetActive()
{
    return ActiveMenuOption;
}

void lMenu::GoUp()
{
    if(ActiveMenuOption == 0)
    {
        ActiveMenuOption = nMenuOptions-1;
    }
    else
    {
        ActiveMenuOption--;
    }
}

void lMenu::GoDown()
{
    ActiveMenuOption++;
    if(ActiveMenuOption >= nMenuOptions)
    {
        ActiveMenuOption = 0;
    }
}

lMenu::lMenu(unsigned int n_menu_options)
    :nMenuOptions(n_menu_options),ActiveMenuOption(0)
{

}

lMenu::~lMenu()
{

}
