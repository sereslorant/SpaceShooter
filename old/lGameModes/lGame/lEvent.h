#ifndef L_EVENT_H
#define L_EVENT_H

#include "liEvent.h"

#include <list>

class lEvent : public liEvent
{
private:
	std::list<liEventCallback *> Callbacks;

public:
	virtual void AddCallback(liEventCallback *callback) override;
	virtual void Fire() override;

	lEvent();
	virtual ~lEvent() override;
};

#endif // L_EVENT_H
