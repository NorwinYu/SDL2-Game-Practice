#include "header.h"

#include "Psyby2Engine.h"
#include "CurrentTimeObject.h"
#include "ProgrammerObject.h"
#include "EnemyObject.h"
#include "HelicopterObject.h"


Psyby2Engine::Psyby2Engine()
: m_state(stateInit)
, pre_state(stateInit)
{
}


Psyby2Engine::~Psyby2Engine()
{
}


void Psyby2Engine::SetupBackgroundBuffer()
{
	switch (m_state)
	{
	case stateInit:
	{
		ImageData image;
		if (image.LoadImage("resources/init.png"))
		{
			std::cout << "loading init image -> success :: Path : " << "resources/init.png" << std::endl;
		}
		else
		{
			std::cout << "loading init image -> fail :: Path : " << "resources/init.png" << std::endl;
			return;
		}

		image.RenderImageWithMask(GetBackground(),
			0, 0,
			0, 0,
			image.GetWidth(), image.GetHeight());
	}
		break;

	case stateStart:
	{
		ImageData image;
		if (image.LoadImage("resources/start.png"))
		{
			std::cout << "loading start image -> success :: Path : " << "resources/start.png" << std::endl;
		}
		else
		{
			std::cout << "loading start image -> fail :: Path : " << "resources/start.png" << std::endl;
			return;
		}

		image.RenderImageWithMask(GetBackground(),
			0, 0,
			0, 0,
			image.GetWidth(), image.GetHeight());
	}
		break;
	
	case stateInfo:
	{
		ImageData image;
		if (image.LoadImage("resources/info.png"))
		{
			std::cout << "loading info image -> success :: Path : " << "resources/info.png" << std::endl;
		}
		else
		{
			std::cout << "loading info image -> fail :: Path : " << "resources/info.png" << std::endl;
			return;
		}

		image.RenderImageWithMask(GetBackground(),
			0, 0,
			0, 0,
			image.GetWidth(), image.GetHeight());

		GetDisplayableObject(2)->SetVisible(false);
		GetDisplayableObject(3)->SetVisible(false);
		GetDisplayableObject(4)->SetVisible(false);
		GetDisplayableObject(5)->SetVisible(false);
	}
		break;

	case stateInitCity:
	{
		FillBackground(GetColour(7));

		DrawBackgroundThickLine(0, 50, 800, 50, GetColour(2), 5);
		DrawBackgroundThickLine(0, 550, 800, 550, GetColour(2), 5);
		DrawBackgroundThickLine(200, 0, 200, 50, GetColour(2), 5);
		DrawBackgroundThickLine(600, 0, 600, 50, GetColour(2), 5);

		DrawBackgroundString(270, 10, "Programmer Story", GetColour(2), NULL);

		tileManager.SetSize(50, 50);

		const char* data[] = {
			"hhhhhhhhhhhhhhhh",
			"hhhhhhhhhhhhhhhh",
			"hhhmmmmmmmmmhhhh",
			"hhhmdddddddmhhhh",
			"hhhmdfffffdmmmmm",
			"hhhmdfffffeeeeee",
			"hhhmdfffffdmmmmm",
			"hhhmdddddddmhhhh",
			"hhhmmmmmmmmmhhhh",
			"hhhhhhhhhhhhhhhh"};

		for (int x = 0; x <= 15; x++)
			for (int y = 0; y <= 9; y++)
				tileManager.SetValue(x, y, data[y][x] - 'a');

		std::cout << "Map: " << std::endl;
		for (int y = 0; y <= 9; y++)
		{
			for (int x = 0; x <= 15; x++)
				std::cout << tileManager.GetValue(x, y) << ' ';
			std::cout << std::endl;
		}

		tileManager.SetBaseTilesPositionOnScreen(0, 50);

		tileManager.DrawAllTiles(this,
			this->GetBackground(),
			0, 0, 15, 9);

		GetDisplayableObject(1)->SetVisible(true);
	}
		break;

	case stateMidCity:
		break;

	case stateEndCity:
		break;

	case stateMap:
	{
		GetDisplayableObject(1)->SetVisible(false);
		if (pre_state != statePaused)
		{
			NextMap();

			GetDisplayableObject(2)->SetVisible(true);
			GetDisplayableObject(3)->SetVisible(true);
			GetDisplayableObject(4)->SetVisible(true);
			GetDisplayableObject(5)->SetVisible(true);
		}
	}
		break;
	case statePaused:
		break;
	}
}

