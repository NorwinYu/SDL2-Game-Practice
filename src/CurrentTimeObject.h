#pragma once

#include "DisplayableObject.h"
#include "Psyby2Engine.h"

class CurrentTimeObject :
	public DisplayableObject
{
public:
	CurrentTimeObject(Psyby2Engine * pEngine);
	~CurrentTimeObject();

	void DoUpdate(int iCurrentTime);
	int GetLastTime();
	void SetLastTime(int iLastTime);
	void Draw();

private:
	Psyby2Engine* m_pMainEngine;
	int iLastTime;
};

