#include <cmath>

#include "lmVector2D.h"

using namespace std;

lmVector2D lmVector2D::NULL_VECTOR;

lmVector2D::lmVector2D(lmScalar x,lmScalar y): X(x),Y(y)
{

}

lmVector2D::lmVector2D(): X(0.0),Y(0.0)
{

}

lmVector2D::~lmVector2D()
{

}

lmScalar lmVector2D::LengthSquared() const
{
    return X*X + Y*Y;
}

lmVector2D lmVector2D::operator+(const lmVector2D &U) const
{
    lmVector2D V;

    V.X = X + U.X;
    V.Y = Y + U.Y;

    return V;
}

lmVector2D lmVector2D::operator-(const lmVector2D &U) const
{
    lmVector2D V;

    V.X = X - U.X;
    V.Y = Y - U.Y;

    return V;
}

lmVector2D lmVector2D::operator*(lmScalar l) const
{
    lmVector2D V;

    V.X = X * l;
    V.Y = Y * l;

    return V;
}

const lmVector2D &lmVector2D::operator+=(const lmVector2D &U)
{
    X += U.X;
    Y += U.Y;

    return *this;
}

const lmVector2D &lmVector2D::operator-=(const lmVector2D &U)
{
    X -= U.X;
    Y -= U.Y;

    return *this;
}

const lmVector2D &lmVector2D::operator*=(lmScalar l)
{
    lmVector2D V;

    X *= l;
    Y *= l;

    return *this;
}

lmScalar lmDot(const lmVector2D &U,const lmVector2D &V)
{
    return(U.X*V.X + U.Y*V.Y);
}

lmVector2D operator *(double l,const lmVector2D &V)
{
    return lmVector2D(V.X * l,V.Y * l);
}
