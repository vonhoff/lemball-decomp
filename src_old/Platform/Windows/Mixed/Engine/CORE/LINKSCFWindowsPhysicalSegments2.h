#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_007)
// FUNCTION: LEMBALL 0x00425dc0
void __fastcall DestroyLiftChunkManager0x190StrideArray(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0x34) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
			((void(__fastcall*)(void*)) 0x402a77)((void*) (*(int*) ((char*) pObject + 0x3c) + i * 0x190));
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_010)
// Compatibility symbol preserving MSVC 4.00 compiler state after moving the
// authoritative projectile destructor to Projectile.cpp.
void* __fastcall PreserveDeleteProjectileObjectScalarWrapperCodegen(void* pThis, int nUnused, unsigned char param_1)
{
	void* pu = ((char*) pThis + 0x138);
	if (pThis == 0) {
		pu = 0;
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pu);
	DestroyLevelChunkObjectBaseAutoThunk(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pThis);
	}
	return pThis;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_038)
// FUNCTION: LEMBALL 0x00422510
void __fastcall DestroyManagedEntityPointerArray(void* pObject)
{
	int i;
	for (i = 0; i < *(int*) ((char*) pObject + 0x3c); i++) {
		void* pElem = *(void**) (*(int*) ((char*) pObject + 0x34) + i * 4);
		if (pElem != 0) {
			(*(void (**)(int)) * *(void***) pElem)(1);
		}
	}
	*(int*) ((char*) pObject + 0x3c) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_047)
// FUNCTION: LEMBALL 0x00409d20
void __fastcall SetInvsChunkObjectPointBounds(void* pObject,
											  int nUnused,
											  unsigned short param_2,
											  unsigned short param_3,
											  unsigned short param_4)
{
	unsigned short p1[3];
	unsigned short p2[3];
	p1[0] = param_2;
	p1[1] = param_3;
	p1[2] = param_4;
	p2[0] = param_2;
	p2[1] = param_3;
	p2[2] = param_4;
	((void(__fastcall*)(void*, void*, void*)) 0x40237e)(pObject, p1, p2);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_060)
// FUNCTION: LEMBALL 0x0042b440
int __fastcall ActivateFirstPendingTramChunkObjectAtNearbyPoint(void* pObject, int nUnused, void* param_2, int param_3)
{
	int i;
	for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
		void* pElem = (void*) (*(int*) ((char*) pObject + 0x38) + i * 0x144);
		if (*(int*) ((char*) pElem + 0x13c) != 0 && *(int*) ((char*) pElem + 0x138) == 0) {
			if (((int(__cdecl*)(void*, void*, int)) 0x403116)(pElem, param_2, param_3) != 0) {
				return 1;
			}
		}
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_062)
// FUNCTION: LEMBALL 0x004257e0
int __fastcall SerializeLiftChunkStartPoints(void* pObject, int nUnused, void* param_1)
{
	int i;
	if (*(int*) ((char*) pObject + 0x34) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
			void* pElem = (void*) (*(int*) ((char*) pObject + 0x3c) + i * 0x190);
			*(int*) param_1 = *(int*) ((char*) pElem + 0x13a);
			*(unsigned short*) ((char*) param_1 + 4) = *(unsigned short*) ((char*) pElem + 0x13e);
			param_1 = (char*) param_1 + 6;
		}
	}
	return *(int*) ((char*) pObject + 0x34);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_077)
// FUNCTION: LEMBALL 0x0041b9f0
void* __fastcall FindCountedChildContainingFixedPoint(void* pThis, int nUnused, int x, int y)
{
	unsigned int nCount = *(unsigned short*) ((char*) pThis + 0x36);
	int* pChild = *(int**) ((char*) pThis + 0x3c);
	for (unsigned int i = 0; i < nCount; i++) {
		int tx = *(int*) (*pChild + 0x9c) >> 12;
		int ty = *(int*) (*pChild + 0xa0) >> 12;
		int rx = x >> 12;
		int ry = y >> 12;
		if ((tx - 8 < rx) && (rx < tx) && (ty - 8 < ry) && (ry < ty)) {
			return *(void**) (*(int*) ((char*) pThis + 0x3c) + i * 4);
		}
		pChild++;
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_078)
// FUNCTION: LEMBALL 0x0041ba80
void* __fastcall FindManagedEntityAtTileByType(void* pThis, int nUnused, int x, int y, int pUnused, int nType)
{
	int rx = x >> 12;
	int ry = y >> 12;
	unsigned int i = 0;
	while (true) {
		unsigned int nCount = *(unsigned short*) ((char*) pThis + 0x36);
		if (nCount <= i) {
			return 0;
		}
		void* pChild = *(void**) (*(int*) ((char*) pThis + 0x3c) + i * 4);
		if (*(int*) ((char*) pChild + 0x64) == nType) {
			int tx = *(int*) ((char*) pChild + 0x9c) >> 12;
			int ty = *(int*) ((char*) pChild + 0xa0) >> 12;
			if ((tx - 8 < rx) && (rx < tx) && (ty - 8 < ry) && (ry < ty)) {
				return *(void**) (*(int*) ((char*) pThis + 0x3c) + i * 4);
			}
		}
		i++;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_131)
// FUNCTION: LEMBALL 0x00418be0
void __fastcall configure_managed_entity_profile_ranges(void* pThis,
														int nEdxSlop,
														int nCount,
														int nData0,
														int nData1,
														int nData2,
														int nData3)
{
	*(int*) ((char*) pThis + 0x124) = nCount;
	if (nData0 == -1) {
		const int* pData = ((const int*) 0x0049d138) + nCount * 4;
		*(int*) ((char*) pThis + 0x110) = pData[0];
		*(int*) ((char*) pThis + 0x114) = pData[1];
		*(int*) ((char*) pThis + 0x118) = pData[2];
		*(int*) ((char*) pThis + 0x11c) = pData[3];
	}
	else {
		*(int*) ((char*) pThis + 0x110) = nData0;
		*(int*) ((char*) pThis + 0x114) = nData1;
		*(int*) ((char*) pThis + 0x118) = nData2;
		*(int*) ((char*) pThis + 0x11c) = nData3;
	}
	if (nCount > 0) {
		int nIndex = 0;
		int* pLo = (int*) ((char*) pThis + 0xe0);
		int* pHi = pLo + 4;
		do {
			if (*pLo > 0x400 || *pLo < 0) {
				*pLo = nIndex;
			}
			if (*pHi > 0x400 || *pHi < 0) {
				*pHi = nIndex;
			}
			pLo++;
			pHi++;
			nIndex += 0x10;
			nCount--;
		} while (nCount != 0);
	}
}
#endif
