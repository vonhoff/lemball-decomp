#ifndef LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H
#define LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h" // complete type
#include "../../Visos/Graphics/ClipRect.h"      // complete type
#include "../../Visos/Graphics/Line.h"          // complete type

// SIZE 0x134
// VTABLE: LEMBALL 0x00497c90 AnimsManager base
// VTABLE: LEMBALL 0x00497c8c progress subobject at +0x70
// VTABLE: LEMBALL 0x00497c88 draw subobject at +0x74
class CdLoadAnim : public AnimsManager {
public:
	CdLoadAnim(Gdi* p_arg0, Main2DDisplay* p_arg1);
	void Draw();
	void Draw(short p_progress);
	void InitialiseScreen();
	~CdLoadAnim();

private:
	void* m_progressVtable;             // 0x70
	void* m_drawVtable;                 // 0x74
	VsPoint* m_points;                  // 0x78
	Main2DDisplay* m_display;           // 0x7c
	Gdi* m_gdi;                         // 0x80
	unsigned int m_animResourceId;      // 0x84
	short m_centerX;                    // 0x88
	short m_centerY;                    // 0x8a
	unsigned int m_initialDraw;         // 0x8c
	short m_progress;                   // 0x90
	ResBitmap* m_backgroundBitmap;      // 0x94
	ResBitmap* m_foregroundBitmap;      // 0x98
	unsigned char m_primitive9c[0x24];  // 0x9c
	unsigned char m_animFramec0[0x1c];  // 0xc0
	RepeatAnim* m_repeatAnim;           // 0xdc
	Line m_line;                        // 0xe0
	ClipRect m_clipRects[3];            // 0xf0
	unsigned char m_primitive120[0x14]; // 0x120
};

#endif
