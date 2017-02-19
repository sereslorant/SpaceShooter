
#include "lGM_AICommand_Goto.h"


bool lTD_UnitGoto(lGM_Fighter *unit,const lmVector2D &destination,float epsilon)
{
	const lmVector2D &AvatarPos = unit->GetPosition();

	bool Arrived = true;
	float Tmp;

	Tmp = destination.Y - AvatarPos.Y;
	if(std::abs(Tmp) > epsilon)
	{
		if(Tmp < 0.0)
		{
			unit->SetUp(true);
		}
		else
		{
			unit->SetDown(true);
		}

		Arrived = false;
	}
	else
	{
		unit->SetUp(false);
		unit->SetDown(false);
	}

	Tmp = destination.X - AvatarPos.X;
	if(std::abs(Tmp) > epsilon)
	{
		if(Tmp < 0.0)
		{
			unit->SetLeft(true);
		}
		else
		{
			unit->SetRight(true);
		}

		Arrived = false;
	}
	else
	{
		unit->SetLeft(false);
		unit->SetRight(false);
	}

	return Arrived;
}
