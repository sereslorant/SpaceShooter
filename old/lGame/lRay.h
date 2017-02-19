#ifndef L_RAY
#define L_RAY

#include "../lMath/lmVector2D.h"

class lRay
{
private:
    lmVector2D Position;
    lmVector2D Direction;
    lmVector2D Normal;

public:
    lRay()
    {

    }
    lRay(const lmVector2D &position,const lmVector2D &direction);

    const lmVector2D &GetPosition() const;
    const lmVector2D &GetDirection() const;
    const lmVector2D &GetNormal() const;

    lmVector2D GetPoint(float l) const;

    void SetPosition(const lmVector2D &position);
    void SetDirection(const lmVector2D &direction);

    void Draw(float l);
};

#endif
