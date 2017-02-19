#ifndef L_JRPG_COLLISION_CALLBACK
#define L_JRPG_COLLISION_CALLBACK

#include "lprotoObject.h"

class lprotoCollisionCallback
{
public:
	lprotoCollisionCallback()
	{

	}

	virtual ~lprotoCollisionCallback()
	{

	}

	virtual void Callback(lprotoObject *obj) = 0;
};

#endif