void Psyby2Engine::KeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		SetExitWithCode(0);
		break;
	case SDLK_SPACE:
		pre_state = m_state;
		switch (m_state)
		{
		case stateInit:
			setState(stateStart);
			break;
		case stateStart:
			setState(stateInitCity);
			break;
		case stateInitCity:
			break;
		case stateMap:
			setState(statePaused);
			m_iPauseStarted = GetTime();
			break;
		case statePaused:
			setState(stateMap);
			IncreaseTimeOffset(m_iPauseStarted - GetTime());
			break;
		}
		break;
	case SDLK_UP:
		UpkeyDown = true;
		break;
	case SDLK_DOWN:
		DownkeyDown = true;
		break;
	case SDLK_LEFT:
		LeftkeyDown = true;
		break;
	case SDLK_RIGHT:
		RightkeyDown = true;
		break;
	case SDLK_t:
		TDown = true;
		break;
	case SDLK_s:
		saveGame();
		break;
	case SDLK_l:
		loadGame();
		break;
	}
}


void Psyby2Engine::UnDrawStrings()
{
	CopyBackgroundPixels(0, 0, GetWindowWidth(), 50);
}

void Psyby2Engine::DrawStringsUnderneath()
{
	switch (m_state)
	{
	case stateInit:
	case stateStart:
	case stateInfo:
		break;
	case stateInitCity:
	case statePaused:
	case stateMap:
	{
		time_t now = time(0);
		tm *ltm = localtime(&now);
		std::string currentTime("");
		currentTime += std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
		//std::cout << "Time: " << currentTime << std::endl;
		DrawForegroundString(650, 10, currentTime.c_str(), GetColour(2), NULL);
	}
		break;
	}
}

int Psyby2Engine::InitialiseObjects()
{
	DrawableObjectsChanged();
	DestroyOldObjects();
	CreateObjectArray(6);

	StoreObjectInArray(0, new CurrentTimeObject(this));
	StoreObjectInArray(1, new HelicopterObject(this));
	StoreObjectInArray(2, new ProgrammerObject(this, "resources/character/Player/p_2_1.png"));
	StoreObjectInArray(3, new EnemyObject(this));
	StoreObjectInArray(4, new EnemyObject(this));
	StoreObjectInArray(5, new EnemyObject(this));

	UpkeyDown = false;
	DownkeyDown = false;
	LeftkeyDown = false;
	RightkeyDown = false;
	TDown = false;
	InfoMouseDown = false;

	return 6;
}

bool Psyby2Engine::getKeyDown(int code)
{
	switch (code)
	{
	case 1:
		return UpkeyDown;
		break;
	case 2:
		return DownkeyDown;
		break;
	case 3:
		return LeftkeyDown;
		break;
	case 4:
		return RightkeyDown;
		break;
	case 5:
		return TDown;
		break;
	}
	return false;
}


void Psyby2Engine::setKeyDown(int code, bool isDown)
{
	switch (code)
	{
	case 1:
		UpkeyDown = isDown;
		break;
	case 2:
		DownkeyDown = isDown;
		break;
	case 3:
		LeftkeyDown = isDown;
		break;
	case 4:
		RightkeyDown = isDown;
		break;
	case 5:
		TDown = isDown;
		break;
	}
}

void Psyby2Engine::GameAction()
{
	if (!IsTimeToAct())
		return;

	SetTimeToAct(1);

	switch (m_state)
	{
	case stateInit:
	case stateStart:
	case statePaused:
	case stateInfo:
		break;
	case stateMap:
	case stateInitCity:
		UpdateAllObjects(GetModifiedTime());
		break;
	}
}

void Psyby2Engine::UndrawObjects()
{
	if (m_state != stateInit && m_state != stateStart)
		BaseEngine::UndrawObjects();
}

