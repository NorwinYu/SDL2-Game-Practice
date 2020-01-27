#pragma once

#include <iostream>
#include <string>

#include "DisplayableObject.h"
#include "Psyby2Engine.h"
#include "JPGImage.h"
#include "MovementPosition.h"

class ProgrammerObject :
	public DisplayableObject
{
public:
	ProgrammerObject(Psyby2Engine* pEngine, const char* fileName);
	~ProgrammerObject();
	ImageData GetImage();
	void SetImage(ImageData im);
    void Draw();
    void DoUpdate( int iCurrentTime );
	void startMove();
	void endMove();
	enum State { stateInit, stateStart, stateInitCity, stateMidCity, stateEndCity, stateMap, stateInfo, statePaused };

private:
	Psyby2Engine* m_pMainEngine;
	ImageData image;
	MovementPosition m_oMover;
	int preMoveCode;
	int preMoveMark;

public:
	int GetPreMoveMark();
	int GetPreMoveCode();
	int getCurrentScreenX();
	int getCurrentScreenY();
	void SetPreMoveMark(int x);
	void SetPreMoveCode(int x);
	void SetCurrentScreenX(int x);
	void SetCurrentScreenY(int x);
};

