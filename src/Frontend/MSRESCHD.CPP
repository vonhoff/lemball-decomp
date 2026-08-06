#include "Platform/Windows/Mixed/Engine/GDI/VSGEOM.H"

class GameResourceGeometryChildState : public GameGeometryOwnerRectState {
public:
	unsigned char m_abReserved38[0x10];
	int m_fRefreshPending48;
};

class GameResourceGeometryChildView {
public:
	void RefreshGeometryChildEnabled(void);

private:
	unsigned char m_abReserved00[0x20];
	int m_fRenderObject20;
	unsigned char m_abReserved24[0x10];
	int m_fChildEnabled34;
	unsigned char m_abReserved38[0x14];
	void* m_pRenderOwner4C;
	unsigned char m_abReserved50[0x40];
	GameResourceGeometryChildState m_ChildState90;
};

class MenuSelectorXorFieldNoArgView {
public:
	unsigned char m_abData[0xd0];
	int m_nXorFieldD0;                     // 0xd0  (XOR operand 1)
	unsigned char m_abDataD4[0x30];
	int m_nComputedField104;               // 0x104 (result)
	unsigned char m_abData108[0x28];
	int m_nXorField130;                    // 0x130 (XOR operand 2)

	void RecomputeField104FromXor(void);
};

// FUNCTION: LEMBALL 0x0043a5e0
void GameResourceGeometryChildView::RefreshGeometryChildEnabled(void)
{
	int nRenderContext;

	if (m_fRenderObject20 != 0) {
		nRenderContext = *(int*) ((char*) m_pRenderOwner4C + 0x0c);
		*(int*) (nRenderContext + *(int*) (*(int*) (nRenderContext + 0x40) + 4) + 0x78) = 1;
	}
	m_ChildState90.m_fRefreshPending48 = 1;
	m_ChildState90.SetEnabled(m_fChildEnabled34);
}

// FUNCTION: LEMBALL 0x00443890
void MenuSelectorXorFieldNoArgView::RecomputeField104FromXor(void)
{
	int nValue;

	nValue = *(volatile int*) &m_nXorFieldD0;
	nValue ^= *(volatile int*) &m_nXorField130;
	m_nComputedField104 = nValue;
}

// FUNCTION: LEMBALL 0x00443930
void noop_entity_button_state_update(void)
{
}

// FUNCTION: LEMBALL 0x00443940
void noop_entity_button_state_recalc(void)
{
}
