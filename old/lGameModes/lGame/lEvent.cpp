
#include "lEvent.h"

void lEvent::AddCallback(liEventCallback *callback)
{
	Callbacks.push_back(callback);
}

void lEvent::Fire()
{
	for(auto I : Callbacks)
	{
		I->Callback();
	}
}

lEvent::lEvent()
{

}

lEvent::~lEvent()
{

}
