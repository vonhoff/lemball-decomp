#ifndef LEMBALL_VISOS_GRAPHICS_PVANIMWND_H
#define LEMBALL_VISOS_GRAPHICS_PVANIMWND_H

#include "../../Common.h"
#include "../Foundation/String.h" // complete type
#include "GWnd.h"                 // complete type

// SIZE 0xa0
// VTABLE: LEMBALL 0x00499e68
class PvAnimWnd : public GWnd {
public:
	virtual unsigned int GetStyle(); // vtable+0x64

	friend class AnimWnd;
	friend class IntroAnimAnimWindow;
	friend class IntroAnimDrawer;
	friend class SuccFailDrawer;

private:
	unsigned int m_useMoviePrefix;   // 0x90
	unsigned int m_resolveMoviePath; // 0x94
	String m_moviePrefix;            // 0x98
};

#endif
