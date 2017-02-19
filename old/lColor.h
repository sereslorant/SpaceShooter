#ifndef L_COLOR
#define L_COLOR

struct lColor
{
    float Red,Green,Blue,Alpha;

    lColor();
    lColor(float red,float green,float blue,float alpha);
    lColor(const lColor &color);
};

#endif
