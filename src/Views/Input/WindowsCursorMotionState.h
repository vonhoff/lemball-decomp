#ifndef LEMBALL_VIEWS_INPUT_WINDOWSCURSORMOTIONSTATE_H
#define LEMBALL_VIEWS_INPUT_WINDOWSCURSORMOTIONSTATE_H

#include "../../Common.h"
#include "../../Visos/Graphics/ClipRect.h"  // complete type
#include "../../Visos/Graphics/SolidRect.h" // complete type

// SIZE 0x78
class WindowsCursorMotionState {
private:
	undefined4 m_unknown00;          // 0x00
	Map* m_map;                      // 0x04
	Ai* m_ai;                        // 0x08
	BaseQueue* m_aiQueue;            // 0x0c
	LemmingAnimsManager* m_anims;    // 0x10
	ClipRect m_clipRects[2];         // 0x14
	int m_fixedX;                    // 0x34
	int m_fixedY;                    // 0x38
	int m_velocityX;                 // 0x3c
	int m_velocityY;                 // 0x40
	int m_accelerationX;             // 0x44
	int m_accelerationY;             // 0x48
	unsigned int m_lastTickX;        // 0x4c
	unsigned int m_lastTickY;        // 0x50
	unsigned int m_horizontalActive; // 0x54
	unsigned int m_verticalActive;   // 0x58
	unsigned int m_positionDirty;    // 0x5c
	SolidRect m_marker;              // 0x60
	unsigned int m_aux0;             // 0x70
	unsigned int m_aux1;             // 0x74
};

#endif
