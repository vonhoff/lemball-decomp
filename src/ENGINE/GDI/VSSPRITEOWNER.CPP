#include "GAME.H"

struct VsResourceSpriteWindowOwnerState {
	unsigned char m_abReserved00[0xf4];
	void* m_pRenderDispatchQueueF4;
	unsigned char m_abReservedF8[0x54];
	int m_nState14C;

	void Initialize(void);
};

// FUNCTION: LEMBALL 0x00469670
void VsResourceSpriteWindowOwnerState::Initialize(void)
{
	m_nState14C = 0;
	m_pRenderDispatchQueueF4 = g_pSharedRenderDispatchQueue;
}
