#include "AI/CGameObject.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;
extern unsigned char g_GAME_ManagedEntitySlotBitMasks[8];
extern unsigned char g_GAME_ManagedEntitySlotClaimBitset[0x100];

unsigned short LEMBALL_FASTCALL GetManagedEntitySlotId(int nManagedEntityObject);

struct ManagedEntitySlotOwnerView {
	void SetManagedEntitySlotId(unsigned short nSlotId);
};

void LEMBALL_FASTCALL ClaimManagedEntitySlotId(int nManagedEntityObject);

// FUNCTION: LEMBALL 0x00416620
void CGameObject::SetId(unsigned short nSlotId)
{
	m_nSlotId6C = nSlotId;
	ClaimManagedEntitySlotId((int) (unsigned long) this);
}

void ManagedEntitySlotOwnerView::SetManagedEntitySlotId(unsigned short nSlotId)
{
	((CGameObject*) this)->SetId(nSlotId);
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
					GetManagedEntitySlotId(g_GAME_ManagedEntityRegistryTable[i & 0xffff]);
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
