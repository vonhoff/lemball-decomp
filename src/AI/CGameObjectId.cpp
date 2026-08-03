#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;
extern unsigned char g_GAME_ManagedEntitySlotBitMasks[8];
extern unsigned char g_GAME_ManagedEntitySlotClaimBitset[0x100];

extern int g_fLevelEndStateRequestLatched;
extern void* g_pActiveManagedEntityOwner;

// FUNCTION: LEMBALL 0x004165e0
void CGameObject::Init(CAI* pAI)
{
	int i;
	unsigned char* pbBits;

	g_fLevelEndStateRequestLatched = 0;
	g_pActiveManagedEntityOwner = pAI;
	pbBits = g_GAME_ManagedEntitySlotClaimBitset;
	for (i = 0x40; i != 0; --i) {
		pbBits[0] = 0;
		pbBits[1] = 0;
		pbBits[2] = 0;
		pbBits[3] = 0;
		pbBits += 4;
	}
	g_GAME_ManagedEntitySlotClaimBitset[0] |= g_GAME_ManagedEntitySlotBitMasks[0];
}

// FUNCTION: LEMBALL 0x00416610
unsigned short CGameObject::GetId(void)
{
	return m_nSlotId6C;
}

// FUNCTION: LEMBALL 0x00416620
void CGameObject::SetId(unsigned short nSlotId)
{
	m_nSlotId6C = nSlotId;
	RegisterId();
}

// FUNCTION: LEMBALL 0x00416640
void CGameObject::ReSetId(void)
{
	unsigned short nSlotId = m_nSlotId6C;
	if (nSlotId != 0xffff) {
		g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3] &=
			(unsigned char) ~g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7];
	}
}

// FUNCTION: LEMBALL 0x00416670
unsigned short CGameObject::NextId(void)
{
	unsigned int iByte = 0;
	do {
		if (g_GAME_ManagedEntitySlotClaimBitset[iByte] != 0xff) {
			int iBit = 0;
			do {
				if ((g_GAME_ManagedEntitySlotBitMasks[iBit] & g_GAME_ManagedEntitySlotClaimBitset[iByte]) == 0) {
					return (unsigned int) (iBit + iByte * 8);
				}
				++iBit;
			} while (iBit < 8);
		}
		++iByte;
	} while ((int) iByte < 0x100);
	return iByte & 0xffff0000;
}

// FUNCTION: LEMBALL 0x004166a0
unsigned short CGameObject::NextLoadingId(void)
{
	unsigned int iByte = 0xff;
	do {
		if (g_GAME_ManagedEntitySlotClaimBitset[iByte] != 0xff) {
			int iBit = 7;
			do {
				if ((g_GAME_ManagedEntitySlotBitMasks[iBit] & g_GAME_ManagedEntitySlotClaimBitset[iByte]) == 0) {
					return (unsigned int) (iBit + iByte * 8);
				}
				--iBit;
			} while (0 < iBit);
		}
		--iByte;
	} while (0 < (int) iByte);
	return iByte & 0xffff0000;
}

// FUNCTION: LEMBALL 0x00416740
void CGameObject::RegisterId(void)
{
	unsigned char bClaimByte;
	unsigned short nSlotId;
	unsigned int i;
	unsigned int cManagedEntities;

	nSlotId = m_nSlotId6C;
	if (nSlotId != 0xffff) {
		bClaimByte = g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3];
		if ((g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] & bClaimByte) != 0) {
			cManagedEntities = (unsigned int) g_GAME_ManagedEntityRegistryCount;
			for (i = 0; (int) i < (int) cManagedEntities; ++i) {
				if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
					((CGameObject*) g_GAME_ManagedEntityRegistryTable[i & 0xffff])->GetId();
				}
			}
			m_nSlotId6C = 0xffff;
			return;
		}
		g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3] = g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] | bClaimByte;
	}
}
