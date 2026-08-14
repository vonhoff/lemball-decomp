#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_009)
struct LargePoolLevelCoord {
	int x;
	int y;
	int z;
};

// FUNCTION: LEMBALL 0x0040ca40
void __fastcall load_network_player_chunk_markers(void* pThis,
												  int nEdxSlop,
												  unsigned short* pData,
												  int nUnused,
												  int fExisting)
{
	typedef unsigned short(__cdecl * FindFreeSlotProc)(void);
	typedef unsigned short(__fastcall * SampleHeightProc)(void*, int, int, int);
	typedef void(__fastcall * AppendTrapDoorProc)(void*, int, unsigned short, LargePoolLevelCoord*, int, int);
	typedef void(__fastcall * SetTripletProc)(void*, int, int, int, int, int);
	typedef void(__fastcall * ConfigureRangesProc)(void*, int, int, int, int, int, int);
	unsigned int nCount;
	unsigned int anRanges[4];
	unsigned short* pEntry;
	LargePoolLevelCoord Position;
	unsigned short nSlot;
	void* pGrid;
	void* pOwner;
	int nTileX;
	int nTileY;
	int nIndex;

	nCount = *pData++;
	anRanges[0] = 0;
	anRanges[1] = 0;
	anRanges[2] = 0;
	anRanges[3] = 0;
	pEntry = pData;
	pOwner = *(void**) 0x004A74B0;

	for (nIndex = 0; nIndex < (int) nCount; ++nIndex) {
		if (fExisting == 0) {
			nSlot = ((FindFreeSlotProc) 0x0040227A)();
		}

		Position.x = (unsigned int) pEntry[0] << 12;
		Position.y = (unsigned int) pEntry[1] << 12;
		Position.z = 0xaa55aa55;
		pGrid = *(void**) 0x004A74B4;
		nTileX = Position.x >> 12;
		nTileY = Position.y >> 12;
		if (nTileX >= 0 && nTileY >= 0 && (nTileX >> 4) < *(int*) ((char*) pGrid + 0x10) &&
			(nTileY >> 4) < *(int*) ((char*) pGrid + 0x14)) {
			Position.z = (unsigned int) ((SampleHeightProc) 0x004029A5)(
							 (char*) *(void**) ((char*) pGrid + 0x0c) +
								 ((nTileY >> 4) * *(int*) ((char*) pGrid + 0x10) + (nTileX >> 4)) * 12,
							 0,
							 nTileX & 0x0f,
							 nTileY & 0x0f)
						 << 12;
		}
		else {
			Position.z = 0;
		}

		anRanges[nIndex] = pEntry[3];
		if (fExisting == 0) {
			((AppendTrapDoorProc) 0x00401393)(pThis, 0, nSlot, &Position, 1, 0);
		}
		((SetTripletProc) 0x0040107D)(pOwner, 0, Position.x >> 12, Position.y >> 12, Position.z >> 12, nIndex);
		pEntry += 4;
	}

	((ConfigureRangesProc) 0x00401D25)(pOwner, 0, nCount, anRanges[0], anRanges[1], anRanges[2], anRanges[3]);
	(void) nEdxSlop;
	(void) nUnused;
}
#endif
