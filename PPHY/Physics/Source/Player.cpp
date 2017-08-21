#include "Player.h"

//#include "MouseController.h"
//#include "KeyboardController.h"
#include "Mtx44.h"
#include "Application.h"
#include "SceneCollision.h"
#include "GameObject.h"
//#include "../Projectile/Projectile.h"
//#include "GoodiesFactory.h"
//#include "../SoundEngine.h"

// Allocating and initializing CPlayer's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayer *CPlayer::s_instance = 0;

CPlayer::CPlayer(void)
	: m_dSpeed(5.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(10.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, RL(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, m_dElapsedTime(0.0)
	, m_dElapsedIdleTime(0.0)
	, playerMoveIndex(0)
	, mapOffset_x(0)
	, mapOffset_y(0)
	, tileOffset_x(0)
	, tileOffset_y(0)
	, mapFineOffset_x(0)
	, mapFineOffset_y(0)
	//, theMapReference(NULL)
	//, theRearMapReference(NULL)
	, rearTileOffset_x(0)
	, rearTileOffset_y(0)
	, rearMapOffset_x(0)
	, rearMapOffset_y(0)
	, rearMapFineOffset_x(0)
	, rearMapFineOffset_y(0)
{
}

CPlayer::~CPlayer(void)
{
}

// Initialise this class instance
void CPlayer::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(0, 0, 10);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set default tile sizes
	tileSize_Width = 25;
	tileSize_Height = 25;

	this->SetPAABB(Vector3(4, 4, 4), GetPos());

	/*CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Jump", "Image//Mario-jump-sound.mp3");*/
}

// Set the boundary for the player info
void CPlayer::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the tile size
void CPlayer::SetTileSize(const int x, const int y)
{
	tileSize_Width = x;
	tileSize_Height = y;
}

// Set the map pointer to this class
//void CPlayer::SetMap(CMap* m_cMap)
//{
//	theMapReference = m_cMap;
//}

// Returns true if the player is on ground
bool CPlayer::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayer::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayer::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayer::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayer::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 10.0;

		//CSoundEngine::GetInstance()->PlayASound("Jump");
	}
}

// Set position
void CPlayer::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayer::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayer::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set m_dJumpAcceleration of the player
void CPlayer::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayer::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Stop the player's movement
void CPlayer::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayer::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 CPlayer::GetPos(void) const
{
	return pos;
}

// Get target
Vector3 CPlayer::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayer::GetUp(void) const
{
	return up;
}

// Get m_dJumpAcceleration of the player
double CPlayer::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

// Set Tile Offset for x-axis
int CPlayer::GetTileOffset_x(void) const
{
	return tileOffset_x;
}

// Get Max Boundary
Vector3 CPlayer::GetMaxBoundary() const
{
	return maxBoundary;
}
// Get Min Boundary
Vector3 CPlayer::GetMinBoundary() const
{
	return minBoundary;
}

