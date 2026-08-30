#ifndef LEMBALL_VISOS_GRAPHICS_ANIMWND_H
#define LEMBALL_VISOS_GRAPHICS_ANIMWND_H

#include "../../Common.h"
#include "../Foundation/String.h" // complete type
#include "PvAnimWnd.h"            // complete type

// SIZE 0xbc
// VTABLE: LEMBALL 0x00499f30
class AnimWnd : public PvAnimWnd {
public:
	AnimWnd();
	virtual void OnCreate();             // vtable+0x18
	virtual void OnDestroy();            // vtable+0x1c
	virtual void OnFrame(int p_frame);   // vtable+0xbc
	virtual void OnSkip(int p_position); // vtable+0xb8
	virtual void OnStart();              // vtable+0xc0
	virtual void OnStop();               // vtable+0xc4
	virtual int ProcessOtherMessages(unsigned int p_message,
									 unsigned int p_wParam,
									 unsigned int p_lParam); // vtable+0xa0
	virtual void Refresh(VsRect* p_rect);                    // vtable+0x70
	void Initialise();
	void OnNotifyError(int p_error);
	void OnNotifyMode(int p_mode);
	void OnNotifyPos(int p_position, int p_flags);
	void OnNotifySize(int p_width, int p_height);
	void Play();
	void Resume();
	void SetAnim(unsigned int p_resourceId);
	void SetMovieWindow();
	void Stop();
	~AnimWnd();

	friend class IntroAnimDrawer;
	friend class IntroAnimAnimWindow;
	friend class SuccFailDrawer;

private:
	unsigned int m_animSet;        // 0xa0
	unsigned int m_playing;        // 0xa4
	unsigned int m_paused;         // 0xa8
	unsigned int m_animResourceId; // 0xac
	String m_moviePath;            // 0xb0
	void* m_movieWindow;           // 0xb8
};

#endif
