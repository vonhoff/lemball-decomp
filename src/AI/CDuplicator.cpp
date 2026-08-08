extern void* g_pLevelTileGrid;
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);

// FUNCTION: LEMBALL 0x004275f0
void __fastcall CDuplicator_Restart(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	*(unsigned short*) ((char*) pObject + 0xbc) = 0;
	*(int*) ((char*) pObject + 0x94) = 0;
	*(int*) ((char*) pObject + 0x13c) = 0;
	*(int*) ((char*) pObject + 0x138) = 0;
	*(int*) ((char*) pObject + 0xb8) = 0x18;
	((void(__fastcall*)(void*, void*)) 0x4023e2)(pObject, (char*) pObject + 0x40);
}
// FUNCTION: LEMBALL 0x00427630
void __fastcall CDuplicator_Set(void* pObject, int nUnused, int* pPos)
{
	int iVar3 = *pPos;
	*(int*) ((char*) pObject + 0x9c) = iVar3;
	int iVar4 = pPos[1];
	*(int*) ((char*) pObject + 0xa0) = iVar4;
	int iVar2 = pPos[2];
	*(int*) ((char*) pObject + 0x138) = 1;
	*(int*) ((char*) pObject + 0x13c) = 1;
	*(int*) ((char*) pObject + 0xa4) = iVar2;
	iVar3 = (iVar3 >> 12) / 16;
	iVar4 = (iVar4 >> 12) / 16;
	if (iVar3 > -1) {
		if (iVar4 > -1 && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar4 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (iVar4 * *(int*) ((char*) g_pLevelTileGrid + 0x10) + iVar3) * 0xc) |= 1;
		}
		iVar4--;
		if (iVar4 > -1) {
			if (iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar4 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar4 + iVar3) * 0xc) |= 1;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004276f0
void __fastcall CDuplicator_Delete(void* pObject, int nUnused)
{
	int iVar2 = (*(int*) ((char*) pObject + 0x9c) >> 12) / 16;
	int iVar3 = (*(int*) ((char*) pObject + 0xa0) >> 12) / 16;
	if (iVar2 > -1) {
		if (iVar3 > -1 && iVar2 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar3 + iVar2) * 0xc) &= 0xfffe;
		}
		iVar3--;
		if (iVar3 > -1) {
			if (iVar2 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar3 + iVar2) * 0xc) &= 0xfffe;
			}
		}
	}
}