// Update Jump Upwards
//void CPlayer::UpdateJumpUpwards(double dt)
//{
//	if (m_bJumpUpwards == false)
//		return;
//
//	// Record current position before jump
//	int currentPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil((float)position.y / tileSize_Height);
//
//	// Update the jump
//	position.y += m_dJumpSpeed;
//	m_dJumpSpeed -= 1;
//	if (m_dJumpSpeed == 0)
//		SetOnFreeFall(true);
//
//	// If the player has jumped out of the screen, 
//	// then quit this method (do not do collision detection anymore)
//	if (position.y + tileSize_Height > maxBoundary.y)
//		return;
//
//	// Check if the player is stopped by obstacles
//	int checkPosition_X = (int)((mapOffset_x + position.x - (tileSize_Width >> 1)) / tileSize_Width);
//	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
//		(int)ceil((float)(position.y + (tileSize_Height >> 1)) / tileSize_Height);
//	for (int i = checkPosition_Y; i <= currentPosition_Y; i++)
//	{
//		if (((int)(position.x - (tileSize_Width >> 1)) % tileSize_Width) == 0)
//		{
//			if (theMapReference->theScreenMap[i][checkPosition_X] == 1)
//			{
//				// Since the new position does not allow the player to move into, then go back to the old position
//				position.y = (theMapReference->GetNumOfTiles_Height() - i - 1) * tileSize_Height - (tileSize_Height >> 1);
//				// Set on free fall
//				SetOnFreeFall(true);
//				break;
//			}
//		}
//		else
//		{
//			if ((theMapReference->theScreenMap[i][checkPosition_X] == 1) ||
//				(theMapReference->theScreenMap[i][checkPosition_X + 1] == 1))
//			{
//				// Since the new position does not allow the player to move into, then go back to the old position
//				position.y = (theMapReference->GetNumOfTiles_Height() - i - 1) * tileSize_Height - (tileSize_Height >> 1);
//				// Set on free fall
//				SetOnFreeFall(true);
//				break;
//			}
//		}
//	}
//}
//
//// Update FreeFall
//void CPlayer::UpdateFreeFall(double dt)
//{
//	if (m_bFallDownwards == false)
//		return;
//
//	// Record current position before fall
//	int currentPosition_Y = theMapReference->GetNumOfTiles_Height() - (int)ceil((float)position.y / tileSize_Height);
//
//	// Update the free fall
//	position.y -= m_dFallSpeed;
//	m_dFallSpeed += 1;
//
//	// Check if the player is still in mid air...
//	int checkPosition_X = (int)((mapOffset_x + position.x - (tileSize_Width >> 1)) / tileSize_Width);
//	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
//		(int)ceil((float)(position.y - m_dFallSpeed) / tileSize_Height);
//	for (int i = currentPosition_Y; i <= checkPosition_Y; i++)
//	{
//		if (((int)(position.x - (tileSize_Width >> 1)) % tileSize_Width) == 0)
//		{
//			if (theMapReference->theScreenMap[i][checkPosition_X] == 1)
//			{
//				// Since the new position does not allow the player to move into, then go back to the old position
//				position.y = (theMapReference->GetNumOfTiles_Height() - i) * tileSize_Height + (tileSize_Height >> 1);
//				// Stop all vertical movement
//				StopVerticalMovement();
//				break;
//			}
//		}
//		else
//		{
//			if ((theMapReference->theScreenMap[i][checkPosition_X] == 1) ||
//				(theMapReference->theScreenMap[i][checkPosition_X + 1] == 1))
//			{
//				// Since the new position does not allow the player to move into, then go back to the old position
//				position.y = (theMapReference->GetNumOfTiles_Height() - i) * tileSize_Height + (tileSize_Height >> 1);
//				// Stop all vertical movement
//				StopVerticalMovement();
//				break;
//			}
//		}
//	}
//}

