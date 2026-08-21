#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_013)
// FUNCTION: LEMBALL 0x0044B750
void* __fastcall construct_registration_info_screen(char* pThis,
													int,
													int* pRenderOwner,
													void* pRenderTarget,
													short* pSize)
{
	int nIndex;
	char* pEntry;
	void* pResource;
	int* pText;
	unsigned int nTime;
	*(void**) pThis = (void*) 0x496d48;
	((void(__fastcall*)(void*, int)) 0x462ea0)(pThis + 4, 0);
	*(void**) (pThis + 0x24) = (void*) 0x496ca8;
	*(int*) (pThis + 0x18) = 0;
	*(void**) (pThis + 0x24) = (void*) 0x496d38;
	*(void**) (pThis + 0x34) = (void*) 0x496c98;
	*(void**) (pThis + 0x3c) = (void*) 0x496ca8;
	*(int*) (pThis + 0x28) = 0;
	*(int*) (pThis + 0x2c) = 0;
	*(int*) (pThis + 0x40) = 0;
	*(void**) (pThis + 0x3c) = (void*) 0x497928;
	*(int*) (pThis + 0x44) = 0;
	*(void**) (pThis + 0x3c) = (void*) 0x497918;
	*(int*) (pThis + 0x48) = 0;
	*(int*) (pThis + 0x5c) = 0;
	pEntry = pThis + 0x64;
	for (nIndex = 0; nIndex < 2; ++nIndex, pEntry += 0x10) {
		((void(__fastcall*)(void*, int)) 0x403139)(pEntry, 0);
	}
	*(void**) pThis = (void*) 0x497cb0;
	*(void**) (pThis + 4) = (void*) 0x497ca0;
	*(int*) (pThis + 0x14) = 0;
	((void(__fastcall*)(void*, int, int)) 0x46b370)(*(void**) 0x4a9bf4, 0, 0);
	*(int*) (pThis + 0x38) = 0;
	((void(__fastcall*)(void*, int, void*, int)) 0x4632a0)(*(void**) 0x4a9360, 0, pThis + 4, 0);
	*(int**) (pThis + 0x1c) = pRenderOwner;
	*(void**) (pThis + 0x20) = pRenderTarget;
	*(short*) (pThis + 0x18) = pSize[0];
	*(short*) (pThis + 0x1a) = pSize[1];
	((void(__fastcall*)(int*, int, int))(*(void***) pRenderOwner)[0x2b])(pRenderOwner, 0, 0x102);
	*(void**) (pThis + 0x60) = ((void*(__cdecl*) (int) ) 0x45e210)(0x101);
	*(int*) (pThis + 0x8c) = 0;
	pResource = ((void*(__cdecl*) (int) ) 0x45a780)(0x24);
	if (pResource) {
		*(void**) (pThis + 0x90) =
			((void*(__fastcall*) (void*, int, int, int, int, int) ) 0x469c60)(pResource, 0, 0x2b6, 1, 10, 0);
	}
	else {
		*(void**) (pThis + 0x90) = 0;
	}
	((void(__fastcall*)(void*, int, int)) 0x469eb0)(*(void**) (pThis + 0x90), 0, 0x32);
	pText = ((int*(__cdecl*) (int) ) 0x45de00)(0x100);
	*(int**) (pThis + 0x84) = pText;
	if (pText[4]) {
		pText[9] = 0;
	}
	else {
		((void(__fastcall*)(int*, int))(*(void***) pText)[7])(pText, 0);
	}
	++pText[2];
	*(int*) (pThis + 0x88) = pText[14];
	nTime = ((unsigned int(__cdecl*)(void)) 0x462e80)();
	*(unsigned int*) (pThis + 0x94) = nTime;
	*(unsigned int*) (pThis + 0x98) = nTime + 5000;
	return pThis;
}

// FUNCTION: LEMBALL 0x0044BCA0
void __fastcall prepare_registration_info_screen_active_upload(char* pThis)
{
	void* pUpload;
	void* pRender;
	int nCount;
	int nIndex;
	short* pRect;
	short aRect[4];
	int nBitmap;
	short aZero[2];
	pRender = *(void**) (*(char**) (pThis + 0x20) + 0x0c);
	pUpload = ((void*(__fastcall*) (void*, int) )(*(void***) pRender)[2])(pRender, 0);
	nCount = ((int(__fastcall*)(void*, int)) 0x466ef0)(pUpload, 0);
	nIndex = ((int(__fastcall*)(void*, int)) 0x467020)(pUpload, 0);
	if (nIndex < nCount) {
		pRect = ((short*(__fastcall*) (void*, int, int) ) 0x467000)(pUpload, 0, nIndex);
		aRect[0] = pRect[0];
		aRect[1] = pRect[1];
		aRect[2] = pRect[2];
		aRect[3] = pRect[3];
		while (++nIndex < nCount) {
			pRect = ((short*(__fastcall*) (void*, int, int) ) 0x467000)(pUpload, 0, nIndex);
			((void(__fastcall*)(short*, int, short*)) 0x402798)(aRect, 0, pRect);
		}
		if ((int) aRect[0] * aRect[1] > 0) {
			if (*(short*) (pThis + 0x18) < aRect[0]) {
				aRect[0] = *(short*) (pThis + 0x18);
			}
			if (*(short*) (pThis + 0x1a) < aRect[1]) {
				aRect[1] = *(short*) (pThis + 0x1a);
			}
			*(short*) (pThis + 0x68) = aRect[0];
			*(short*) (pThis + 0x6a) = aRect[1];
			*(short*) (pThis + 0x6c) = aRect[2];
			*(short*) (pThis + 0x6e) = aRect[3];
			*(int*) (pThis + 0x70) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x64))[1])(pThis + 0x64, 0, *(void**) (pThis + 0x20));
			nBitmap = *(int*) (pThis + 0x60);
			*(short*) (pThis + 0x28) = *(short*) (pThis + 0x18);
			*(short*) (pThis + 0x2a) = *(short*) (pThis + 0x1a);
			*(int*) (pThis + 0x2c) = 0;
			*(int*) (pThis + 0x30) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x24))[1])(pThis + 0x24, 0, *(void**) (pThis + 0x20));
			*(short*) (pThis + 0x40) = (*(short*) (pThis + 0x18) - *(short*) (nBitmap + 0x48)) / 2;
			*(short*) (pThis + 0x42) = (*(short*) (pThis + 0x1a) - *(short*) (nBitmap + 0x4a)) / 2;
			*(int*) (pThis + 0x4c) = nBitmap;
			*(int*) (pThis + 0x50) = 0x800;
			*(int*) (pThis + 0x54) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x3c))[1])(pThis + 0x3c, 0, *(void**) (pThis + 0x20));
			((void(__fastcall*)(void*, int)) 0x403733)(pThis, 0);
			aZero[0] = aZero[1] = 0;
			*(int*) (pThis + 0x80) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x74))[1])(pThis + 0x74, 0, *(void**) (pThis + 0x20));
		}
	}
	((void(__fastcall*)(void*, int)) 0x466b60)(pUpload, 0);
	((void(__fastcall*)(void*, int, void*)) 0x4670f0)(*(void**) (pThis + 0x20), 0, pThis + 0x34);
}
#endif
