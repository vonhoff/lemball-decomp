#include "ENGINE/CORE/COMMON.H"

struct VsQueuedRenderPointSinkView {
	char m_abReserved00[0x30];
	int m_nSink30;

	void SetSink(int nSink);
};

struct VsShortPoint {
	short m_nX;
	short m_nY;
};

struct VsWindowOwnerRuntimeStateView {
	char m_abReserved00[0x34];
	int m_nFixedPointX34;
	int m_nFixedPointY38;
	int m_nOwnerState3C;
	int m_nChildState40;
	int m_nOwnerMotion44;
	int m_nChildMotion48;
	char m_abReserved4C[8];
	int m_nOwnerRuntime54;
	int m_nChildRuntime58;
	char m_abReserved5C[0x14];
	int m_nRenderOriginX70;
	int m_nRenderOriginY74;

	void GetPixelPosition(VsShortPoint* pPoint);
	void ClearChildState(void);
	void ClearOwnerState(void);
	void SetRenderOrigin(int nX, int nY);
};

// FUNCTION: LEMBALL 0x004323c0
void VsQueuedRenderPointSinkView::SetSink(int nSink)
{
	m_nSink30 = nSink;
}

// FUNCTION: LEMBALL 0x004324a0
void LEMBALL_FASTCALL CacheObjectField44AsCurrentContext(void* pObject)
{
	extern void* g_pCachedCurrentContextWindow;
	extern void* g_pCachedCurrentContextOwner;

	g_pCachedCurrentContextWindow = *(void**) ((char*) pObject + 0x44);
	g_pCachedCurrentContextOwner = pObject;
}

// FUNCTION: LEMBALL 0x004324d0
int LEMBALL_FASTCALL IsObjectField44CurrentContext(void* pObject)
{
	extern void* g_pCachedCurrentContextWindow;

	return *(void**) ((char*) pObject + 0x44) == g_pCachedCurrentContextWindow;
}

// FUNCTION: LEMBALL 0x00432840
void VsWindowOwnerRuntimeStateView::GetPixelPosition(VsShortPoint* pPoint)
{
	int nY;

	nY = m_nFixedPointY38;
	pPoint->m_nX = (short) (m_nFixedPointX34 >> 12);
	pPoint->m_nY = (short) (nY >> 12);
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

// FUNCTION: LEMBALL 0x00432920
void VsWindowOwnerRuntimeStateView::SetRenderOrigin(int nX, int nY)
{
	m_nRenderOriginX70 = nX;
	m_nRenderOriginY74 = nY;
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