/********************************************************************************
Update
********************************************************************************/
void CPlayer::Update(double dt)
{
	float m_speed = 1.0f;

	//m_dElapsedIdleTime += dt;

	// Update the player position
	if (Application::IsKeyPressed('W'))
	{
		pos.y += 30 * dt * m_speed;
	}

	if (Application::IsKeyPressed('S'))
	{
		pos.y -= 30 * dt * m_speed;
	}

	if (Application::IsKeyPressed('D'))
	{
		RL = false;
		//svel.x += 25 * dt * m_speed;
		pos.x += 25 * dt * m_speed;
		SetAnimationStatus(RL,true,dt);
		//m_dElapsedTime += dt;
		std::cout << this->vel.x << std::endl;
	}

	if (Application::IsKeyPressed('A'))
	{
		RL = true;
		pos.x -= 25 * dt * m_speed;
		SetAnimationStatus(RL,true,dt);
		//m_dElapsedTime += dt;
	}

	if(!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D'))
		SetAnimationStatus(RL, false, dt);

	/*if (m_dElapsedTime > 0.3)
	{
		m_dElapsedTime = 0;
		UpdateAnimationIndex(dt);
	}
*/
	/*if (m_dElapsedIdleTime > 0.3)
	{
		m_dElapsedIdleTime = 0;
		UpdateAnimationIndex(dt);
	}*/

	this->SetPAABB(Vector3(4, 4, 4), GetPos());
	// If the user presses SPACEBAR, then make him jump
	//if (Application::IsKeyPressed(VK_SPACE))
	//	SetToJumpUpwards(true);
	//else
	//{
	//	// Check if the player has walked off the platform
	//	if (isOnAir())
	//		this->SetOnFreeFall(true);
	//}

	// Constrain the position
	Constrain();

	// If the user presses R key, then reset the view to default values
	/*if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset();
	}
	else if (KeyboardController::GetInstance()->IsKeyReleased(VK_F5))
	{
		Load();
	}
	else if (KeyboardController::GetInstance()->IsKeyReleased(VK_F6))
	{
		if (Save() == true)
			cout << "Save to file is successful!" << endl;
		else
			cout << "Save to file is unsuccessful!" << endl;
	}
	else
	{
		UpdateJumpUpwards(dt);
		UpdateFreeFall(dt);
		UpdateTileOffset();
	}*/

	// Check for collision with CGoodies instances

	// Calculate the tile indices for the PlayerInfo2D's latest position
	/*checkPosition_X = (int)((mapOffset_x + position.x - (tileSize_Width >> 1)) / tileSize_Width);
	checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
		(int)ceil(position.y / theMapReference->GetTileSize_Height());*/

	//UpdateGoodies(checkPosition_X, checkPosition_Y);
}

// Update tile offsets
//void CPlayer::UpdateTileOffset(void)
//{
//	tileOffset_x = (int)(mapOffset_x / theMapReference->GetTileSize_Width());
//	if (tileOffset_x + theMapReference->GetNumOfTiles_Width() > theMapReference->getNumOfTiles_MapWidth())
//		tileOffset_x = theMapReference->getNumOfTiles_MapWidth() - theMapReference->GetNumOfTiles_Width();
//}
//
//// Update side movements
//void CPlayer::UpdateSideMovements(void)
//{
//	// Calculate the position to check on, using the player's reference point
//	checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
//		(int)ceil(position.y / theMapReference->GetTileSize_Height());
//
//	// Check if the hero can move sideways
//	if (KeyboardController::GetInstance()->IsKeyDown('A'))
//	{
//		// Find the tile number which the player's left side is on
//		checkPosition_X = (int)((mapOffset_x + position.x - (tileSize_Width >> 1)) / tileSize_Width);
//
//		if (checkPosition_X >= 0)
//		{
//			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
//			{
//				position.x = (checkPosition_X + 1 - tileOffset_x) * tileSize_Width -
//					mapFineOffset_x + (tileSize_Width >> 1);
//			}
//		}
//	}
//	else if (KeyboardController::GetInstance()->IsKeyDown('D'))
//	{
//		// Find the tile number which the player's right side is on
//		checkPosition_X = (int)((mapOffset_x + position.x + (tileSize_Width >> 1)) / tileSize_Width);
//
//		if (checkPosition_X < theMapReference->GetNumOfTiles_Width())
//		{
//			if (theMapReference->theScreenMap[checkPosition_Y][checkPosition_X] == 1)
//			{
//				// this part causes the player to be stuck when there is a tile on its right
//				position.x = (checkPosition_X - 1 - tileOffset_x) * tileSize_Width -
//					mapFineOffset_x + (tileSize_Width >> 1);
//			}
//		}
//	}
//}

void CPlayer::UpdateSideMovements(void)
{
}

/********************************************************************************
Move Up Down
********************************************************************************/
void CPlayer::MoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{
		position.y = position.y + (int)(m_dSpeed * timeDiff);
	}
	else
	{
		position.y = position.y - (int)(m_dSpeed * timeDiff);
	}
}

/********************************************************************************
Move Left Right
********************************************************************************/
void CPlayer::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		position.x = position.x - (int)(m_dSpeed * timeDiff);
		//SetAnimationStatus(true);
		//UpdateAnimationIndex();
	}
	else
	{
		position.x = position.x + (int)(m_dSpeed * timeDiff);
		//SetAnimationStatus(false);
		//UpdateAnimationIndex();
	}

	UpdateSideMovements();

}

