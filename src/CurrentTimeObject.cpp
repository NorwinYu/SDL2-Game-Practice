#include "header.h"

#include "CurrentTimeObject.h"

CurrentTimeObject::CurrentTimeObject(Psyby2Engine* pEngine)
: DisplayableObject(pEngine)
, m_pMainEngine(pEngine)
, iLastTime(0)
{
	m_iPreviousScreenX = m_iCurrentScreenX = 610;
	m_iPreviousScreenY = m_iCurrentScreenY = 50;

	SetVisible(true);
}

CurrentTimeObject::~CurrentTimeObject()
{
}

void CurrentTimeObject::DoUpdate(int iCurrentTime)
{
	if (m_pMainEngine->GetTime() - GetLastTime() > 1000)
	{
		SetLastTime(m_pMainEngine->GetTime());
		m_pMainEngine->Redraw(true);
	}
}


int CurrentTimeObject::GetLastTime()
{
	return iLastTime;
}


void CurrentTimeObject::SetLastTime(int iTime)
{
	this->iLastTime = iTime;
}


void CurrentTimeObject::Draw()
{
}
