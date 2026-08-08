// FUNCTION: LEMBALL 0x00422420
void __fastcall CCollectableManager_Restart(void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x34) != 0 && *(int*) ((char*) param_1 + 0x38) > 0) {
		int i;
		for (i = 0; i < *(int*) ((char*) param_1 + 0x38); i++) {
			void* pElem = *(void**) (*(int*) ((char*) param_1 + 0x34) + i * 4);
			if (pElem != 0) {
				(*( void(**)(void)) (*(void***) pElem + 0x104 / 4))();
			}
		}
	}
}
// FUNCTION: LEMBALL 0x00422790
void __fastcall CCollectableManager_LoadLevel(void* pThis, int nUnused, unsigned short* pStream, int param_3)
{
	unsigned short nCount = *pStream;
	pStream++;
	if (param_3 == 0) {
		((void(__fastcall*) (void*, int, int)) 0x40187f)(pThis, 0, nCount);
	}
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x30) + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			unsigned int subtype = *(pStream);
			pStream++;
			unsigned int coord2 = *(pStream);
			pStream++;
			unsigned int coord3 = *(pStream);
			pStream++;
			unsigned int coord4 = *(pStream);
			pStream++;
			int appendType = (int) subtype;
			if (param_3 == 0) {
				void* pBase = *(void**) ((char*) pThis + 0x30);
				if ((subtype == 0xc || subtype == 0xb) && *(int*) ((char*) pBase + 0x64) == 1) {
					if (*(int*) ((char*) pBase + 0x7c) == 1) {
						if (subtype == 0xc) appendType = 0xc; else appendType = 0xb;
					}
					else {
						if (subtype == 0xc) appendType = 0xb; else appendType = 0xc;
					}
				}
			}
			((void(__fastcall*) (void*, int, int, int, int, int, int)) 0x401afa)(*(void**) ((char*) pThis + 0x30), 0, uSlot, coord2, coord3, coord4, appendType);
			nCount--;
		} while (nCount != 0);
	}
}