// Check if the player is standing on air
//bool CPlayer::isOnAir(void)
//{
//	if ((m_bJumpUpwards == true) || (m_bFallDownwards == true))
//		return false;
//
//	// Check if the player is still in mid air...
//	int checkPosition_X = (int)((position.x - (tileSize_Width >> 1)) / tileSize_Width);
//	int checkPosition_Y = theMapReference->GetNumOfTiles_Height() -
//		(int)ceil(position.y / tileSize_Height);
//
//	if (((int)(position.x - (tileSize_Width >> 1)) % tileSize_Width) == 0)
//	{
//		if (theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 0)
//		{
//			return true;
//		}
//	}
//	else
//	{
//		if ((theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X] == 0)
//			&& (theMapReference->theScreenMap[checkPosition_Y + 1][checkPosition_X + 1] == 0))
//		{
//			return true;
//		}
//	}
//
//	return false;
//}

 //Constrain the position within the borders
void CPlayer::Constrain(void)
{
	// Constrain player within the boundary
	/*if (position.x > maxBoundary.x - (tileSize_Width >> 1))
	{
		position.x = maxBoundary.x - (tileSize_Width >> 1);
		mapOffset_x = mapOffset_x + m_dSpeed;
		if (mapOffset_x > maxBoundary.x + tileSize_Width)
			mapOffset_x = maxBoundary.x + tileSize_Width;
	}
	if (position.y > maxBoundary.y - tileSize_Height)
	{
		position.y = maxBoundary.y - (tileSize_Height >> 1);
	}
	if (position.x < minBoundary.x - (tileSize_Width >> 1))
	{
		position.x = minBoundary.x - (tileSize_Width >> 1);
		mapOffset_x = mapOffset_x - m_dSpeed;
		if (mapOffset_x < minBoundary.x)
			mapOffset_x = 0;
	}
	if (position.y < minBoundary.y)
	{
		position.y = minBoundary.y + (tileSize_Height >> 1);
	}*/
}

