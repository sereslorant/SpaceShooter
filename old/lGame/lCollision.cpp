
#include "lCollision.h"

#include "../lMath/lmGeometry2D.h"

using namespace std;

bool CheckCollision(const lRect *Q,const lRay *R,float *Dist,lmVector2D *Intersection)
{
    lmVector2D  P0 = Q->GetPosition();

    lmVector2D  P1 = Q->GetPosition();
                P1.X += Q->GetWidth();

    lmVector2D  P2 = P1;
                P2.Y += Q->GetHeight();

    lmVector2D  P3 = Q->GetPosition();
                P3.Y += Q->GetHeight();

    lRay    s1(P0,P1 + (P0*(-1))),
            s2(P1,P2 + (P1*(-1))),
            s3(P2,P3 + (P2*(-1))),
            s4(P3,P0 + (P3*(-1)));

    bool Collides = false;
    float TmpMlt;
    lmVector2D TmpPoint;

    float MinMlt;
    lmVector2D MinVec;

    if(lmLineLineIntersection(R->GetPosition(),R->GetDirection(),s1.GetPosition(),s1.GetNormal(),&TmpMlt,&TmpPoint))
    {
        lmVector2D  V1 = TmpPoint + (s1.GetPosition()*(-1));

        float t = sqrt((TmpPoint + (s1.GetPosition()*(-1))).LengthSquared() / s1.GetDirection().LengthSquared());

        if(t < 1.0 && t > 0.0 && lmDot(V1,s1.GetDirection()) > 0.0)
        {
            MinMlt = TmpMlt;
            MinVec = TmpPoint;

            Collides = true;
        }
    }

    if(lmLineLineIntersection(R->GetPosition(),R->GetDirection(),s2.GetPosition(),s2.GetNormal(),&TmpMlt,&TmpPoint))
    {
        lmVector2D  V1 = TmpPoint + (s2.GetPosition()*(-1));

        float t = sqrt((TmpPoint + (s2.GetPosition()*(-1))).LengthSquared() / s2.GetDirection().LengthSquared());

        if(t < 1.0 && t > 0.0 && lmDot(V1,s2.GetDirection()) > 0.0)
        {
            if(Collides != true)
            {
                MinMlt = TmpMlt;
                MinVec = TmpPoint;

                Collides = true;
            }
            else
            {
                if(TmpMlt < MinMlt)
                {
                    MinMlt = TmpMlt;
                    MinVec = TmpPoint;
                }
            }
        }
    }

    if(lmLineLineIntersection(R->GetPosition(),R->GetDirection(),s3.GetPosition(),s3.GetNormal(),&TmpMlt,&TmpPoint))
    {
        lmVector2D  V1 = TmpPoint + (s3.GetPosition()*(-1));

        float t = sqrt((TmpPoint + (s3.GetPosition()*(-1))).LengthSquared() / s3.GetDirection().LengthSquared());

        if(t < 1.0 && t > 0.0 && lmDot(V1,s3.GetDirection()) > 0.0)
        {
            if(Collides != true)
            {
                MinMlt = TmpMlt;
                MinVec = TmpPoint;

                Collides = true;
            }
            else
            {
                if(TmpMlt < MinMlt)
                {
                    MinMlt = TmpMlt;
                    MinVec = TmpPoint;
                }
            }
        }
    }

    if(lmLineLineIntersection(R->GetPosition(),R->GetDirection(),s4.GetPosition(),s4.GetNormal(),&TmpMlt,&TmpPoint))
    {
        lmVector2D  V1 = TmpPoint + (s4.GetPosition()*(-1));

        float t = sqrt((TmpPoint + (s4.GetPosition()*(-1))).LengthSquared() / s4.GetDirection().LengthSquared());

        if(t < 1.0 && t > 0.0 && lmDot(V1,s4.GetDirection()) > 0.0)
        {
            if(Collides != true)
            {
                MinMlt = TmpMlt;
                MinVec = TmpPoint;

                Collides = true;
            }
            else
            {
                if(TmpMlt < MinMlt)
                {
                    MinMlt = TmpMlt;
                    MinVec = TmpPoint;
                }
            }
        }
    }

    if(Dist != nullptr)
    {*Dist = MinMlt;}

    if(Intersection != nullptr)
    {*Intersection = MinVec;}

    return Collides;
}

bool CheckIfContains(const lRect *Rect,const lRect *Cont)
{
    if( Rect->GetPosition().Y                       > (Cont->GetPosition().Y)                       &&
        Rect->GetPosition().Y + Rect->GetHeight()   < (Cont->GetPosition().Y + Cont->GetHeight())   &&
        Rect->GetPosition().X                       > (Cont->GetPosition().X)                       &&
        Rect->GetPosition().X + Rect->GetWidth()    < (Cont->GetPosition().X + Cont->GetWidth())
      )
    {
        return true;
    }

    return false;
}
