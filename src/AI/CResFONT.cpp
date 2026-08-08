#include "AI/CResFONT.h"
#include "Visos/Generic/Memory.h"

extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);

// FUNCTION: LEMBALL 0x0045da50
void CResFONT::UnLoadResources(unsigned long nIndex, unsigned char fReleaseMode)
{
	ReleaseTypedResourceObjectIfLoaded((void*) (m_pGlyphData80 + nIndex * 0x4c), 0, fReleaseMode);
	ReleaseTypedResourceObjectIfLoaded((void*) (m_pIntCodeData7C + nIndex * 0x54), 0, fReleaseMode);
}
// FUNCTION: LEMBALL 0x0045da90
void CResFONT::UnLoadVramData(unsigned long nIndex, unsigned char fReleaseMode)
{
	(*( void(**)(int)) (*(void***) (m_pIntCodeData7C + nIndex * 0x54) + 0x24 / 4))(fReleaseMode);
}
// FUNCTION: LEMBALL 0x0045daf0
void CResFONT::OnLoad(void)
{
	if (m_pFontTable78 == 0) {
		void* pThis = (void*) ((void*(__cdecl*)(int)) 0x45baf0)(8);
		if (pThis != 0) {
			m_pFontTable78 = (unsigned char*) ((void*(__fastcall*)(void*, void*)) 0x473650)(pThis, this);
			return;
		}
		m_pFontTable78 = 0;
	}
}
// FUNCTION: LEMBALL 0x0045d990
int CResFONT::DirectResources(unsigned long nIndex, unsigned char*& pIntCodeData, unsigned char*& pGlyphData)
{
	int iVar1 = ((int(__cdecl*)(void*, void*, void*, void*)) 0x45cfb0)(m_pGlyphData80 + nIndex * 0x4c, pGlyphData, pIntCodeData, this);
	if (iVar1 == 0) {
		iVar1 = ((int(__cdecl*)(void*, void*, void*, void*)) 0x45cfb0)(m_pIntCodeData7C + nIndex * 0x54, pGlyphData, pIntCodeData, this);
		if (iVar1 == 0) {
			return 0;
		}
	}
	return 1;
}
// FUNCTION: LEMBALL 0x0045d9f0
int CResFONT::DirectResources(unsigned long nIndex, unsigned char*& pGlyphData)
{
	int iVar1 = ((int(__cdecl*)(void*, void*, void*)) 0x45cf70)(m_pGlyphData80 + nIndex * 0x4c, pGlyphData, this);
	if (iVar1 == 0) {
		iVar1 = ((int(__cdecl*)(void*, void*, void*)) 0x45cf70)(m_pIntCodeData7C + nIndex * 0x54, pGlyphData, this);
		if (iVar1 == 0) {
			return 0;
		}
	}
	return 1;
}
// FUNCTION: LEMBALL 0x0045dab0
int CResFONT::ForceLoadVram(unsigned long nIndex)
{
	void* pSlot = *(void**) (m_pIntCodeData7C + nIndex * 0x54);
	int r = ((int(__fastcall*)(void*)) (*(int*) pSlot + 0xc))(pSlot);
	if (r != 0) {
		return 1;
	}
	return ((int(__fastcall*)(void*)) (*(int*) pSlot + 0xc))(pSlot);
}
// FUNCTION: LEMBALL 0x0045d8b0
void CResFONT::AllocateResources(unsigned long cEntries)
{
	unsigned int* pGlyphBase;
	unsigned int* pIntCodeBase;
	unsigned int i;

	pGlyphBase = (unsigned int*) AllocateVSMemBlock(cEntries * 76u + 4u);
	if (pGlyphBase != (void*) 0x0) {
		pGlyphBase[0] = cEntries;
		for (i = 0; (int) i < (int) cEntries; i++) {
			((void* (__fastcall*) (void*)) 0x45e8f0)((char*) (pGlyphBase + 1) + i * 76);
		}
		m_pGlyphData80 = (unsigned char*) (pGlyphBase + 1);
	}
	else {
		m_pGlyphData80 = (unsigned char*) 0x0;
	}
	pIntCodeBase = (unsigned int*) AllocateVSMemBlock(cEntries * 84u + 4u);
	if (pIntCodeBase != (void*) 0x0) {
		pIntCodeBase[0] = cEntries;
		for (i = 0; (int) i < (int) cEntries; i++) {
			((void* (__fastcall*) (void*)) 0x45e7e0)((char*) (pIntCodeBase + 1) + i * 84);
		}
		m_pIntCodeData7C = (unsigned char*) (pIntCodeBase + 1);
	}
	else {
		m_pIntCodeData7C = (unsigned char*) 0x0;
	}
}