//// Set mapFineOffset_x values
//void CPlayer::UpdateMapFineOffset(void)
//{
//	mapFineOffset_x = mapOffset_x % theMapReference->GetTileSize_Width();
//}
//
//// Get mapFineOffset_x values
//int CPlayer::GetMapFineOffset_x(void) const
//{
//	return mapFineOffset_x;
//}
//
///*
//For Parallax Scrolling
//*/
//
//// Set the rear map pointer to this class
//void CPlayer::SetRearMap(CMap* m_cRearMap)
//{
//	theRearMapReference = m_cRearMap;
//}
//
//// Update the rear map
//void CPlayer::UpdateRearMap(void)
//{
//	rearMapOffset_x = (int)(mapOffset_x / 2);
//	rearTileOffset_x = (int)(rearMapOffset_x / theRearMapReference->GetTileSize_Width());
//	if (rearTileOffset_x + theRearMapReference->GetNumOfTiles_Width() >
//		theRearMapReference->getNumOfTiles_MapWidth())
//		rearTileOffset_x = theRearMapReference->getNumOfTiles_MapWidth() -
//		theRearMapReference->GetNumOfTiles_Width();
//	rearMapFineOffset_x = rearMapOffset_x % theRearMapReference->GetTileSize_Width();
//}
//
//// Get Tile Offset for x-axis
//int CPlayer::GetRearTileOffset_x(void) const
//{
//	return rearTileOffset_x;
//}
//
//// Get the rear map fine offset X value
//int  CPlayer::GetRearMapFineOffset_x(void) const
//{
//	return rearMapFineOffset_x;
//}
//
//// Methods to tokenize a string into a specific data type variable
//Vector3 CPlayer::Token2Vector(string token)
//{
//	Vector3 tempVector(0.0f, 0.0f, 0.0f);
//
//	std::istringstream ss(token);
//	std::string aToken = "";
//	std::getline(ss, aToken, ',');
//	tempVector.x = Token2Double(aToken);
//	std::getline(ss, aToken, ',');
//	tempVector.y = Token2Double(aToken);
//	std::getline(ss, aToken, ',');
//	tempVector.z = Token2Double(aToken);
//
//	return tempVector;
//}
//double CPlayer::Token2Double(const string token)
//{
//	return atof(token.c_str());
//}
//bool CPlayer::Token2Bool(const string token)
//{
//	return token.at(0) == '1';
//}
//
//
//// Load this class
//bool CPlayer::Load(const string saveFileName)
//{
//	ifstream myfile(saveFileName.c_str(), ios::in);
//	if (myfile.is_open())
//	{
//		string line;
//		while (getline(myfile, line))
//		{
//			cout << line << '\n';
//
//			std::istringstream ss(line);
//			std::string aToken = "";
//
//			// Get the tag from the line
//			while (std::getline(ss, aToken, '=')) {
//				std::cout << aToken << endl;
//
//				// Get the data from the line
//				std::string theTag = aToken;
//				std::getline(ss, aToken, '=');
//				std::cout << aToken << endl;
//				if (theTag == "defaultPosition")
//				{
//					defaultPosition = Token2Vector(aToken);
//				}
//				else if (theTag == "defaultTarget")
//				{
//					defaultTarget = Token2Vector(aToken);
//				}
//				else if (theTag == "defaultUp")
//				{
//					defaultUp = Token2Vector(aToken);
//				}
//				else if (theTag == "position")
//				{
//					position = Token2Vector(aToken);
//				}
//				else if (theTag == "target")
//				{
//					target = Token2Vector(aToken);
//				}
//				else if (theTag == "up")
//				{
//					up = Token2Vector(aToken);
//				}
//				else if (theTag == "maxBoundary")
//				{
//					maxBoundary = Token2Vector(aToken);
//				}
//				else if (theTag == "minBoundary")
//				{
//					minBoundary = Token2Vector(aToken);
//				}
//				else if (theTag == "m_dSpeed")
//				{
//					m_dSpeed = Token2Double(aToken);
//				}
//				else if (theTag == "m_dAcceleration")
//				{
//					m_dAcceleration = Token2Double(aToken);
//				}
//				else if (theTag == "m_dJumpSpeed")
//				{
//					m_dJumpSpeed = Token2Double(aToken);
//				}
//				else if (theTag == "m_dJumpAcceleration")
//				{
//					m_dJumpAcceleration = Token2Double(aToken);
//				}
//				else if (theTag == "m_dFallSpeed")
//				{
//					m_dFallSpeed = Token2Double(aToken);
//				}
//				else if (theTag == "m_dFallAcceleration")
//				{
//					m_dFallAcceleration = Token2Double(aToken);
//				}
//				else if (theTag == "m_dElapsedTime")
//				{
//					m_dElapsedTime = Token2Double(aToken);
//				}
//				else if (theTag == "m_bJumpUpwards")
//				{
//					m_bJumpUpwards = Token2Bool(aToken);
//				}
//				else if (theTag == "m_bFallDownwards")
//				{
//					m_bFallDownwards = Token2Bool(aToken);
//				}
//				else if (theTag == "m_dFallAcceleration")
//				{
//					m_dFallAcceleration = Token2Bool(aToken);
//				}
//				else if (theTag == "m_dElapsedTime")
//				{
//					m_dElapsedTime = Token2Bool(aToken);
//				}
//				else if (theTag == "tileOffset_x")
//				{
//					tileOffset_x = Token2Bool(aToken);
//				}
//				else if (theTag == "tileOffset_y")
//				{
//					tileOffset_y = Token2Bool(aToken);
//				}
//				else if (theTag == "mapFineOffset_x")
//				{
//					mapFineOffset_x = Token2Bool(aToken);
//				}
//				else if (theTag == "mapFineOffset_y")
//				{
//					mapFineOffset_y = Token2Bool(aToken);
//				}
//				else if (theTag == "rearTileOffset_x")
//				{
//					rearTileOffset_x = Token2Bool(aToken);
//				}
//				else if (theTag == "rearTileOffset_y")
//				{
//					rearTileOffset_y = Token2Bool(aToken);
//				}
//				else if (theTag == "rearMapOffset_x")
//				{
//					rearMapOffset_x = Token2Bool(aToken);
//				}
//				else if (theTag == "rearMapOffset_y")
//				{
//					rearMapOffset_y = Token2Bool(aToken);
//				}
//				else if (theTag == "rearMapFineOffset_x")
//				{
//					rearMapFineOffset_x = Token2Bool(aToken);
//				}
//				else if (theTag == "rearMapFineOffset_y")
//				{
//					rearMapFineOffset_y = Token2Bool(aToken);
//				}
//			}
//		}
//		myfile.close();
//	}
//	else
//	{
//#if(_DEBUG == TRUE)
//		cout << "PlayerInfo: Unable to load " << saveFileName.c_str() << endl;
//#endif
//		myfile.close();
//		return false;
//	}
//
//	return true;
//}
//
//// Save this class
//bool CPlayer::Save(const string saveFileName)
//{
//	ofstream myfile;
//	myfile.open(saveFileName.c_str(), ios::out | ios::ate);
//	if (myfile.is_open())
//	{
//		myfile << "defaultPosition=" << defaultPosition.x << "," << defaultPosition.y << ","
//			<< defaultPosition.z << endl;
//		myfile << "defaultTarget=" << defaultTarget.x << "," << defaultTarget.y << ","
//			<< defaultTarget.z << endl;
//		myfile << "defaultUp=" << defaultUp.x << "," << defaultUp.y << "," << defaultUp.z << endl;
//		myfile << "position=" << position.x << "," << position.y << "," << position.z << endl;
//		myfile << "target=" << target.x << "," << target.y << "," << target.z << endl;
//		myfile << "up=" << up.x << "," << up.y << "," << up.z << endl;
//		myfile << "maxBoundary=" << maxBoundary.x << "," << maxBoundary.y << "," << maxBoundary.z << endl;
//		myfile << "minBoundary=" << minBoundary.x << "," << minBoundary.y << "," << minBoundary.z << endl;
//		myfile << "tileSize_Width=" << tileSize_Width << endl;
//		myfile << "tileSize_Height=" << tileSize_Height << endl;
//		myfile << "m_dSpeed=" << m_dSpeed << endl;
//		myfile << "m_dAcceleration=" << m_dAcceleration << endl;
//		myfile << "m_bJumpUpwards=" << m_bJumpUpwards << endl;
//		myfile << "m_dJumpSpeed=" << m_dJumpSpeed << endl;
//		myfile << "m_dJumpAcceleration=" << m_dJumpAcceleration << endl;
//		myfile << "m_dFallSpeed=" << m_dFallSpeed << endl;
//		myfile << "m_bFallDownwards=" << m_bFallDownwards << endl;
//		myfile << "m_dFallAcceleration=" << m_dFallAcceleration << endl;
//		myfile << "m_dElapsedTime=" << m_dElapsedTime << endl;
//		myfile << "tileOffset_x=" << tileOffset_x << endl;
//		myfile << "tileOffset_y=" << tileOffset_y << endl;
//		myfile << "mapFineOffset_x=" << mapFineOffset_x << endl;
//		myfile << "mapFineOffset_y=" << mapFineOffset_y << endl;
//		myfile << "rearTileOffset_x=" << rearTileOffset_x << endl;
//		myfile << "rearTileOffset_y=" << rearTileOffset_y << endl;
//		myfile << "rearMapOffset_x=" << rearMapOffset_x << endl;
//		myfile << "rearMapOffset_y=" << rearMapOffset_y << endl;
//		myfile << "rearMapFineOffset_x=" << rearMapFineOffset_x << endl;
//		myfile << "rearMapFineOffset_y=" << rearMapFineOffset_y << endl;
//
//		myfile.close();
//		return true;
//	}
//	else
//	{
//#if(_DEBUG == TRUE)
//		cout << "PlayerInfo: Unable to save " << saveFileName.c_str() << endl;
//#endif
//		myfile.close();
//		return false;
//	}
//}
//
//// Check for collision with CGoodies instances
//void CPlayer::UpdateGoodies(const int tileIndex_Column, const int tileIndex_Row)
//{
//	CGoodies* theGoodie = CGoodiesFactory::GetInstance()->GetGoodies(tileIndex_Column, tileIndex_Row);
//	if (theGoodie)
//	{
//		// Perform any special operation
//
//		// Remove the goodie from the factory
//		if (CGoodiesFactory::GetInstance()->RemoveGoodies(tileIndex_Column, tileIndex_Row) == true)
//		{
//			// Remove the instance from the map
//			theMapReference->theScreenMap[tileIndex_Row][tileIndex_Column] = 0;
//		}
//	}
//}

