#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelStatusIndicatorInterfaceView {
	virtual void Dispatch(void);
};

struct LevelStatusIndicatorChildView {
	unsigned char m_abReserved00[0x74];
	LevelStatusIndicatorInterfaceView m_Interface74;
};

struct LevelStatusIndicatorManagerView {
	unsigned char m_abReserved00[0x78];
	LevelStatusIndicatorChildView* m_pChild78;

	void DispatchChild(void);
};

// FUNCTION: LEMBALL 0x00433fb0
void LevelStatusIndicatorManagerView::DispatchChild(void)
{
	m_pChild78->m_Interface74.Dispatch();
}
