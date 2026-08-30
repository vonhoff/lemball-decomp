#ifndef LEMBALL_VISOS_GRAPHICS_GDI_H
#define LEMBALL_VISOS_GRAPHICS_GDI_H

#include "../../Common.h"

struct TargetWinGDrawCodecState;

// SIZE 0x10
class Gdi {
public:
	Gdi(const VsRect& p_arg0, int p_arg1, Surface* p_arg2);
	void AddToList(Primitive* p_primitive);
	void Render();
	~Gdi();

	friend class Bitmap;
	friend class ClipRect;
	friend class CopyColourToBackBuff;
	friend class CopyToBackBuff;
	friend class DrawingMark;
	friend class Line;
	friend class PopActive;
	friend class PushActive;
	friend class SolidRect;
	friend class Text;
	friend class Zrle;
	friend class Main2DDisplay;
	friend class GWnd;
	friend class BaseFrontendDrawer;
	friend class AnimsManager;
	friend class GraphicButton;
	friend class GunButtons;
	friend class DepressedButton;
	friend class PvButton;
	friend class GunController;
	friend class BitmapRes;
	friend class BitmapResBase;
	friend class CdLoadAnimDraw;
	friend class BaseCursor;
	friend class Anim;
	friend class SpriteWindow;
	friend class TrackWindow;
	friend class HiliteWindow;
	friend class TargetAboutScreen;
	friend class HiliteButtons;
	friend class PasswordHiliteWindow;
	friend class HiliteController;
	friend class PasswordDrawer;
	friend struct TargetWinGDrawCodecState;
	friend TargetWinGDrawCodecState* TargetWinGDrawCodec_Open(void* p_openInfo);

private:
	Primitive** m_primitives; // 0x00
	int m_primitiveCount;     // 0x04
	int m_primitiveCapacity;  // 0x08
	Surface* m_renderTarget;  // 0x0c
};

extern Primitive* g_pCurrentPrimitive;
#endif
