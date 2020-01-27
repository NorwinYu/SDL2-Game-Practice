#include "header.h"

#include "HelicopterObject.h"

HelicopterObject::HelicopterObject(Psyby2Engine* pEngine)
: DisplayableObject(pEngine)
, m_pMainEngine(pEngine)
, isSetMove(false)
, isAddPlayer(false)
{
	std::string fileName("resources/helicopter/h_1.png");

	if (image.LoadImage(fileName.c_str()))
	{
		//std::cout << "loading helicopter image -> success :: Path : " << fileName << std::endl;
	}
	else
	{
		std::cout << "loading helicopter image -> fail :: Path : " << fileName << std::endl;
		return;
	}

	m_iPreviousScreenX = m_iCurrentScreenX = 1;

	m_iPreviousScreenY = m_iCurrentScreenY = 100;

	SetVisible(true);
}


HelicopterObject::~HelicopterObject()
{
}

void HelicopterObject::Draw()
{
	if (!IsVisible())
		return;

	image.RenderImageWithMask(m_pMainEngine->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		image.GetWidth(), image.GetHeight());

	StoreLastScreenPositionForUndraw();
}

void HelicopterObject::DoUpdate(int iCurrentTime)
{
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	int nextX = 300;
	int nextY = 250;

	if (m_oMover.HasMovementFinished(iCurrentTime))
	{
		if (isSetMove)
		{
			if (!isAddPlayer)
			{
				m_pMainEngine->AddPlayer();
			}
			return;
		}

		m_oMover.Setup(
			m_iCurrentScreenX,
			m_iCurrentScreenY,
			nextX,
			nextY,
			iCurrentTime,
			iCurrentTime + 4000);
		isSetMove = true;
	}

	if (!m_oMover.HasMovementFinished(iCurrentTime))
	{
		m_oMover.Calculate(iCurrentTime);
		m_iCurrentScreenX = m_oMover.GetX();
		m_iCurrentScreenY = m_oMover.GetY();

		std::string fileName("resources/helicopter/h_");
		int speed = 4;
		int imageCode = (iCurrentTime % (120 * speed)) / (60 * speed) + 1;
		fileName += std::to_string(imageCode) + ".png";

		if (image.LoadImage(fileName.c_str()))
		{
			//std::cout << "loading helicopter image -> success :: Path : " << fileName << std::endl;
		}
		else
		{
			std::cout << "loading helicopter image -> fail :: Path : " << fileName << std::endl;
			return;
		}
	}

	RedrawObjects();
	m_pMainEngine->Redraw(true);
}