#ifndef LEMBALL_VISOS_ANIMATION_CANIMFRAMEBASE_H
#define LEMBALL_VISOS_ANIMATION_CANIMFRAMEBASE_H

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d00
class CAnimFrameBASE {
public:
	CAnimFrameBASE(unsigned int p_frames = 0) { m_frames = p_frames; }
	virtual unsigned int GetFrameNo() = 0;

	friend class CBaseFrontendDrawer;
	friend class CGunController;
	friend class CAnimsManager;
	friend class CCDLoadAnim;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CLemmingAnimsManager;
	friend class CPlayThruAnim;
	friend class CRepeatAnim;
	friend class CPreviewDrawer;

protected:
	unsigned int m_frames;     // 0x04
	unsigned int m_reserved08; // 0x08
	unsigned int m_frameState; // 0x0c
};

#endif
