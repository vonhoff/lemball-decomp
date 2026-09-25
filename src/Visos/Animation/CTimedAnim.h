#ifndef LEMBALL_VISOS_ANIMATION_CTIMEDANIM_H
#define LEMBALL_VISOS_ANIMATION_CTIMEDANIM_H

#include "CFrames.h"

// SIZE 0x1c
// VTABLE: LEMBALL 0x00496d08
class CTimedAnim : public CFrames {
public:
	CTimedAnim(unsigned int p_frames = 0, int p_direction = 0) : CFrames(p_frames) { m_direction = p_direction; }
	virtual unsigned int GetFrameNo();                    // vtable+0x00
	virtual void StartAnim(unsigned long p_animTime) = 0; // vtable+0x04
	virtual void SetAnimTime(unsigned long p_animTime);   // vtable+0x08
	virtual void SetStartTime(unsigned long p_startTime); // vtable+0x0c
	virtual void SetAnimDirection(int p_direction);       // vtable+0x10

	friend class CBaseFrontendDrawer;
	friend class CCDLoadAnim;
	friend class CGunController;
	friend class CPlayThruAnim;
	friend class CRepeatAnim;
	friend class CPreviewDrawer;
	friend class CLemmingAnimsManager;

private:
	unsigned int m_animTime;  // 0x10
	unsigned int m_fixedTime; // 0x14
	int m_direction;          // 0x18
};

#endif
