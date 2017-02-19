#include <cmath>

#include "lmGeometry2D.h"

using namespace std;

bool lmLineLineIntersection(const lmVector2D &line0_point,const lmVector2D &line0_dir,const lmVector2D &line1_point,const lmVector2D &line1_normal,lmScalar *iDist,lmVector2D *iPoint)
{
    lmScalar i = lmDot(line0_dir,line1_normal);
    lmVector2D V = line1_point - line0_point;

    if(abs(i) > 1e-6)
    {
        lmScalar Mlt = lmDot(V,line1_normal);
        Mlt /= i;

        if(iDist != nullptr)
            {*iDist = Mlt;}

        if(iPoint != nullptr)
            {*iPoint = line0_point + (line0_dir * Mlt);}

        return true;
    }
    else
    {
        if(sqrt(V.LengthSquared()) < 1e-6)
        {
            if(iDist != nullptr)
                {*iDist = 0.0;}

            if(iPoint != nullptr)
                {*iPoint = lmVector2D::NULL_VECTOR;}

            return true;
        }

        return false;
    }
}

bool lmCircleLineIntersection(const lmVector2D &circle_center,lmScalar circle_radius,const lmVector2D &line_point,const lmVector2D &line_normal,lmScalar *iDist,lmVector2D *iPoint)
{
    lmScalar    siDist  = 0.0;
    lmVector2D  siPoint = lmVector2D::NULL_VECTOR;

    if(lmLineLineIntersection(circle_center,line_normal * -1.0,line_point,line_normal,&siDist,&siPoint) && (abs(siDist) <= circle_radius))
    {
        if(iDist != nullptr)
            {*iDist = siDist;}

        if(iPoint != nullptr)
            {*iPoint = siPoint;}

        return true;
    }

    if(iDist != nullptr)
        {*iDist = 0.0;}

    if(iPoint != nullptr)
        {*iPoint = lmVector2D::NULL_VECTOR;}

    return false;
}

bool lmCircleCircleIntersection(const lmVector2D &circle0_center,const lmScalar circle0_radius,const lmVector2D &circle1_center,const lmScalar circle1_radius)
{
    lmVector2D V = circle1_center + (circle0_center * -1.0);
    lmScalar Dist = /*sqrt(*/V.LengthSquared()/*)*/;

    if(Dist <= (circle0_radius + circle1_radius)*(circle0_radius + circle1_radius))
    {
        V.Normalize();

        return true;
    }

    return false;
}

bool lmPointAABBIntersection(const lmVector2D &point,const lmVector2D &aabb_min,const lmVector2D &aabb_max)
{
    if(point.X < aabb_min.X)
    {
        return false;
    }
    if(point.X > aabb_max.X)
    {
        return false;
    }
    if(point.Y < aabb_min.Y)
    {
        return false;
    }
    if(point.Y > aabb_max.Y)
    {
        return false;
    }

    return true;
}

bool lmAABBAABBIntersection(const lmVector2D &aabb0_min,const lmVector2D &aabb0_max,const lmVector2D &aabb1_min,const lmVector2D &aabb1_max)
{
    if (aabb0_max.Y < aabb1_min.Y)
    {
        return false;
    }

    else if (aabb0_min.Y > aabb1_max.Y)
    {
        return false;
    }

    else if (aabb0_max.X < aabb1_min.X)
    {
        return false;
    }

    else if (aabb0_min.X > aabb1_max.X)
    {
        return false;
    }

    return true;
};
