struct ShortPoint {
	short x;
	short y;
};

static __inline void QueueRenderEntry(char* pEntry, void* pOwner)
{
	((void (__fastcall*)(void*, int, void*)) (*(void***) pEntry)[1])(pEntry, 0, pOwner);
}

// FUNCTION: LEMBALL 0x00442690
void __fastcall rebuild_level_status_panel_render_entries(void* pObject)
{
	char* p;
	char* pState;
	char* pSprite;
	char* pAltSprite;
	char* pEntry;
	void* pOwner;
	ShortPoint source;
	ShortPoint destination;
	ShortPoint extraSource;
	ShortPoint extraDestination;
	unsigned int nColor;
	int nColorOffset;
	int nMode;
	int nIndex;
	int nSpriteIndex;

	p = (char*) pObject;
	pState = *(char**) (p + 0x110);
	pSprite = 0;
	nSpriteIndex = *(int*) (pState + 0x14);
	if (nSpriteIndex < 4) {
		pSprite = *(char**) (*(char**) (*(char**) (pState + 0x0c) + 0x10) + 0x64 + nSpriteIndex * 4);
	}
	pAltSprite = 0;
	nSpriteIndex = *(int*) (pState + 0x18);
	if (nSpriteIndex != -1 && nSpriteIndex < 4) {
		pAltSprite = *(char**) (*(char**) (*(char**) (pState + 0x0c) + 0x10) + 0x64 + nSpriteIndex * 4);
	}
	nMode = 0;
	if (*(int*) (p + 0x104) == 0 || *(int*) (p + 0x168) != 0) {
		nMode = 1;
	}
	pOwner = *(void**) (p + 0x4c);
	{
		char* pContext = *(char**) ((char*) pOwner + 0x0c);
		char* pChild = *(char**) (pContext + 0x40);
		char* pSink = *(char**) (pChild + 4) + (int) pContext + 0x40;
		((void (__fastcall*)(void*)) (*(void***) pSink)[14])(pSink);
	}

	*(short*) (p + 0x17c) = *(short*) (p + 0x158);
	*(short*) (p + 0x17e) = *(short*) (p + 0x15a);
	*(int*) (p + 0x194) = *(int*) (*(char**) (pState + 0x0c) + 0x34);
	*(int*) (p + 0x190) = nMode;
	*(int*) (p + 0x184) = 0;
	*(char**) (p + 0x188) = pSprite;
	QueueRenderEntry(p + 0x178, pOwner);

	if (*(int*) (pState + 0x18) != -1 && *(int*) (p + 0x168) == 0) {
		*(short*) (p + 0x19c) = *(short*) (p + 0x160);
		*(short*) (p + 0x19e) = *(short*) (p + 0x162);
		*(int*) (p + 0x1b4) = *(int*) (*(char**) (pState + 0x0c) + 0x3c);
	}
	else {
		nMode = *(int*) (p + 0x168) != 0 ? 2 : nMode;
		if (*(int*) (p + 0x16c) != 0) {
			nMode += 3;
		}
		*(short*) (p + 0x19c) = *(short*) (p + 0x160);
		*(short*) (p + 0x19e) = *(short*) (p + 0x162);
		*(int*) (p + 0x1b4) = *(int*) (*(char**) (pState + 0x0c) + 0x38);
	}
	*(int*) (p + 0x1b0) = nMode;
	*(int*) (p + 0x1a4) = 0;
	*(char**) (p + 0x1a8) = pAltSprite;
	QueueRenderEntry(p + 0x198, pOwner);

	source.x = (short) ((*(unsigned short*) (*(char**) (pState + 8) + 0x228) * 27) / 50);
	source.y = 9;
	destination.x = 7;
	destination.y = 11;
	extraSource.x = 6;
	extraSource.y = 4;
	extraDestination.x = 7;
	extraDestination.y = 4;
	nColorOffset = 0x76;
	if (*(int*) (p + 0x104) == 0 || *(int*) (p + 0x168) != 0) {
		nColorOffset = 0x45;
		++destination.x;
		++destination.y;
		++extraDestination.x;
		++extraDestination.y;
	}
	if (pSprite != 0) {
		nColor = *(unsigned char*) (*(char**) pSprite + nColorOffset);
	}
	else {
		nColor = *(unsigned int*) &source;
	}
	((void (__fastcall*)(void*, int, ShortPoint*)) 0x00402248)(p + 0x118, 0, &source);
	((void (__fastcall*)(void*, int, ShortPoint*)) 0x00402004)(p + 0x11c, 0, &destination);
	*(unsigned int*) (p + 0x120) = nColor;
	QueueRenderEntry(p + 0x114, pOwner);

	if (*(int*) (p + 0x168) == 0) {
		nIndex = 0;
		if (*(int*) (pState + 0x1c) > 0) {
			pEntry = p + 0x124;
			do {
				nSpriteIndex = *(int*) (pState + 0x20 + nIndex * 4);
				pSprite = nSpriteIndex < 4 ? *(char**) (*(char**) (*(char**) (pState + 0x0c) + 0x10) + 0x64 + nSpriteIndex * 4) : 0;
				if (pSprite != 0) {
					nColor = *(unsigned char*) (*(char**) pSprite + nColorOffset);
				}
				else {
					nColor = (unsigned int) nColorOffset;
				}
				((void (__fastcall*)(void*, int, ShortPoint*)) 0x00402248)(pEntry + 4, 0, &extraSource);
				((void (__fastcall*)(void*, int, ShortPoint*)) 0x00402004)(pEntry + 8, 0, &extraDestination);
				*(unsigned int*) (pEntry + 0x0c) = nColor;
				QueueRenderEntry(pEntry, pOwner);
				pEntry += 0x10;
				extraDestination.x += 11;
				++nIndex;
			} while (nIndex < *(int*) (pState + 0x1c));
		}
	}
}
