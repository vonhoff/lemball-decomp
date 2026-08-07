// Cluster reconstruction: clean scan/array operations appended to a fresh TU
// (avoids disturbing existing translation units' formatting).

// MACINTOSH: append_static_anim_chunk_record_if_unique(short*)
// FUNCTION: LEMBALL 0x0040d0c0
void __fastcall AppendStaticAnimChunkRecordIfUnique(void* pObject, int nUnused, short* param_1)
{
	int i;
	if (((int(__fastcall*)(void*, short*)) 0x40291e)(pObject, param_1) != 0) {
		return;
	}
	if (*(int*) ((char*) pObject + 4) >= 200) {
		return;
	}
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 0x1c) = 0;
	*(unsigned short*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 0x1a) = 0;
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 8) = *(int*) param_1;
	*(short*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 0xc) = param_1[2];
	*(int*) ((char*) pObject + 4) = *(int*) ((char*) pObject + 4) + 1;
	if (*(int*) ((char*) pObject + 4) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 4); i++) {
			*(unsigned short*) ((char*) pObject + 0x14 + i * 0x18) = *(unsigned short*) ((char*) pObject + 0x14 + i * 0x18 + 2);
		}
	}
}

// MACINTOSH: get_last_queued_projectile_request_code()
// FUNCTION: LEMBALL 0x0040fbe0
int __fastcall GetLastQueuedProjectileRequestCode(void* pObject)
{
	int i;
	int count = *(int*) ((char*) pObject + 0x220);
	if (count == 0) {
		return 0xffff;
	}
	for (i = count - 1; i >= 0; i--) {
		switch (*(int*) ((char*) pObject + 0x1c0 + i * 4)) {
		case 0x27: return 0x27;
		case 0x29: return 0x29;
		case 0x2b: return 0x2b;
		case 0x2d: return 0x2d;
		}
	}
	return 0xffff;
}

// MACINTOSH: remove_queued_projectile_request_code(int)
// FUNCTION: LEMBALL 0x0040fc50
void __fastcall RemoveQueuedProjectileRequestCode(void* pObject, int nUnused, int param_1)
{
	int iVar1 = *(int*) ((char*) pObject + 0x220);
	int iVar4 = 0;
	if (iVar1 > 0) {
		int* piVar3 = (int*) ((char*) pObject + 0x1c0);
		while (piVar3[0] != param_1) {
			piVar3++;
			iVar4++;
			if (iVar1 <= iVar4) {
				return;
			}
		}
		iVar4++;
		if (iVar4 < iVar1) {
			do {
				*(int*) ((char*) pObject + iVar4 * 4 + 0x1bc) = *(int*) ((char*) pObject + (iVar4 + 1) * 4 + 0x1bc);
				iVar4++;
			} while (iVar4 < *(int*) ((char*) pObject + 0x220));
		}
		*(int*) ((char*) pObject + 0x220) = *(int*) ((char*) pObject + 0x220) - 1;
	}
}
