#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_002)
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

	if (nRecords < 2 || nRecordSize == 0) {
		return;
	}
	nStackDepth = 0;
	pRight = pFirst + (nRecords - 1) * nRecordSize;
partition:
	nPartitionRecords = (unsigned int) (pRight - pFirst) / nRecordSize + 1;
	if (nPartitionRecords <= 8) {
		SelectionSortRecordsWithComparator(pFirst, pRight, nRecordSize, pComparator);
		goto pop_partition;
	}
	((void(__cdecl*)(char*, char*, int)) 0x00463B20)(pFirst + (nPartitionRecords >> 1) * nRecordSize,
													 pFirst,
													 nRecordSize);
	pScanRight = pRight + nRecordSize;
	pScanLeft = pFirst;
scan_left:
	pScanLeft += nRecordSize;
	if (pScanLeft <= pRight && ((int(__cdecl*)(void*, void*)) pComparator)(pScanLeft, pFirst) <= 0) {
		goto scan_left;
	}
scan_right:
	pScanRight -= nRecordSize;
	if (pScanRight > pFirst && ((int(__cdecl*)(void*, void*)) pComparator)(pScanRight, pFirst) >= 0) {
		goto scan_right;
	}
	if (pScanLeft <= pScanRight) {
		((void(__cdecl*)(char*, char*, int)) 0x00463B20)(pScanLeft, pScanRight, nRecordSize);
		goto scan_left;
	}
	((void(__cdecl*)(char*, char*, int)) 0x00463B20)(pFirst, pScanRight, nRecordSize);
	if ((pScanRight - pFirst - 1) < (pRight - pScanLeft)) {
		if (pScanLeft < pRight) {
			pLeftStack[nStackDepth] = pScanLeft;
			pRightStack[nStackDepth] = pRight;
			++nStackDepth;
		}
		if (pFirst + nRecordSize < pScanRight) {
			pRight = pScanRight - nRecordSize;
			goto partition;
		}
	}
	else {
		if (pFirst + nRecordSize < pScanRight) {
			pLeftStack[nStackDepth] = pFirst;
			pRightStack[nStackDepth] = pScanRight - nRecordSize;
			++nStackDepth;
		}
		pFirst = pScanLeft;
		if (pScanLeft < pRight) {
			goto partition;
		}
	}
pop_partition:
	--nStackDepth;
	if (nStackDepth < 0) {
		return;
	}
	pFirst = pLeftStack[nStackDepth];
	pRight = pRightStack[nStackDepth];
	goto partition;
}
#endif
