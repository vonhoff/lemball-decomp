struct LargePoolSixDwords
{
	unsigned long m_values[6];
};

// FUNCTION: LEMBALL 0x0040F7E0
int __fastcall poll_managed_entity_stream_dirty_flag(void* pObject)
{
	char* pThis;
	int* pDirty;
	int nState;
	pThis = (char*)pObject;
	pDirty = (int*)(pThis + 0x164);

	if (((*(unsigned long*)(pThis + 0x194) ^ *(unsigned long*)(pThis + 0x9c)) & 0xfffff000UL) == 0 && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;
	if (((*(unsigned long*)(pThis + 0x198) ^ *(unsigned long*)(pThis + 0xa0)) & 0xfffff000UL) == 0 && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;
	if (((*(unsigned long*)(pThis + 0x19c) ^ *(unsigned long*)(pThis + 0xa4)) & 0xfffff000UL) == 0 && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;

	nState = *(int*)(pThis + 0xb8);
	if (nState == 0)
	{
		if (*(int*)(pThis + 0x1a4) == 0 && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
	}
	else if (nState != 1)
	{
		if (*(int*)(pThis + 0x1a4) == nState && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
		if (*(short*)(pThis + 0x1a8) == *(short*)(pThis + 0xbc) && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
		if (*(int*)(pThis + 0x1ac) == *(int*)(pThis + 0x94) && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
	}
	if (nState != 1)
		*(int*)(pThis + 0x1a4) = nState;

	if (*(int*)(pThis + 0x1b0) == *(int*)(pThis + 0x98) && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;

	*(unsigned long*)(pThis + 0x194) = *(unsigned long*)(pThis + 0x9c);
	*(short*)(pThis + 0x1a0) = *(short*)(pThis + 0xb4);
	*(unsigned long*)(pThis + 0x198) = *(unsigned long*)(pThis + 0xa0);
	*(unsigned long*)(pThis + 0x19c) = *(unsigned long*)(pThis + 0xa4);
	*(int*)(pThis + 0x1b0) = *(int*)(pThis + 0x98);
	*(short*)(pThis + 0x1a8) = *(short*)(pThis + 0xbc);
	*(int*)(pThis + 0x1ac) = *(int*)(pThis + 0x94);
	return *pDirty;
}

extern void __cdecl SelectionSortRecordsWithComparator(char*, char*, int, void*);

// FUNCTION: LEMBALL 0x00463960
void __cdecl VSQSort(char* pFirst, unsigned int nRecords, unsigned int nRecordSize, void* pComparator)
{
	char* pLeftStack[30];
	char* pRightStack[30];
	char* pRight;
	char* pScanLeft;
	char* pScanRight;
	unsigned int nPartitionRecords;
	int nStackDepth;

	if (nRecords < 2 || nRecordSize == 0)
		return;
	nStackDepth = 0;
	pRight = pFirst + (nRecords - 1) * nRecordSize;
partition:
	nPartitionRecords = (unsigned int)(pRight - pFirst) / nRecordSize + 1;
	if (nPartitionRecords <= 8)
	{
		SelectionSortRecordsWithComparator(pFirst, pRight, nRecordSize, pComparator);
		goto pop_partition;
	}
	((void(__cdecl*)(char*, char*, int))0x00463B20)(pFirst + (nPartitionRecords >> 1) * nRecordSize, pFirst, nRecordSize);
	pScanRight = pRight + nRecordSize;
	pScanLeft = pFirst;
scan_left:
	pScanLeft += nRecordSize;
	if (pScanLeft <= pRight && ((int(__cdecl*)(void*, void*))pComparator)(pScanLeft, pFirst) <= 0)
		goto scan_left;
scan_right:
	pScanRight -= nRecordSize;
	if (pScanRight > pFirst && ((int(__cdecl*)(void*, void*))pComparator)(pScanRight, pFirst) >= 0)
		goto scan_right;
	if (pScanLeft <= pScanRight)
	{
		((void(__cdecl*)(char*, char*, int))0x00463B20)(pScanLeft, pScanRight, nRecordSize);
		goto scan_left;
	}
	((void(__cdecl*)(char*, char*, int))0x00463B20)(pFirst, pScanRight, nRecordSize);
	if ((pScanRight - pFirst - 1) < (pRight - pScanLeft))
	{
		if (pScanLeft < pRight)
		{
			pLeftStack[nStackDepth] = pScanLeft;
			pRightStack[nStackDepth] = pRight;
			++nStackDepth;
		}
		if (pFirst + nRecordSize < pScanRight)
		{
			pRight = pScanRight - nRecordSize;
			goto partition;
		}
	}
	else
	{
		if (pFirst + nRecordSize < pScanRight)
		{
			pLeftStack[nStackDepth] = pFirst;
			pRightStack[nStackDepth] = pScanRight - nRecordSize;
			++nStackDepth;
		}
		pFirst = pScanLeft;
		if (pScanLeft < pRight)
			goto partition;
	}
pop_partition:
	--nStackDepth;
	if (nStackDepth < 0)
		return;
	pFirst = pLeftStack[nStackDepth];
	pRight = pRightStack[nStackDepth];
	goto partition;
}

// NOTE: 0x004766F0 unmatchable: natural clipped ZRLE decoder structure measured 15.20%.
// NOTE: 0x00476EE0 unmatchable: mirrored clipped decoder register/loop shape measured 7.06%.

// FUNCTION: LEMBALL 0x00468C50
void __fastcall update_buffered_geometry_child_fill_and_edges(void* pObject)
{
	char* pThis;
	char* pEntry;
	int nTarget;
	int nOffset;
	int nFill;
	int nEdge;
	int nActive;
	short nWidth;
	short nHeight;

	pThis = (char*)pObject;
	nTarget = *(int*)(*(int*)(pThis + 0x4c) + 0x0c);
	((void(__fastcall*)(void*))(*(void***)(*(int*)(*(int*)(nTarget + 0x40) + 4) + nTarget + 0x40))[0x38 / 4])(
		(void*)(*(int*)(*(int*)(nTarget + 0x40) + 4) + nTarget + 0x40));

	pEntry = *(char**)(pThis + 0x10c);
	*(short*)(pEntry + 4) = *(short*)(pThis + 0xc0);
	*(short*)(pEntry + 6) = *(short*)(pThis + 0xc2);
	*(short*)(pEntry + 8) = 0;
	*(short*)(pEntry + 0x0a) = 0;
	*(int*)(pEntry + 0x0c) = *(int*)(pThis + 0x114);
	((void(__fastcall*)(void*, int, void*))(*(void***)pEntry)[1])(
		pEntry, 0, *(void**)(pThis + 0x4c));

	nActive = (*(int*)(pThis + 0xd0) != 0 && *(int*)(pThis + 0x94) != 0);
	if (nActive)
	{
		nEdge = 0xf8;
		nFill = 0xff;
	}
	else
	{
		nEdge = 0xff;
		nFill = 0xf8;
	}

	pEntry = *(char**)(pThis + 0x110);
	nWidth = *(short*)(pThis + 0xc0);
	nHeight = *(short*)(pThis + 0xc2);
	*(short*)(pEntry + 4) = 0;
	*(short*)(pEntry + 6) = 0;
	*(short*)(pEntry + 8) = nWidth - 1;
	*(short*)(pEntry + 0x0a) = 0;
	*(int*)(pEntry + 0x0c) = nEdge;

	*(short*)(pEntry + 0x14) = 0;
	*(short*)(pEntry + 0x16) = 0;
	*(short*)(pEntry + 0x18) = 0;
	*(short*)(pEntry + 0x1a) = nHeight - 1;
	*(int*)(pEntry + 0x1c) = nEdge;

	*(short*)(pEntry + 0x24) = nWidth - 1;
	*(short*)(pEntry + 0x26) = 0;
	*(short*)(pEntry + 0x28) = nWidth - 1;
	*(short*)(pEntry + 0x2a) = nHeight - 1;
	*(int*)(pEntry + 0x2c) = nFill;

	*(short*)(pEntry + 0x34) = 0;
	*(short*)(pEntry + 0x36) = nHeight - 1;
	*(short*)(pEntry + 0x38) = nWidth - 1;
	*(short*)(pEntry + 0x3a) = nHeight - 1;
	*(int*)(pEntry + 0x3c) = nFill;

	nOffset = 0;
	do
	{
		pEntry = *(char**)(pThis + 0x110) + nOffset;
		nOffset += 0x10;
		((void(__fastcall*)(void*, int, void*))(*(void***)pEntry)[1])(
			pEntry, 0, *(void**)(pThis + 0x4c));
	} while (nOffset < 0x40);
}

// FUNCTION: LEMBALL 0x00469210
void __fastcall layout_resource_sprite_text_owner(void* pObject, int, char* pPrimaryText, char* pSecondaryText)
{
	char* pThis;
	short aPrimaryExtent[4];
	short aSecondaryExtent[4];
	short aRect[4];
	short aPrimaryPosition[2];
	short aSecondaryPosition[2];
	void* pChild;

	pThis = (char*)pObject;
	*(char**)(pThis + 0x11c) = pSecondaryText;
	*(char**)(pThis + 0x118) = pPrimaryText;
	((void(__fastcall*)(void*, int, short*, char*, int))0x0045DB30)(
		*(void**)(pThis + 0x134), 0, aPrimaryExtent, pPrimaryText, 0x20);
	((void(__fastcall*)(void*, int, short*))0x00469120)(pThis, 0, aPrimaryExtent);

	aSecondaryExtent[0] = aPrimaryExtent[0];
	aSecondaryExtent[1] = aPrimaryExtent[1];
	if (pSecondaryText != 0)
	{
		((void(__fastcall*)(void*, int, short*, char*, int))0x0045DB30)(
			*(void**)(pThis + 0x134), 0, aSecondaryExtent, pSecondaryText, 0x20);
		((void(__fastcall*)(void*, int, short*))0x00469120)(pThis, 0, aSecondaryExtent);
	}

	((void(__fastcall*)(void*, int, short*, short*))0x00469180)(
		pThis, 0, (short*)(pThis + 0x12c), aPrimaryPosition);
	if (pSecondaryText == 0)
	{
		*(char**)(pThis + 0x11c) = pPrimaryText;
		*(short*)(pThis + 0x128) = *(short*)(pThis + 0x12c);
		*(short*)(pThis + 0x12a) = *(short*)(pThis + 0x12e);
	}
	else
	{
		((void(__fastcall*)(void*, int, short*, short*))0x00469180)(
			pThis, 0, (short*)(pThis + 0x128), aSecondaryPosition);
	}
	++*(short*)(pThis + 0x128);
	++*(short*)(pThis + 0x12a);

	if (*(int*)(pThis + 0x13c) == 0)
	{
		aRect[0] = *(short*)(pThis + 0xc0);
		aRect[1] = *(short*)(pThis + 0xc2);
		aRect[2] = *(short*)(pThis + 0xdc);
		aRect[3] = *(short*)(pThis + 0xde);
		((void(__fastcall*)(void*, int, short*, void*, const char*))(*(void***)pThis)[1])(
			pThis, 0, aRect, *(void**)(pThis + 0xc8), (const char*)0x0049F02C);
		pChild = pThis != 0 ? pThis + 0x90 : 0;
		*(short*)(pThis + 0xc4) += *(short*)(pThis + 0x18);
		*(short*)(pThis + 0xc6) += *(short*)(pThis + 0x1a);
		((void(__fastcall*)(void*, int, void*))0x0046A9A0)(
			*(void**)(*(int*)(pThis + 0xc8) + 0x1c), 0, pChild);
		((void(__fastcall*)(void*, int, int))0x0046A530)(pThis + 0x90, 0, 1);
		*(int*)(pThis + 0x13c) = 1;
	}
	*(int*)(pThis + 0xd8) = 1;
}
