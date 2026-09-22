#ifndef LEMBALL_AI_OBJECTS_CVIEWDATA_H
#define LEMBALL_AI_OBJECTS_CVIEWDATA_H

#include "../../Views/Sound/SoundEffects.h"
#include "../Base/AiCoord.h" // complete type
#include "../Base/ObjectActions.h"
#include "../Base/ObjectTypes.h"

// SIZE 0x4c
class CViewData {
public:
	void SetViewActionTuple(eAction p_action, unsigned int p_argument, unsigned int p_stateTimer);

	// FUNCTION: LEMBALL 0x00439340
	CViewData()
	{
		m_objectId = 0xffff;
		m_transientFlags = 0;
		m_unk0x30 = 0;
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
	int m_positionX;               // 0x04
	int m_positionY;               // 0x08
	int m_positionZ;               // 0x0c
	short m_gameX;                 // 0x10
	short m_gameY;                 // 0x12
	unsigned int m_statusFlags;    // 0x14
	eAction m_action;              // 0x18
	short m_actionArgument;        // 0x1c
	unsigned int m_stateTimer;     // 0x20
	unsigned int m_animationTime;  // 0x24
	eObjectType m_objectType;      // 0x28
	unsigned short m_objectId;     // 0x2c
	unsigned short m_playerIndex;  // 0x2e
	unsigned int m_unk0x30;        // 0x30
	unsigned int m_transientFlags; // 0x34
	AiCoord m_auxiliaryPosition;   // 0x38
	eSoundEffect m_soundEffect;    // 0x44
	unsigned int m_sortZKey;       // 0x48
};

#endif
