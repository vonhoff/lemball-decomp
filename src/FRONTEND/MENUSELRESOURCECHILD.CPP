#include "ENGINE/GDI/VSGEOM.H"

class GameResourceGeometryChildView {
public:
	void RefreshGeometryChildEnabled(void);
};

class GameResourceGeometryChildState : public GameGeometryOwnerRectState {
public:
	unsigned char m_abReserved38[0x10];
	int m_fRefreshPending48;
};

class MenuSelectorXorFieldNoArgView {
public:
	void RecomputeField104FromXor(void);
};

// FUNCTION: LEMBALL 0x0043a5e0
void GameResourceGeometryChildView::RefreshGeometryChildEnabled(void)
{
	char* pObject;
	int nRenderContext;

	pObject = (char*) this;
	if (*(int*) (pObject + 0x20) != 0) {
		nRenderContext = *(int*) (*(int*) (pObject + 0x4c) + 0x0c);
		*(int*) (nRenderContext + *(int*) (*(int*) (nRenderContext + 0x40) + 4) + 0x78) = 1;
	}
	((GameResourceGeometryChildState*) (pObject + 0x90))->m_fRefreshPending48 = 1;
	((GameResourceGeometryChildState*) (pObject + 0x90))->SetEnabled(*(int*) (pObject + 0x34));
}

// FUNCTION: LEMBALL 0x00443890
void MenuSelectorXorFieldNoArgView::RecomputeField104FromXor(void)
{
	int nValue;

	nValue = *(volatile int*) ((char*) this + 0xd0);
	nValue ^= *(volatile int*) ((char*) this + 0x130);
	*(int*) ((char*) this + 0x104) = nValue;
}
