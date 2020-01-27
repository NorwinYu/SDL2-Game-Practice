#include "header.h"

#include "ProgrammerObject.h"

ProgrammerObject::ProgrammerObject(Psyby2Engine* pEngine, const char* fileName)
: DisplayableObject(pEngine)
, m_pMainEngine(pEngine)
, preMoveCode(0)
, preMoveMark(0)
{
	if (image.LoadImage(fileName))
	{
		//std::cout << "loading player image -> success :: Path : " << fileName << std::endl;
	}
	else
	{
		std::cout << "loading player image -> fail :: Path : " << fileName << std::endl;
		return;
	}

    m_iPreviousScreenX = m_iCurrentScreenX = 400;
    m_iPreviousScreenY = m_iCurrentScreenY = 300;

	SetVisible(false);
}

ProgrammerObject::~ProgrammerObject()
{
}


ImageData ProgrammerObject::GetImage()
{
	return image;
}


void ProgrammerObject::SetImage(ImageData im)
{
	image = im;
}


void ProgrammerObject::Draw()
{
	if (!IsVisible())
		return;

	m_pMainEngine->DrawForegroundString(m_iCurrentScreenX - 10, m_iCurrentScreenY - 20, "You", m_pMainEngine->GetColour(21), NULL);

	image.RenderImageWithMask(m_pMainEngine->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		image.GetWidth(), image.GetHeight());

    StoreLastScreenPositionForUndraw();
}

void ProgrammerObject::DoUpdate(int iCurrentTime)
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
			m_pMainEngine->setKeyDown(preMoveCode, false);
			endMove();
			preMoveCode = 0;
		}

		if (m_pMainEngine->getKeyDown(1))
			moveCode = 1;
		else if (m_pMainEngine->getKeyDown(2))
			moveCode = 2;
		else if (m_pMainEngine->getKeyDown(3))
			moveCode = 3;
		else if (m_pMainEngine->getKeyDown(4))
			moveCode = 4;
		else if (m_pMainEngine->getKeyDown(5))
		{
			
			m_pMainEngine->changeBlockTile(preMoveMark, GetXCentre(), GetYCentre());
			m_pMainEngine->setKeyDown(5, false);
			std::cout << "Tool Keys Pressed" << std::endl;
		}

		if (m_pMainEngine->GetInfoMouseDown())
		{
			std::cout << "To Player Info" << std::endl;
			if (m_pMainEngine->GetMouseXClickedDown() >= GetXCentre() - 25 &&
				m_pMainEngine->GetMouseXClickedDown() <= GetXCentre() + 25 &&
				m_pMainEngine->GetMouseYClickedDown() >= GetYCentre() - 25 &&
				m_pMainEngine->GetMouseYClickedDown() <= GetYCentre() + 25
				)
			{
				m_pMainEngine->toInfoState();
			}
			m_pMainEngine->SetInfoMouseDown(false);
		}

		if (moveCode == 0)
			return;

		switch (moveCode)
		{
		case 1:
			nextY -= 52;
			std::cout << "Up Keys Pressed" << std::endl;
			break;
		case 2:
			nextY += 52;
			std::cout << "Down Keys Pressed" << std::endl;
			break;
		case 3:
			nextX -= 52;
			std::cout << "Left Keys Pressed" << std::endl;
			break;
		case 4:
			nextX += 52;
			std::cout << "Right Keys Pressed" << std::endl;
			break;

		}

		preMoveCode = moveCode;
		preMoveMark = moveCode;

		if (nextX < 0)
		{
			m_iCurrentScreenX = m_pMainEngine->GetWindowWidth() - 50;
			m_pMainEngine->SetupBackgroundBuffer();
			m_pMainEngine->Redraw(true);
			return;
			//nextX = 0
		}

		if (nextX >= m_pMainEngine->GetWindowWidth() - image.GetWidth())
		{
			switch (m_pMainEngine->GetState())
			{
				case stateInitCity:
				{
					m_pMainEngine->toMapState();
					m_iCurrentScreenX = 0;
					return;
				}
				break;
			}

			m_iCurrentScreenX = 0;
			m_pMainEngine->SetupBackgroundBuffer();
			m_pMainEngine->Redraw(true);
			return;
			//nextX = m_pMainEngine->GetWindowWidth() - image.GetWidth();
		}

		if (nextY < 50)
		{
			//nextY = 50;
			m_iCurrentScreenY = 500;
			m_pMainEngine->SetupBackgroundBuffer();
			m_pMainEngine->Redraw(true);
			return;
		}

		if (nextY >= m_pMainEngine->GetWindowHeight() - image.GetHeight() - 150)
		{
			//nextY = m_pMainEngine->GetWindowHeight() - image.GetHeight() - 150;
			m_iCurrentScreenY = 50;
			m_pMainEngine->SetupBackgroundBuffer();
			m_pMainEngine->Redraw(true);
			return;
		}

		Psyby2TileManager* psyby2TileManager = &(m_pMainEngine->GetTileManager());
		bool isNextBlocked = psyby2TileManager->isTileBlocked(nextX + image.GetWidth()/2, nextY + image.GetHeight()/2);

		if (isNextBlocked)
		{
			std::cout << "Target Tile is blocked : nextX: "<< nextX << " nextY: "<< nextY << std::endl;
			m_pMainEngine->setKeyDown(moveCode, false);
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

		std::string fileName("resources/character/Player/p_");
		int speed = 4;

		int imgaeCode = (iCurrentTime % (120 * speed)) / (40 * speed) + 1;
		fileName += std::to_string(preMoveCode) + "_" + std::to_string(imgaeCode) + ".png";

		if (image.LoadImage(fileName.c_str()))
		{
			//std::cout << "loading player image -> success :: Path : " << fileName << std::endl;
		}
		else
		{
			std::cout << "loading player image -> fail :: Path : " << fileName << std::endl;
			return;
		}
	}

	RedrawObjects();
	m_pMainEngine->Redraw(true);
}

void ProgrammerObject::startMove()
{

}


void ProgrammerObject::endMove()
{
	std::string fileName("resources/character/Player/p_");
	fileName += std::to_string(preMoveCode) + "_1.png";
	if (image.LoadImage(fileName.c_str()))
	{
		//std::cout << "loading player image -> success :: Path : " << fileName << std::endl;
	}
	else
	{
		std::cout << "loading player image -> fail :: Path : " << fileName << std::endl;
		return;
	}
}

int ProgrammerObject::GetPreMoveMark()
{
	return preMoveMark;
}


int ProgrammerObject::GetPreMoveCode()
{
	return preMoveCode;
}


int ProgrammerObject::getCurrentScreenX()
{
	return m_iCurrentScreenX;
}


int ProgrammerObject::getCurrentScreenY()
{
	return m_iCurrentScreenY;
}

void ProgrammerObject::SetPreMoveMark(int x)
{
	preMoveMark = x;
}


void ProgrammerObject::SetPreMoveCode(int x)
{
	preMoveCode = x;
}

void ProgrammerObject::SetCurrentScreenX(int x)
{
	m_iCurrentScreenX = x;
}


void ProgrammerObject::SetCurrentScreenY(int x)
{
	m_iCurrentScreenY = x;
}
