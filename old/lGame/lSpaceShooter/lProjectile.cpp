
#include "lProjectile.h"

using namespace std;

float lProjectile::GetSpeed() const
{
    return Speed;
}

float lProjectile::GetDamage() const
{
    return Damage;
}

SPECIES lProjectile::GetSpecies() const
{
    return Species;
}

lRay *lProjectile::GetTrajectory()
{
    return Trajectory;
}

void lProjectile::Update(float dt)
{
    Trajectory->SetPosition(Trajectory->GetPoint(Speed * dt));
}

void lProjectile::Draw()
{
	glPointSize(4.0);

    glLoadIdentity();
    glBegin(GL_POINTS);

    glColor4f(1.0,1.0,1.0,1.0);
    glVertex2f(Trajectory->GetPosition().X,Trajectory->GetPosition().Y);

    glEnd();
}

lProjectile::lProjectile(const lmVector2D &position,const lmVector2D &direction,float speed,float damage,SPECIES species)
    :Speed(speed),Damage(damage),Species(species)
{
    Trajectory = new lRay(position,direction);
}

lProjectile::~lProjectile()
{
    delete Trajectory;
}
