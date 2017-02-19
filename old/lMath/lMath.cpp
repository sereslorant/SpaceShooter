
#include "lMath.h"

lmScalar lmDegToRad(lmScalar Deg)
{
    return Deg*PI/180.0;
}

lmScalar lmRadToDeg(lmScalar Rad)
{
    return Rad*180.0/PI;
}

lmScalar lmClamp(lmScalar Min,lmScalar Max,lmScalar x)
{
    if(x < Min)
    {
        return Min;
    }
    if(x > Max)
    {
        return Max;
    }

    return x;
}

lmScalar lmHeronFormula(const lmVector2D &A,const lmVector2D &B,const lmVector2D &C)
{
    lmScalar Side_A = sqrt((A-B).LengthSquared());
    lmScalar Side_B = sqrt((A-C).LengthSquared());
    lmScalar Side_C = sqrt((B-C).LengthSquared());

    lmScalar Semiperimeter =(Side_A+Side_B+Side_C)/2;

    return sqrt(Semiperimeter*(Semiperimeter-Side_A)*(Semiperimeter-Side_B)*(Semiperimeter-Side_C));
}
