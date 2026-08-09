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
