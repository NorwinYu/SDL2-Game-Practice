#pragma once

#include "DisplayableObject.h"
#include "Psyby2Engine.h"
#include "JPGImage.h"
#include "MovementPosition.h"

class HelicopterObject :
	public DisplayableObject
{
public:
	HelicopterObject(Psyby2Engine* pEngine);
	~HelicopterObject();
	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	Psyby2Engine* m_pMainEngine;
	ImageData image;
	MovementPosition m_oMover;
	bool isSetMove;
	bool isAddPlayer;
};

