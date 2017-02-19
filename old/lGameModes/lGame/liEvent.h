#ifndef LI_EVENT_H
#define LI_EVENT_H

class liEventCallback
{
public:
	virtual void Callback() = 0;

	liEventCallback(){}
	virtual ~liEventCallback(){}
};

class liEvent
{
public:
	virtual void AddCallback(liEventCallback *callback) = 0;
	virtual void Fire() = 0;

	liEvent(){}
	virtual ~liEvent(){}
};

#endif // LI_EVENT_H
