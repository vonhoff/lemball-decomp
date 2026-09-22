#ifndef LEMBALL_VISOS_GRAPHICS_CPVANIMWND_H
#define LEMBALL_VISOS_GRAPHICS_CPVANIMWND_H

#include "../Foundation/CString.h" // complete type
#include "CGWnd.h"                 // complete type

// SIZE 0xa0
// VTABLE: LEMBALL 0x00499e68
class CPVAnimWnd : public CGWnd {
public:
	CPVAnimWnd()
	{
		m_resolveMoviePath = 0;
		m_useMoviePrefix = 0;
	}

	virtual unsigned int GetStyle(); // vtable+0x64

	friend class CAnimWnd;
	friend class CIntroAnimAnimWindow;
	friend class CIntroAnimDrawer;
	friend class CSuccFailDrawer;

private:
	unsigned int m_useMoviePrefix;   // 0x90
	unsigned int m_resolveMoviePath; // 0x94
	CString m_moviePrefix;           // 0x98
};

#endif
