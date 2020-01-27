#pragma once

#include "DisplayableObject.h"
#include "Psyby2Engine.h"
#include "JPGImage.h"
#include "MovementPosition.h"

class EnemyObject :
	public DisplayableObject
{
public:
	EnemyObject(Psyby2Engine* pEngine);
	~EnemyObject();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void startMove();
	void endMove();

private:
	Psyby2Engine* m_pMainEngine;
	ImageData image;
	MovementPosition m_oMover;
	int imageCode;
	int preMoveCode;
public:
	int GetImageCode();
	int GetPreMoveCode();
	int getCurrentScreenX();
	int getCurrentScreenY();
	void SetImageCode(int x);
	void SetPreMoveCode(int x);
	void SetCurrentScreenX(int x);
	void SetCurrentScreenY(int x);
};

