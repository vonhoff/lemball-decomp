#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;
extern unsigned char g_GAME_ManagedEntitySlotBitMasks[8];
extern unsigned char g_GAME_ManagedEntitySlotClaimBitset[0x100];

void LEMBALL_FASTCALL ClaimManagedEntitySlotId(int nManagedEntityObject);

// FUNCTION: LEMBALL 0x00416610
unsigned short CGameObject::GetId(void)
{
	return m_nSlotId6C;
}

// FUNCTION: LEMBALL 0x00416620
void CGameObject::SetId(unsigned short nSlotId)
{
	m_nSlotId6C = nSlotId;
	ClaimManagedEntitySlotId((int) (unsigned long) this);
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

// Macintosh: CGameObject::RegisterId()
// FUNCTION: LEMBALL 0x00416740
void LEMBALL_FASTCALL ClaimManagedEntitySlotId(int nManagedEntityObject)
{
	unsigned char bClaimByte;
	unsigned short nSlotId;
	unsigned int i;
	unsigned int cManagedEntities;

	nSlotId = *(unsigned short*) (nManagedEntityObject + 0x6c);
	if (nSlotId != 0xffff) {
		bClaimByte = g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3];
		if ((g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] & bClaimByte) != 0) {
			cManagedEntities = (unsigned int) g_GAME_ManagedEntityRegistryCount;
			for (i = 0; (int) i < (int) cManagedEntities; ++i) {
				if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
					((CGameObject*) g_GAME_ManagedEntityRegistryTable[i & 0xffff])->GetId();
				}
			}
			*(unsigned short*) (nManagedEntityObject + 0x6c) = 0xffff;
			return;
		}
		g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3] = g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] | bClaimByte;
	}
}

void ClaimManagedEntitySlotIdCompatibility(int nManagedEntityObject)
{
	ClaimManagedEntitySlotId(nManagedEntityObject);
}
