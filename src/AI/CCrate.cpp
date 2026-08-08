#include "AI/CCrate.h"

// FUNCTION: LEMBALL 0x0041ccc0
void CCrate::TriggerContents(void)
{
	if (m_nContentsType14C != 0xffff) {
		void* pvVar1 = m_pContents148;
		m_nPositionX9C = *(int*) ((char*) pvVar1 + 0x9c);
		m_nPositionYA0 = *(int*) ((char*) pvVar1 + 0xa0);
		m_nPositionZA4 = *(int*) ((char*) pvVar1 + 0xa4);
		((void(__fastcall*)(void*, int, void*, int)) 0x402cac)(*(void**) 0x4a74c0, -1, pvVar1, 0);
		m_nContentsType14C = 0xffff;
	}
}
