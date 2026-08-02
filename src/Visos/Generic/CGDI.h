#ifndef LEMBALL_VISOS_GENERIC_CGDI_H
#define LEMBALL_VISOS_GENERIC_CGDI_H

class CPrimitive;
class CSurface;
class CVSRect;

// Recovered from CODE_02 `Visos (Generic)`. Windows storage remains unknown.
class CGDI {
public:
	CGDI(const CVSRect& rect, int nFlags, CSurface* pSurface);
	~CGDI(void);
	void AddToList(CPrimitive* pPrimitive);
	void Render(void);
};

#endif
