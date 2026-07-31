#include "ENGINE/CORE/COMMON.H"

struct VsQueuedRenderPointSinkView {
	char m_abReserved00[0x30];
	int m_nSink30;

	void SetSink(int nSink);
};

struct VsWindowOwnerRuntimeStateView {
	char m_abReserved00[0x3c];
	int m_nOwnerState3C;
	int m_nChildState40;
	int m_nOwnerMotion44;
	int m_nChildMotion48;
	char m_abReserved4C[8];
	int m_nOwnerRuntime54;
	int m_nChildRuntime58;

	void ClearChildState(void);
	void ClearOwnerState(void);
};

// FUNCTION: LEMBALL 0x004323c0
void VsQueuedRenderPointSinkView::SetSink(int nSink)
{
	m_nSink30 = nSink;
}

// FUNCTION: LEMBALL 0x00432860
void VsWindowOwnerRuntimeStateView::ClearChildState(void)
{
	m_nChildMotion48 = 0;
	m_nChildState40 = 0;
	m_nChildRuntime58 = 0;
}

// FUNCTION: LEMBALL 0x00432870
void VsWindowOwnerRuntimeStateView::ClearOwnerState(void)
{
	m_nOwnerMotion44 = 0;
	m_nOwnerState3C = 0;
	m_nOwnerRuntime54 = 0;
}

// FUNCTION: LEMBALL 0x00458180
int LEMBALL_FASTCALL IsDisplayModeWindowedRange(const int* pnDisplayMode)
{
	switch (*pnDisplayMode) {
	case 2:
	case 3:
	case 4:
	case 5:
		return 1;
	default:
		return 0;
	}
}
