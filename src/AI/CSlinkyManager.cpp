// FUNCTION: LEMBALL 0x0040be20
void __fastcall CSlinkyManager_Process(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0xc) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0xc); i++) {
			(*( void(**)(void)) (*(void***) (*(int*) ((char*) pObject + 4) + i * 0x150) + 0x14 / 4))();
		}
	}
}
// FUNCTION: LEMBALL 0x0040b9e0
void __fastcall CSlinkyManager_Add(void* pThis, int nUnused, unsigned short param_1, int param_2, int param_3, int param_4, int param_5)
{
	if (*(int*) ((char*) pThis + 0xc) < *(int*) ((char*) pThis + 8)) {
		((void(__fastcall*)(void*, unsigned short)) 0x402293)(
			(void*) (*(int*) ((char*) pThis + 0xc) * 0x150 + *(int*) ((char*) pThis + 4)), param_1);
		((void(__fastcall*)(void*, int, int, int, int)) 0x402892)(
			(void*) (*(int*) ((char*) pThis + 0xc) * 0x150 + *(int*) ((char*) pThis + 4)),
			param_2, param_4, param_3, param_5);
		*(int*) ((char*) pThis + 0xc) = *(int*) ((char*) pThis + 0xc) + 1;
	}
}
// FUNCTION: LEMBALL 0x0040be50
void __fastcall CSlinkyManager_LoadLevel(void* pThis, int nUnused, unsigned short* pStream)
{
	unsigned short nCount = *pStream;
	pStream++;
	((void(__fastcall*) (void*, int, int)) 0x403396)(pThis, 0, nCount);
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) (*(int*) ((char*) pThis) + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			unsigned int word1 = *(pStream);
			pStream++;
			unsigned int word2 = *(pStream);
			pStream++;
			unsigned int word3 = *(pStream);
			pStream++;
			unsigned int word4 = *(pStream);
			pStream++;
			((void(__fastcall*) (void*, int, int, int, int, int, int)) 0x402c6b)(pThis, 0, uSlot, word1, word2, word3, word4);
			nCount--;
		} while (nCount != 0);
	}
}
