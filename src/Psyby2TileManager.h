#pragma once

#include <iostream>
#include <string>

#include "TileManager.h"
#include "JPGImage.h"

class Psyby2TileManager :
	public TileManager
{
public:
	Psyby2TileManager();
	~Psyby2TileManager();

	virtual void DrawTileAt(
		BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;
	bool isTileBlocked(int pX, int pY);
	int GetIMapWidth();
	int GetIMapHeight();

private:
	int m_iMapWidth;
	int m_iMapHeight;
};

