#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_014)
#include <string.h>

// FUNCTION: LEMBALL 0x00450160
void __fastcall layout_password_entry_text(char* pThis)
{
	void* pFont;
	char** pTable;
	char* pSource;
	char* pSeparator;
	char* pSplit;
	short aExtent[4];
	short nTop;
	short nLeft;
	short nHeight;
	char* pPreviousSplit;
	int* pLayout;
	pFont = ((void*(__fastcall*) (void*, int, int) ) 0x469ef0)(*(void**) (pThis + 0x98), 0, *(int*) (pThis + 0x384));
	if (*(int*) (pThis + 0x8c)) {
		pTable = *(int*) (pThis + 0x518) ? (char**) 0x49fb58 : (char**) 0x49fb98;
	}
	else {
		pTable = *(int*) (pThis + 0x518) ? (char**) 0x49fb38 : (char**) 0x49fb78;
	}
	pSource = pTable[*(int*) (*(char**) 0x49cb68 + 0x0c)];
	pSeparator = strchr(pSource, '#');
	if (!pSeparator) {
		strcpy(pThis + 0x3e0, pSource);
	}
	else {
		if (pSeparator != pSource) {
			strncpy(pThis + 0x3e0, pSource, pSeparator - pSource);
		}
		pThis[0x3e0 + pSeparator - pSource] = 0;
		if (*(int*) 0x4a011c) {
			char* pPeer =
				(char*) ((void*(__fastcall*) (void*, int, int) ) 0x402a31)(*(void**) 0x4a0120, 0, *(int*) 0x4a011c);
			strcat(pThis + 0x3e0, pPeer + 0x2c);
		}
		strcat(pThis + 0x3e0, pSeparator + 1);
	}
	pLayout = *(int**) (pThis + 0x510);
	nLeft = *(short*) ((char*) pLayout + 0x48);
	nTop = *(short*) ((char*) pLayout + 0x4c);
	*(char**) (pThis + 0x4e0) = pThis + 0x3e0;
	*(char**) (pThis + 0x4e4) = 0;
	for (;;) {
		short* pSize = ((short*(__fastcall*) (void*, int, short*, char*, int) ) 0x45db30)(pFont,
																						  0,
																						  aExtent,
																						  *(char**) (pThis + 0x4e0),
																						  0x20);
		nHeight = pSize[1];
		nLeft = (short) pLayout[14] + (short) ((pLayout[16] - pSize[0]) / 2);
		pPreviousSplit = *(char**) (pThis + 0x4e4);
		if (pPreviousSplit) {
			--pPreviousSplit;
		}
		if (nLeft >= *(short*) ((char*) pLayout + 0x48)) {
			break;
		}
		pSplit = strrchr(*(char**) (pThis + 0x4e0), ' ');
		*(char**) (pThis + 0x4e4) = pSplit;
		*pSplit = 0;
		*(char**) (pThis + 0x4e4) = pSplit + 1;
		if (pPreviousSplit) {
			*pPreviousSplit = ' ';
		}
	}
	*(short*) (pThis + 0x4e8) = nLeft;
	*(short*) (pThis + 0x4ea) = nTop;
	if (!*(char**) (pThis + 0x4e4)) {
		*(short*) (pThis + 0x4ea) = nTop + nHeight / 2;
	}
	else {
		short* pSize;
		nTop += nHeight;
		pSize = ((short*(__fastcall*) (void*, int, short*, char*, int) ) 0x45db30)(pFont,
																				   0,
																				   aExtent,
																				   *(char**) (pThis + 0x4e4),
																				   0x20);
		*(short*) (pThis + 0x4ec) = (short) pLayout[14] + (short) ((pLayout[16] - pSize[0]) / 2);
		*(short*) (pThis + 0x4ee) = nTop;
	}
	{
		short* pSize = ((
			short*(__fastcall*) (void*, int, short*, char*, int) ) 0x45db30)(pFont, 0, aExtent, (char*) 0x49fc88, 0x20);
		short nPasswordHeight = pSize[1];
		short nPasswordTop = *(short*) ((char*) pLayout + 0x64);
		*(short*) (pThis + 0x4f0) = (short) pLayout[14] + (short) ((pLayout[16] - pSize[0]) / 2);
		*(short*) (pThis + 0x4f2) = nPasswordTop;
		pSize = ((short*(__fastcall*) (void*, int, short*, char*, int) ) 0x45db30)(pFont,
																				   0,
																				   aExtent,
																				   *(char**) (pThis + 0x4f8),
																				   0x20);
		*(short*) (pThis + 0x4f4) = (short) pLayout[14] + (short) ((pLayout[16] - pSize[0]) / 2);
		*(short*) (pThis + 0x4f6) = nPasswordTop + nPasswordHeight;
	}
}
#endif
