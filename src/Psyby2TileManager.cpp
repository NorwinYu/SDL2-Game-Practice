#include "header.h"

#include "Psyby2TileManager.h"

Psyby2TileManager::Psyby2TileManager()
	: TileManager(50, 50)
{
}


Psyby2TileManager::~Psyby2TileManager()
{
}

void Psyby2TileManager::DrawTileAt(
	BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	std::string filePath("resources/map/map_");
	filePath += std::to_string(GetValue(iMapX, iMapY)) + ".bmp";
	ImageData image;
	if (image.LoadImage(filePath.c_str()))
	{
		//std::cout << "loading background image -> success :: Path : " << filePath << std::endl;
	}
	else
	{
		std::cout << "loading background image -> fail :: Path : " << filePath << std::endl;
		return;
	}
	//image.ResizeFrom(&image, GetTileWidth(), GetTileHeight(), false);

	image.RenderImage(pSurface, 0, 0,
		iStartPositionScreenX,
		iStartPositionScreenY,
		GetTileWidth(), GetTileHeight());
}


bool Psyby2TileManager::isTileBlocked(int pX, int pY)
{
	return GetValue(pX / 50, (pY - 50) / 50) > 10;
}


int Psyby2TileManager::GetIMapWidth()
{
	return m_iMapWidth;
}


int Psyby2TileManager::GetIMapHeight()
{
	return m_iMapHeight;
}