void Psyby2Engine::DrawObjects()
{
	if (m_state != stateInit && m_state != stateStart)
		BaseEngine::DrawObjects();
}

void Psyby2Engine::AddPlayer()
{
	GetDisplayableObject(2)->SetVisible(true);
}


void Psyby2Engine::NextMap()
{
	FillBackground(GetColour(7));

	DrawBackgroundThickLine(0, 50, 800, 50, GetColour(2), 5);
	DrawBackgroundThickLine(0, 550, 800, 550, GetColour(2), 5);
	DrawBackgroundThickLine(200, 0, 200, 50, GetColour(2), 5);
	DrawBackgroundThickLine(600, 0, 600, 50, GetColour(2), 5);

	DrawBackgroundString(270, 10, "Programmer Story", GetColour(2), NULL);

	tileManager.SetSize(50, 50);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 12);

	for (int x = 0; x <= 15; x++)
		for (int y = 0; y <= 9; y++)
			tileManager.SetValue(x, y, dis(gen));

	std::cout << "Map: " << std::endl;
	for (int y = 0; y <= 9; y++)
	{
		for (int x = 0; x <= 15; x++)
			std::cout << tileManager.GetValue(x, y) << ' ';
		std::cout << std::endl;
	}

	tileManager.SetBaseTilesPositionOnScreen(0, 50);

	tileManager.DrawAllTiles(this,
		this->GetBackground(),
		0, 0, 15, 9);
}

void Psyby2Engine::changeBlockTile(int preMoveCode, int xCenter, int yCenter)
{
	int iMapX = xCenter / tileManager.GetTileWidth();
	int iMapY = (yCenter - 50) / tileManager.GetTileHeight();
	std::cout << xCenter << " "<< yCenter << " "<< iMapX << " " << iMapY << std::endl;
	switch (preMoveCode)
	{
	case 1:
		iMapY--;
		break;
	case 0:
	case 2:
		iMapY++;
		break;
	case 3:
		iMapX--;
		break;
	case 4:
		iMapX++;
		break;
	}

	if (iMapX < 0 || iMapX > tileManager.GetIMapWidth() ||
		iMapX < 0 || iMapY > tileManager.GetIMapHeight())
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 12);

	int newValue = dis(gen);

	std::cout << "Set Map(" << iMapX << ", " << iMapY << ") to " 
		<< newValue << std::endl;

	tileManager.UpdateTile(this, iMapX, iMapY, newValue);
	Redraw(true);
}

void Psyby2Engine::setState(State newState)
{
	pre_state = m_state;
	m_state = newState;
	SetupBackgroundBuffer();
	Redraw(true);
}


void Psyby2Engine::toMapState()
{
	setState(stateMap);
}


void Psyby2Engine::toInfoState()
{
	setState(stateInfo);
}

void Psyby2Engine::MouseDown(int iButton, int iX, int iY)
{
	switch (m_state)
	{
	case stateInit:
	case stateStart:
	case statePaused:
	case stateInitCity:
		break;
	case stateMap:
	{
		InfoMouseDown = true;
	}
		break;
	case stateInfo:
	{
		if (iX > 15 && iX < 75 && iY > 20 && iY < 120)
			toMapState();
	}
		break;
	}
}


bool Psyby2Engine::GetInfoMouseDown()
{
	return InfoMouseDown;
}


void Psyby2Engine::SetInfoMouseDown(bool isDown)
{
	InfoMouseDown = isDown;
}


