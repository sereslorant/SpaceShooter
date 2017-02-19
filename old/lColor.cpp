
#include "lColor.h"

lColor::lColor()
    :Red(0.0),Green(0.0),Blue(0.0),Alpha(1.0)
{

}

lColor::lColor(float red,float green,float blue,float alpha)
    :Red(red),Green(green),Blue(blue),Alpha(alpha)
{

}

lColor::lColor(const lColor &color)
    :Red(color.Red),Green(color.Green),Blue(color.Blue),Alpha(color.Alpha)
{

}
