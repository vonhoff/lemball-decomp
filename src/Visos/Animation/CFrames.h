#ifndef LEMBALL_VISOS_ANIMATION_CFRAMES_H
#define LEMBALL_VISOS_ANIMATION_CFRAMES_H

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d04
class CFrames {
public:
	CFrames(unsigned int p_frames = 0) { m_frames = p_frames; }
	virtual unsigned int GetFrameNo(); // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CGunController;
	friend class CAnimsManager;
	friend class CCDLoadAnim;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CLemmingAnimsManager;

protected:
	unsigned int m_frames;     // 0x04
	unsigned int m_reserved08; // 0x08
	unsigned int m_frameState; // 0x0c
};

#endif