void Psyby2Engine::saveGame()
{
	std::ofstream outfile;
	outfile.open("game.dat");

	std::cout << "Saving Game ..." << std::endl;
	outfile << "***Game-Save-File***" << std::endl;
	outfile << "<>Engine" << std::endl;
	outfile << "---m_iPauseStarted" << std::endl;
	outfile << m_iPauseStarted << std::endl;
	outfile << "---m_state" << std::endl;
	outfile << m_state << std::endl;
	outfile << "---pre_state" << std::endl;
	outfile << pre_state << std::endl;
	outfile << "<>Enemy" << std::endl;

	for (size_t i = 3; i < 6; i++)
	{
		outfile << "===E_" << i - 2 << std::endl;
		DisplayableObject *object = GetDisplayableObject(i);
		EnemyObject *enemy = dynamic_cast<EnemyObject*>(object);
		outfile << "---imageCode" << std::endl;
		outfile << enemy->GetImageCode() << std::endl;
		outfile << "---preMoveCode" << std::endl;
		outfile << enemy->GetPreMoveCode() << std::endl;
		outfile << "---m_iCurrentScreenX" << std::endl;
		outfile << enemy->getCurrentScreenX() << std::endl;
		outfile << "---m_iCurrentScreenY" << std::endl;
		outfile << enemy->getCurrentScreenY() << std::endl;
	}

	outfile << "===P"<< std::endl;
	DisplayableObject *object = GetDisplayableObject(2);
	ProgrammerObject *enemy = dynamic_cast<ProgrammerObject*>(object);
	outfile << "---preMoveMark" << std::endl;
	outfile << enemy->GetPreMoveMark() << std::endl;
	outfile << "---preMoveCode" << std::endl;
	outfile << enemy->GetPreMoveCode() << std::endl;
	outfile << "---m_iCurrentScreenX" << std::endl;
	outfile << enemy->getCurrentScreenX() << std::endl;
	outfile << "---m_iCurrentScreenY" << std::endl;
	outfile << enemy->getCurrentScreenY() << std::endl;

	outfile << "***End***" << std::endl;

	outfile.close();
}


void Psyby2Engine::loadGame()
{
	char data[100];

	std::ifstream infile;
	infile.open("game.dat");

	std::cout << "Loading Game ... " << std::endl;

	for (size_t i = 0; i < 2; i++)
	{
		infile >> data;
	}
	for (size_t i = 0; i < 3; i++)
	{
		infile >> data;
		infile >> data;

		int x;
		try {
			x = std::stoi(data);
		}
		catch (std::exception const &e) {
			
		}
		
		switch (i)
		{
		case 0:
			m_iPauseStarted = x;
			break;
		case 1:
		{
			if (m_state == State(x) && m_state == stateMap)
			{
				GetDisplayableObject(1)->SetVisible(false);

				for (size_t i = 0; i < 4; i++)
				{
					if (GetDisplayableObject(i + 2) != NULL)
					{
						GetDisplayableObject(i + 2)->SetVisible(true);
					}
				}
			}
			else
			{
				for (size_t i = 0; i < 5; i++)
				{
					if (GetDisplayableObject(i + 1) != NULL)
					{
						GetDisplayableObject(i + 1)->SetVisible(false);
					}

					if (GetDisplayableObject(i + 2) == NULL)
					{
						i = 5;
					}
				}

				setState(State(x));
			}
		}
			break;
		case 2:
			pre_state = State(x);
			break;
		}
	}

	infile >> data;
	for (size_t i = 0; i < 4; i++)
	{
		infile >> data;
		for (size_t j = 0; j < 4; j++)
		{
			infile >> data;
			infile >> data;
			
			if (i != 3)
			{
				DisplayableObject *object = GetDisplayableObject(i+3);
				EnemyObject *enemy = dynamic_cast<EnemyObject*>(object);

				int x;
				try {
					x = std::stoi(data);
				}
				catch (std::exception const &e) {

				}

				switch (j)
				{
				case 0:
					enemy->SetImageCode(x);
					break;
				case 1:
					enemy->SetPreMoveCode(x);
					break;
				case 2:
					enemy->SetCurrentScreenX(x);
					break;
				case 3:
					enemy->SetCurrentScreenY(x);
					break;
				}
			}
			else
			{
				DisplayableObject *object = GetDisplayableObject(2);
				ProgrammerObject *enemy = dynamic_cast<ProgrammerObject*>(object);
				
				int x;
				try {
					x = std::stoi(data);
				}
				catch (std::exception const &e) {

				}

				switch (j)
				{
				case 0:
					enemy->SetPreMoveMark(x);
					break;
				case 1:
					enemy->SetPreMoveCode(x);
					break;
				case 2:
					enemy->SetCurrentScreenX(x);
					break;
				case 3:
					enemy->SetCurrentScreenY(x);
					break;
				}
			}
		}
	}

	Redraw(true);

	infile.close();
}
