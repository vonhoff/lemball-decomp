#ifndef LEMBALL_VISOS_GENERIC_CGDI_H
#define LEMBALL_VISOS_GENERIC_CGDI_H

class CPrimitive;
class CSurface;
class CVSRect;

// Recovered from CODE_02 `Visos (Generic)`. The authoritative 0x10-byte
// Windows render-queue layout remains in Visos/Windows/CGDI.cpp.
class CGDI {
public:
	CGDI(const CVSRect& rect, int nFlags, CSurface* pSurface);
	~CGDI(void);
	void AddToList(CPrimitive* pPrimitive);
	void Render(void);
};

#endif
