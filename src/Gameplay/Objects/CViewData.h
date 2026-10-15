#ifndef LEMBALL_AI_OBJECTS_CVIEWDATA_H
#define LEMBALL_AI_OBJECTS_CVIEWDATA_H

#include "Application/SoundEffects.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Geometry/CPt3.h"
#include "ObjectActions.h"
#include "ObjectIds.h"
#include "ObjectTypes.h"

enum eLemmingViewStatusFlag {
	LEMMING_VIEW_STATUS_GROUP_LEADER = 0x01,
	LEMMING_VIEW_STATUS_IN_GROUP = 0x02
};

enum eRemotePaletteRemap {
	REMOTE_PALETTE_REMAP_DISABLED = 0,
	REMOTE_PALETTE_REMAP_ENABLED = 1
};

// SIZE 0x4c
class CViewData {
public:
	CPt3 Coord() { return m_position; }

	void SetViewActionTuple(eAction p_action, unsigned int p_argument, unsigned long p_stateTimer);

	// FUNCTION: LEMBALL 0x00439340
	CViewData()
	{
		m_objectId = INVALID_OBJECT_REGISTRY_INDEX;
		m_transientFlags = 0;
		m_hidden = false;
		m_action = ACTION_NONE;
		m_stateTimer = 0;
	}

	friend class CGameObject;
	friend class CSwitch;
	friend class CRocket;
	friend class CPlayerLemming;
	friend class CTrapDoor;
	friend class CLaser;
	friend class CSoundView;
	friend class C2D;
	friend int ViewDataCmp(const void* p_left, const void* p_right);

private:
	short m_facingDirection;       // 0x00
	CPt3 m_position;               // 0x04
	short m_gameX;                 // 0x10
	short m_gameY;                 // 0x12
	unsigned int m_statusFlags;    // 0x14
	eAction m_action;              // 0x18
	short m_actionArgument;        // 0x1c
	unsigned long m_stateTimer;    // 0x20
	unsigned long m_animationTime; // 0x24
	eObjectType m_objectType;      // 0x28
	unsigned short m_objectId;     // 0x2c
	unsigned short m_playerIndex;  // 0x2e
	bool m_hidden;                 // 0x30
	unsigned int m_transientFlags; // 0x34
	C3DVector m_momentum;          // 0x38
	eSoundEffect m_soundEffect;    // 0x44
	unsigned int m_sortZKey;       // 0x48
};

int ViewDataCmp(const void* p_left, const void* p_right);

#endif
