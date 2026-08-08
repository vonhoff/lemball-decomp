// FUNCTION: LEMBALL 0x0041ebe0
void __fastcall CGenericGroupManager_CreateNewGroup(void* pObject, int nUnused, unsigned short nIdCount, unsigned short* pIdList)
{
	void* pMsg;
	void* pGroup;
	if (*(int*) ((char*) pObject + 0xa4) < 0x28) {
		pMsg = 0;
		pGroup = (void*) ((void* (__cdecl*)(int)) 0x45a780)(0x168);
		if (pGroup != 0) {
			pMsg = ((void* (__fastcall*)(void*, void*, void*, void*, void*)) 0x402103)(pGroup, 0,
				(void*) (*(int*) 0x4a782c), (void*) (*(int*) 0x4a7830), (void*) (*(int*) 0x4a7834));
		}
		*(void**) ((char*) pObject + *(int*) ((char*) pObject + 0xa4) * 4 + 4) = pMsg;
		*(int*) ((char*) pObject + 0xa4) += 1;
		if (nIdCount != 0) {
			unsigned int nIdx;
			typedef void (__fastcall * GroupAddProc)(void*, void*, void*, void*);
			GroupAddProc pAdd = *(GroupAddProc*) ((char*) *(void**) pObject + 0x3c);
			for (nIdx = 0; nIdx < nIdCount; nIdx++) {
				unsigned short uId = pIdList[nIdx];
				pAdd(pObject, 0, (void*) (0x4a6510 + (unsigned int) uId * 4), pMsg);
			}
		}
	}
}
