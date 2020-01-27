#pragma once

#include <random>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

#include "BaseEngine.h"
#include "Psyby2TileManager.h"

class Psyby2Engine :
	public BaseEngine
{
public:
	Psyby2Engine();
	~Psyby2Engine();

	void SetupBackgroundBuffer();
	void KeyDown(int iKeyCode);
	void UnDrawStrings();
	void DrawStringsUnderneath();
	int InitialiseObjects();
	void UndrawObjects();
	void DrawObjects();

	Psyby2TileManager& GetTileManager() { return tileManager; }

	bool getKeyDown(int moveCode);
	void setKeyDown(int moveCode, bool isDown);
	void GameAction();

	enum State { stateInit, stateStart, stateInitCity, stateMidCity, stateEndCity, stateMap, stateInfo, statePaused };
	int m_iPauseStarted;

private:
	Psyby2TileManager tileManager;

	bool UpkeyDown;
	bool DownkeyDown;
	bool LeftkeyDown;
	bool RightkeyDown;
	bool TDown;
	
	bool InfoMouseDown;

	State m_state;
	State pre_state;

public:
	void AddPlayer();
	void NextMap();
	void changeBlockTile(int preMoveCode, int m_iCurrentScreenX, int m_iCurrentScreenY);
	void setState(State newState);
	State GetState() { return m_state; }
	void toMapState();
	void toInfoState();
	void MouseDown(int iButton, int iX, int iY);
	bool GetInfoMouseDown();
	void SetInfoMouseDown(bool isDown);
	void saveGame();
	void loadGame();
};