#ifndef LEMBALL_VISOS_GRAPHICS_CDRAWER_H
#define LEMBALL_VISOS_GRAPHICS_CDRAWER_H

class CVsRect;

// SIZE 0x04
// VTABLE: LEMBALL 0x00496d48
class CDrawer {
public:
	virtual ~CDrawer() {}                         // vtable+0x00
	virtual void ShutDown();                      // vtable+0x04
	virtual void Draw(const CVsRect& p_rect) = 0; // vtable+0x08
	virtual void OnZoom(const CVsRect& p_rect);   // vtable+0x0c
	virtual void OnSize(const CVsRect& p_rect);   // vtable+0x10
	virtual void OnMove(const CVsRect& p_rect);   // vtable+0x14
	virtual void DestroyDrawer();                 // vtable+0x18
	virtual void Process();                       // vtable+0x1c
	virtual void ResetPrimitives() = 0;           // vtable+0x20
	virtual void Initialise();                    // vtable+0x24
	virtual int GetReturnState() = 0;             // vtable+0x28
	virtual bool QuitYet() = 0;                   // vtable+0x2c
	virtual void OnDriverChange();                // vtable+0x30
	virtual void RefreshView();                   // vtable+0x34
};

// SYNTHETIC: LEMBALL 0x00439400
// CDrawer::`scalar deleting destructor'

#endif
