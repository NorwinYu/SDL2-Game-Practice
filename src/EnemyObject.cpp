#include "header.h"

#include "EnemyObject.h"


EnemyObject::EnemyObject(Psyby2Engine* pEngine)
: DisplayableObject(pEngine)
, m_pMainEngine(pEngine)
, imageCode(0)
, preMoveCode(0)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 5);
	imageCode = dis(gen);

	std::string fileName("resources/character/Enemy/");
	fileName += std::to_string(imageCode) + "/e_2_1.png";

	if (image.LoadImage(fileName.c_str()))
	{
		std::cout << "loading enemy image -> success :: Path : " << fileName << std::endl;
	}
	else
	{
		std::cout << "loading enemy image -> fail :: Path : " << fileName << std::endl;
		return;
	}

	m_iPreviousScreenX = m_iCurrentScreenX = imageCode * 3 * 50;

	m_iPreviousScreenY = m_iCurrentScreenY = imageCode * 50 + 50;

	SetVisible(false);
}


EnemyObject::~EnemyObject()
{
}

void EnemyObject::Draw()
{
	if (!IsVisible())
		return;

	image.RenderImageWithMask(m_pMainEngine->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		image.GetWidth(), image.GetHeight());

	StoreLastScreenPositionForUndraw();
}

void EnemyObject::DoUpdate(int iCurrentTime)
{
	int moveCode = 0;

	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	int nextX = m_iCurrentScreenX;
	int nextY = m_iCurrentScreenY;

	if (m_oMover.HasMovementFinished(iCurrentTime))
	{
		if (preMoveCode != 0)
		{
			endMove();
			preMoveCode = 0;
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 4);
		moveCode = dis(gen);

		switch (moveCode)
		{
		case 1:
			nextY -= 50;
			break;
		case 2:
			nextY += 50;
			break;
		case 3:
			nextX -= 50;
			break;
		case 4:
			nextX += 50;
			break;

		}

		if (nextX < 0)
			nextX = 0;

		if (nextX >= m_pMainEngine->GetWindowWidth() - image.GetWidth())
			nextX = m_pMainEngine->GetWindowWidth() - image.GetWidth();

		if (nextY < 50)
			nextY = 50;

		if (nextY >= m_pMainEngine->GetWindowHeight() - image.GetHeight() - 150)
			nextY = m_pMainEngine->GetWindowHeight() - image.GetHeight() - 150;

		Psyby2TileManager* psyby2TileManager = &(m_pMainEngine->GetTileManager());
		bool isNextBlocked = psyby2TileManager->isTileBlocked(nextX + image.GetWidth() / 2, nextY + image.GetHeight() / 2);

		preMoveCode = moveCode;

		if (isNextBlocked)
		{
			std::cout << "Target Tile is blocked : nextX: " << nextX << " nextY: " << nextY << std::endl;
			endMove();
			preMoveCode = 0;
			return;
		}

		m_oMover.Setup(
			m_iCurrentScreenX,
			m_iCurrentScreenY,
			nextX,
			nextY,
			iCurrentTime,
			iCurrentTime + 1000);

		startMove();
	}

	if (!m_oMover.HasMovementFinished(iCurrentTime))
	{
		m_oMover.Calculate(iCurrentTime);
		m_iCurrentScreenX = m_oMover.GetX();
		m_iCurrentScreenY = m_oMover.GetY();

		std::string fileName("resources/character/Enemy/");
		fileName += std::to_string(imageCode) + "/e_";

		int speed = 4;

		int imageCode = (iCurrentTime % (120 * speed)) / (40 * speed) + 1;
		fileName += std::to_string(preMoveCode) + "_" + std::to_string(imageCode) + ".png";

		if (image.LoadImage(fileName.c_str()))
		{
			//std::cout << "loading enemy image -> success :: Path : " << fileName << std::endl;
		}
		else
		{
			std::cout << "loading enemy image -> fail :: Path : " << fileName << std::endl;
			return;
		}
	}

	RedrawObjects();
	m_pMainEngine->Redraw(true);
}

void EnemyObject::startMove()
{

}


void EnemyObject::endMove()
{
	std::string fileName("resources/character/Enemy/");
	fileName += std::to_string(imageCode) + "/e_";
	fileName += std::to_string(preMoveCode) + "_1.png";
	if (image.LoadImage(fileName.c_str()))
	{
		//std::cout << "loading enemy image -> success :: Path : " << fileName << std::endl;
	}
	else
	{
		std::cout << "loading enemy image -> fail :: Path : " << fileName << std::endl;
		return;
	}
}

int EnemyObject::GetImageCode()
{
	return imageCode;
}


int EnemyObject::GetPreMoveCode()
{
	return preMoveCode;
}


int EnemyObject::getCurrentScreenX()
{
	return m_iCurrentScreenX;
}


int EnemyObject::getCurrentScreenY()
{
	return m_iCurrentScreenY;
}

void EnemyObject::SetImageCode(int x)
{
	imageCode = x;
}


void EnemyObject::SetPreMoveCode(int x)
{
	preMoveCode = x;
}

void EnemyObject::SetCurrentScreenX(int x)
{
	m_iCurrentScreenX = x;
}


void EnemyObject::SetCurrentScreenY(int x)
{
	m_iCurrentScreenY = x;
}

