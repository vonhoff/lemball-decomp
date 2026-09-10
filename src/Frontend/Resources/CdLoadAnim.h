#ifndef LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H
#define LEMBALL_FRONTEND_RESOURCES_CDLOADANIM_H

#include "../../Common.h"
#include "../../Visos/Animation/AnimsManager.h" // complete type
#include "../../Visos/Animation/RepeatAnim.h"   // complete type
#include "../../Visos/Graphics/Bitmap.h"        // complete type
#include "../../Visos/Graphics/BitmapRes.h"     // complete type
#include "../../Visos/Graphics/DrawingMark.h"   // complete type
#include "../../Visos/Graphics/Line.h"          // complete type
#include "CdLoadAnimDraw.h"
#include "CdLoadAnimProgress.h"

// SIZE 0x134
// VTABLE: LEMBALL 0x00497c90
class CdLoadAnim : public AnimsManager {
public:
	CdLoadAnim(Gdi* p_arg0, Main2DDisplay* p_arg1);
	void InitialiseScreen();
	~CdLoadAnim();

	friend class FrontendResourceLoader;

	CdLoadAnimProgress m_progress; // 0x70
};

extern unsigned int g_dwCdLoadAnimCompactPoints[5];
extern unsigned int g_dwCdLoadAnimFullPoints[5];

#endif
