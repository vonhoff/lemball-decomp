#include "Ice.h"

#include "../../Control/Game/Game.h"

// 68K 0x1061176c __ct__4CIceFv
// STUB: LEMBALL 0x0042ca70
Ice::Ice()
{
}

// 68K 0x106117aa Restart__4CIceFv
// FUNCTION: LEMBALL 0x0042ca90
void Ice::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x106117da Initialise__4CIceFv
// FUNCTION: LEMBALL 0x0042cab0
void Ice::Initialise()
{
	m_stateTimer = 0;
	m_switched = 0;
	m_enabled = 0;
	m_objectCount = 0;
	m_action = (eAction) 0x18;
	m_lastMovementTick = g_dwGameTick;
}

// 68K 0x10611870 Set__4CIceFUsRC8tCoord3dRC8tCoord3diiUc
// STUB: LEMBALL 0x0042cb00
void Ice::Set(unsigned short p_id,
			  const Coord3d& p_cornerA,
			  const Coord3d& p_cornerB,
			  int p_velocityX,
			  int p_velocityY,
			  unsigned char p_initialSwitched)
{
}

// 68K 0x10611a44 Process__4CIceFv
// STUB: LEMBALL 0x0042cd70
bool Ice::Process()
{
	return 0;
}

// 68K 0x1061200c StepOn__4CIceFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x0042d380
bool Ice::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	return 0;
}

// 68K 0x1061217a Leave__4CIceFP14CPlayerLemming
// STUB: LEMBALL 0x0042d4d0
void Ice::Leave(PlayerLemming* p_lemming)
{
}

// 68K 0x1061220e Switch__4CIceFv
// FUNCTION: LEMBALL 0x0042d550
void Ice::Switch()
{
	RequestAction(0x1a);
}

// 68K 0x10612236 Switched__4CIceFv
// STUB: LEMBALL 0x0042d560
void Ice::Switched()
{
}

// 68K 0x1061181a __dt__4CIceFv
Ice::~Ice()
{
}
