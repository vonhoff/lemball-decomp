#include "ENGINE/GDI/VSGDI.H"

struct VsGdiDibRectDispatch240 {
	virtual void Reserved0(void) = 0;
	virtual void Reserved1(void) = 0;
	virtual void Reserved2(void) = 0;
	virtual void Reserved3(void) = 0;
	virtual void Reserved4(void) = 0;
	virtual void Reserved5(void) = 0;
	virtual void Reserved6(void) = 0;
	virtual void Reserved7(void) = 0;
	virtual int CopyDisplayRect(void* pDestinationBinding,
								short* pDestinationRect,
								void* pSourceBinding,
								short* pSourcePoint) = 0;
};

class VsGdiDib240DisplayState : public VsGdiMetricsDisplayState {
public:
	int StretchDisplayRect(void* pDestinationBinding,
						   short* pDestinationRect,
						   void* pSourceBinding,
						   short* pSourceRect);
};

// FUNCTION: LEMBALL 0x004572c0
int VsGdiDib240DisplayState::StretchDisplayRect(void* pDestinationBinding,
												short* pDestinationRect,
												void* pSourceBinding,
												short* pSourceRect)
{
	short aSourcePoint[2];
	short* pAdjustedSourceRect;

	pAdjustedSourceRect = pSourceRect != 0 ? pSourceRect + 2 : 0;
	aSourcePoint[0] = pAdjustedSourceRect[0];
	aSourcePoint[1] = pAdjustedSourceRect[1];
	return ((VsGdiDibRectDispatch240*) this)
		->CopyDisplayRect(pDestinationBinding, pDestinationRect, pSourceBinding, aSourcePoint);
}
