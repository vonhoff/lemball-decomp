#ifndef LEMBALL_AI_OBJECTS_VIEWDATA_H
#define LEMBALL_AI_OBJECTS_VIEWDATA_H

#include "../../Common.h"
#include "../Base/C3DVector.h" // complete type

// SIZE 0x4c
class ViewData {
public:
	ViewData();

	friend class PlayerLemming;
	friend class SoundView;
	friend int ViewDataCmp(const void* p_left, const void* p_right);

private:
	short m_facingDirection;       // 0x00
	int m_positionX;               // 0x04
	int m_positionY;               // 0x08
	int m_positionZ;               // 0x0c
	short m_viewX;                 // 0x10
	short m_viewY;                 // 0x12
	unsigned int m_statusFlags;    // 0x14
	eAction m_action;              // 0x18
	short m_actionArgument;        // 0x1c
	unsigned int m_stateTimer;     // 0x20
	unsigned int m_animationTime;  // 0x24
	eObjectType m_objectType;      // 0x28
	unsigned short m_objectId;     // 0x2c
	unsigned short m_playerIndex;  // 0x2e
	undefined4 m_unk0x30;          // 0x30
	unsigned int m_transientFlags; // 0x34
	C3DVector m_auxiliaryPosition; // 0x38
	eSoundEffect m_soundEffect;    // 0x44
	unsigned int m_sortZKey;       // 0x48
};

#endif
