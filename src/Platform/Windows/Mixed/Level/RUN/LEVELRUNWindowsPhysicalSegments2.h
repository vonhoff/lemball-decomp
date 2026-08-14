#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_014)
// FUNCTION: LEMBALL 0x00421440
int __fastcall AppendNodeChunkRecordFromLevelData(void* pObject, int nUnused, int param_2, int param_3)
{
	int iVar1 = *(int*) ((char*) pObject + 4);
	*(int*) ((char*) pObject + 4) = iVar1 + 1;
	((void(__fastcall*)(void*, int, int, int)) 0x402572)((void*) (iVar1 * 0x14 + *(int*) pObject), param_2, param_3, 0);
	return iVar1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_017)
// FUNCTION: LEMBALL 0x004131b0
void __fastcall SetLevelModeProfileRangeSelector(void* pObject, int nUnused, int param_2)
{
	*(int*) ((char*) pObject + 0xd0) = param_2;
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x401d25)(pObject,
																   param_2,
																   *(int*) ((char*) pObject + 0xc0),
																   *(int*) ((char*) pObject + 0xc4),
																   *(int*) ((char*) pObject + 0xc8),
																   *(int*) ((char*) pObject + 0xcc));
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_034)
// FUNCTION: LEMBALL 0x00413130
void __fastcall ConfigureLevelModeManagedEntityProfiles(void* pObject,
														int nUnused,
														int param_2,
														int param_3,
														int param_4,
														int param_5,
														int param_6)
{
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x402720)(*(void**) ((char*) pObject + 0x15c),
																   param_2,
																   param_3,
																   param_4,
																   param_5,
																   param_6);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_056)
// FUNCTION: LEMBALL 0x00412e80
void __fastcall AppendType18ObjectWithFreeSlot(void* pObject, int nUnused, void* param_2, int param_3)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	AppendType18ChunkObject(*(void**) ((char*) pObject + 0x1c4), nUnused, (unsigned short) nSlot, param_2, 0, param_3);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_058)
// FUNCTION: LEMBALL 0x0040b290
int __fastcall RouteLevelChunkStreamPayload(void* pObject, int nUnused, void* param_1)
{
	unsigned short uVar1 = *(unsigned short*) (*(int*) ((char*) param_1 + 4) + 8);
	if (uVar1 < 0xb) {
		return 0;
	}
	((void(__cdecl*)(void*, void*)) 0x45f280)(
		*(void**) (*(int*) ((char*) pObject + 0x10) +
				   *(int*) (*(int*) ((char*) pObject + 0x1c) + (unsigned int) uVar1 * 4 - 0x2c) * 4),
		(void*) (*(int*) ((char*) param_1 + 4) + 0x10));
	*(int*) ((char*) param_1 + 0x24) = 0;
	return 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_061)
// FUNCTION: LEMBALL 0x0042a460
void __fastcall DeactivateBoonChunkObjectInManager(void* param_1, int nUnused, int param_2)
{
	unsigned short uVar1;
	int iVar2;
	switch (*(int*) ((char*) param_2 + 0x64)) {
	case 0x28:
		uVar1 = 0xfffe;
		iVar2 = 0;
		break;
	case 0x2a:
		uVar1 = 0xfffd;
		iVar2 = 1;
		break;
	case 0x2c:
		uVar1 = 0xfffb;
		iVar2 = 2;
		break;
	case 0x2e:
		uVar1 = 0xfff7;
		iVar2 = 3;
		break;
	default:
		return;
	}
	*(unsigned short*) param_1 = *(unsigned short*) param_1 & uVar1;
	*(int*) (*(int*) ((char*) param_1 + iVar2 * 2 + 0x1a) + 0x124) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_069)
// FUNCTION: LEMBALL 0x0040d230
void __fastcall RemoveAnimChunkRecordsAtTile(void* pThis, int nUnused, short* pTile)
{
	int i = 0;
	while (i < *(int*) ((char*) pThis + 4)) {
		if (*(short*) ((char*) pThis + 8 + i * 0x18) == pTile[0] &&
			*(short*) ((char*) pThis + 8 + i * 0x18 + 2) == pTile[1]) {
			int k;
			for (k = i + 1; k < *(int*) ((char*) pThis + 4); k++) {
				memmove((char*) pThis + (k - 1) * 0x18, (char*) pThis + k * 0x18, 0x18);
			}
			*(int*) ((char*) pThis + 4) = *(int*) ((char*) pThis + 4) - 1;
		}
		i++;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_072)
// FUNCTION: LEMBALL 0x00417150
void __cdecl CGlobalGameObjectDeleteMessages(void)
{
	void* p;
	p = *(void**) 0x49d130;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d12c;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d128;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d110;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d114;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d118;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d11c;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d120;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d124;
	if (p != 0) {
		((void(__fastcall*)(void*, int))(*(void***) p)[5])(p, 1);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_074)
// Minimal views so the level-mode stream loaders emit direct `call` to the
// already-reconstructed members (matches orig direct calls to 0x45f250/0x45f280).
struct CNetworkMessage {
	int SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange);
};
struct GameEffStream {
	int LoadEffStreamFromMemory(int nSourceBuffer);
};
// FUNCTION: LEMBALL 0x00412b80
void __fastcall LoadLevelModeManagerStreamRanges0x1b4(void* pThis)
{
	int* pMgr = (int*) ((char*) pThis + 0x1b4);
	unsigned int uMask;
	unsigned int uBase;
	int count = 4;
	{
		unsigned int v = (unsigned int) ((char*) pThis - 0x1c);
		uMask = v >= 1 ? 0xffffffffu : 0u;
		uBase = uMask & (unsigned int) pThis;
	}
	do {
		int mgr = *pMgr++;
		((CNetworkMessage*) (mgr + 0x138))->SaveEffStreamToMemoryRange(*(int*) (uBase + 0x1c), 0);
		*(int*) (uBase + 0x1c) += *(int*) (mgr + 0x154) - *(int*) (mgr + 0x140);
	} while (--count != 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_075)
// FUNCTION: LEMBALL 0x00412be0
void __fastcall LoadLevelModeManagerStreams0x1c4(void* pThis)
{
	int* pMgr = (int*) ((char*) pThis + 0x1c4);
	unsigned int uMask;
	unsigned int uBase;
	int count = 4;
	{
		unsigned int v = (unsigned int) ((char*) pThis - 0x1c);
		uMask = v >= 1 ? 0xffffffffu : 0u;
		uBase = uMask & (unsigned int) pThis;
	}
	do {
		int mgr = *pMgr++;
		int r = ((GameEffStream*) (mgr + 0x138))->LoadEffStreamFromMemory(*(int*) (uBase + 0x20));
		if (r != 0) {
			*(int*) (uBase + 0x20) = *(int*) (mgr + 0x158);
		}
	} while (--count != 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_082)
// FUNCTION: LEMBALL 0x00420d30
void __fastcall remove_cached_enmy_chunk_object_and_destroy(void* pThis, int nUnusedEdx, void* pObject)
{
	void* pOwner;
	void** ppItems;
	void** ppVtbl;
	int nCount;
	int nIndex;
	int nByte;

	(void) nUnusedEdx;
	pOwner = g_pCachedChunkManagerLevelMode;
	nCount = *(int*) ((char*) pOwner + 0x118);
	nIndex = 0;
	if (nCount > 0) {
		ppItems = *(void***) ((char*) pOwner + 0x120);
		do {
			if (ppItems[nIndex] == pObject) {
				nCount = nCount - 1;
				*(int*) ((char*) pOwner + 0x118) = nCount;
				if (nIndex < nCount) {
					nByte = nIndex * 4;
					while (nIndex < *(int*) ((char*) pOwner + 0x118)) {
						nIndex = nIndex + 1;
						*(int*) ((char*) ppItems + nByte) = *(int*) ((char*) ppItems + nByte + 4);
						nByte = nByte + 4;
					}
				}
				*(void**) ((char*) ppItems + nCount * 4) = 0;
				break;
			}
			nIndex = nIndex + 1;
		} while (nIndex < nCount);
	}
	((void(__fastcall*)(void*, int, void*))(*(void***) pThis)[0x44 / 4])(pThis, 0, pObject);
	ppVtbl = *(void***) pObject;
	((void(__fastcall*)(void*, int)) ppVtbl[0xc8 / 4])(pObject, 0);
	if (pObject != 0) {
		((void(__fastcall*)(void*, int, unsigned char)) ppVtbl[0])(pObject, 0, 1);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_083)
struct CountedU16Record {
	unsigned int m_nType00;
	unsigned int m_cValues04;
	unsigned int m_nFlags08;
	int m_nSignedValue0C;
	unsigned short* m_pValues10;
};

// FUNCTION: LEMBALL 0x00420f90
unsigned char* __fastcall ParseCountedU16RecordFromStream(void*,
														  int,
														  unsigned char* pStream,
														  CountedU16Record** ppRecord)
{
	unsigned int cValues;
	unsigned char bSigned;
	unsigned char* pSource;
	int nOffset;
	int cRemaining;

	((void(__cdecl*)(const unsigned char*)) 0x40168b)(pStream);
	pStream += 4;
	*ppRecord = (CountedU16Record*) AllocateVSMemBlock(sizeof(CountedU16Record));
	cValues = pStream[1];
	(*ppRecord)->m_nType00 = pStream[0];
	(*ppRecord)->m_cValues04 = cValues;
	(*ppRecord)->m_nFlags08 = pStream[2];
	bSigned = pStream[3];
	if ((bSigned & 0x80) != 0) {
		(*ppRecord)->m_nSignedValue0C = (int) bSigned | ~0xff;
	}
	else {
		(*ppRecord)->m_nSignedValue0C = bSigned;
	}
	(*ppRecord)->m_pValues10 = (unsigned short*) AllocateVSMemBlock(cValues * sizeof(unsigned short));
	if (cValues > 0) {
		pSource = pStream + 4;
		nOffset = 0;
		cRemaining = cValues;
		do {
			unsigned short nValue = *(unsigned short*) pSource;
			pSource += 2;
			nOffset += 2;
			--cRemaining;
			*(unsigned short*) ((char*) (*ppRecord)->m_pValues10 + nOffset - 2) = nValue;
		} while (cRemaining != 0);
	}
	return pStream + cValues * 2 + 4;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_085)
// FUNCTION: LEMBALL 0x0042a3a0
void __fastcall activate_boon_chunk_object_by_subtype_at_position(void* pThis,
																  int nUnusedEdx,
																  int posX,
																  int posY,
																  int posZ,
																  int subtype)
{
	unsigned short nFlag;
	int nIndex;
	void* pBoonObject;

	(void) nUnusedEdx;
	switch (subtype) {
	case 0x28:
		nFlag = 1;
		nIndex = 0;
		break;
	case 0x2a:
		nFlag = 2;
		nIndex = 1;
		break;
	case 0x2c:
		nFlag = 4;
		nIndex = 2;
		break;
	case 0x2e:
		nFlag = 8;
		nIndex = 3;
		break;
	default:
		return;
	}
	*(unsigned short*) pThis = *(unsigned short*) pThis | nFlag;
	*(int*) ((char*) pThis + nIndex * 12 + 4) = posX << 12;
	*(int*) ((char*) pThis + nIndex * 12 + 8) = posY << 12;
	*(int*) ((char*) pThis + nIndex * 12 + 12) = posZ << 12;
	pBoonObject = *(void**) ((char*) pThis + nIndex * 4 + 0x34);
	*(int*) ((char*) pBoonObject + 0x9c) = posX << 12;
	*(int*) ((char*) pBoonObject + 0xa0) = posY << 12;
	*(int*) ((char*) pBoonObject + 0xa4) = posZ << 12;
	*(int*) ((char*) pBoonObject + 0x124) = 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_086)
// FUNCTION: LEMBALL 0x004166d0
unsigned short collect_free_managed_entity_slot_ids(unsigned short* pOut, int nLimit)
{
	extern unsigned char g_GAME_ManagedEntitySlotBitMasks[8];
	extern unsigned char g_GAME_ManagedEntitySlotClaimBitset[0x100];
	int nCount = 0;
	for (int nSlot = 0; nSlot < 0x100; ++nSlot) {
		if (g_GAME_ManagedEntitySlotClaimBitset[nSlot] == 0xff) {
			continue;
		}
		for (int nMask = 0; nMask < 8; ++nMask) {
			if ((g_GAME_ManagedEntitySlotClaimBitset[nSlot] & g_GAME_ManagedEntitySlotBitMasks[nMask]) != 0) {
				continue;
			}
			pOut[nCount++] = (unsigned short) ((nSlot << 3) | nMask);
			if (nCount == nLimit) {
				return (unsigned short) nLimit;
			}
		}
	}
	return (unsigned short) nCount;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_092)
// FUNCTION: LEMBALL 0x00412740
void __fastcall CollectLevelModeManagerEntries(void* pLevelModeManager, int nUnused, int* pOutCount)
{
	int nFirstCount;
	*pOutCount = 0;
	nFirstCount =
		(*(int(__fastcall**)(void*, int, int*))(*(void***) ((char*) pLevelModeManager + 0x170) +
												0x12))((void*) ((char*) pLevelModeManager + 0x170), 0, pOutCount + 1);
	*pOutCount = nFirstCount;
	*(int*) pOutCount += (*(int(__fastcall**)(void*, int, int*))(
		*(void***) ((char*) pLevelModeManager + 0x15c) +
		0x12))((void*) ((char*) pLevelModeManager + 0x15c), 0, pOutCount + nFirstCount + 1);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_143)
// FUNCTION: LEMBALL 0x00412800
void* __fastcall find_counted_level_child_containing_point(void* this_, int nUnused, int x, int y, int z)
{
	int pt[3];
	((void(__fastcall*)(int*, int*)) 0x0040161d)(pt, &x);
	return ((void*(__fastcall*) (void*, int, int) ) 0x00403206)(*(void**) ((char*) this_ + 0x158), pt[0], pt[1]);
	(void) y;
	(void) z;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_144)
// FUNCTION: LEMBALL 0x00412830
void* __fastcall find_level_mode_managed_entity_at_point_by_type(void* this_,
																 int nUnused,
																 int x,
																 int y,
																 int z,
																 int type)
{
	int pt[3];
	((void(__fastcall*)(int*, int*)) 0x0040161d)(pt, &x);
	return ((void*(__fastcall*) (void*, int, int, int, int) ) 0x00402504)(*(void**) ((char*) this_ + 0x158),
																		  pt[0],
																		  pt[1],
																		  pt[2],
																		  type);
	(void) y;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_154)
// Minimal view of GameLevelProgressState (links to CGame.cpp def; member mangling via name+signature).
struct GameLevelProgressState {
	int ValidateAndApplyPassword(char* pszPassword);
	void Snapshot(void);
};

// FUNCTION: LEMBALL 0x0040d130
void __fastcall AppendAnimChunkRecordIfUnique(void* pObject,
											  int nUnusedEdx,
											  short* pTile,
											  unsigned short nFirstFrame,
											  unsigned short nLastFrame)
{
	int nCount;
	int nIndex;
	int nRow;
	int nColumn;
	unsigned short* pWord;

	// ILT has_anim_chunk_record_at_tile (0x40291e) forwards to HasAnimChunkRecordAtTile (0x40d080).
	if (((int(__fastcall*)(void*, int, short*)) 0x0040291e)(pObject, 0, pTile) == 0) {
		nCount = *(int*) ((char*) pObject + 4);
		if (nCount < 200) {
			*(int*) ((char*) pObject + nCount * 0x18 + 0x1c) = 1;
			nIndex = *(int*) ((char*) pObject + 4);
			*(int*) ((char*) pObject + nIndex * 0x18 + 8) = *(int*) pTile;
			*(short*) ((char*) pObject + nIndex * 0x18 + 0x0c) = pTile[2];
			*(unsigned short*) ((char*) pObject + nCount * 0x18 + 0x14) = nFirstFrame;
			*(unsigned short*) ((char*) pObject + nCount * 0x18 + 0x16) = nFirstFrame;
			*(unsigned short*) ((char*) pObject + nCount * 0x18 + 0x18) = nLastFrame;
			*(short*) ((char*) pObject + nCount * 0x18 + 0x1a) = (nLastFrame < nFirstFrame) ? -1 : 1;
			nRow = ((int) (short) pTile[1] + ((int) (short) pTile[1] >> 31 & 0x0f)) >> 4;
			nColumn = ((int) (short) pTile[0] + ((int) (short) pTile[0] >> 31 & 0x0f)) >> 4;
			*(int*) ((char*) pObject + nCount * 0x18 + 0x10) =
				(nRow * *(int*) ((char*) g_pAnimChunkTileGrid + 0x10) + nColumn) * 0x0c +
				*(int*) ((char*) g_pAnimChunkTileGrid + 0x0c);
			nCount = *(int*) ((char*) pObject + 4) + 1;
			*(int*) ((char*) pObject + 4) = nCount;
			if (nCount > 0) {
				pWord = (unsigned short*) ((char*) pObject + 0x14);
				nIndex = 0;
				do {
					*pWord = pWord[1];
					++nIndex;
					pWord = (unsigned short*) ((char*) pWord + 0x18);
				} while (nIndex < *(int*) ((char*) pObject + 4));
			}
		}
	}
	(void) nUnusedEdx;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_163)
extern void* g_pCachedChunkManagerEntityLookup;
class CFormationManager;
extern CFormationManager* g_pGenericGroupFormationManager;
extern void __fastcall AppendGeometryOwnerChild(void* pOwner, int nUnusedEdx, void* pChildOwner);

// FUNCTION: LEMBALL 0x0041f170
void __fastcall CreateShpgGroupWithFourChildren(void* pManager, int nUnusedEdx, int nX, int nY, int nZ)
{
	typedef void(__fastcall * NoArgVirtualProc)(void*);
	typedef void(__fastcall * OneArgVirtualProc)(void*, int, int);
	void* pGroup;
	void* pChild;
	void* pMemory;
	int nRemaining;

	pGroup = 0;
	pMemory = AllocateVSMemBlock(0x16c);
	if (pMemory != 0) {
		pGroup = ((void*(__fastcall*) (void*, int, void*, void*, void*) ) 0x402e2d)(pMemory,
																					0,
																					g_pCachedChunkManagerLevelMode,
																					g_pCachedChunkManagerEntityLookup,
																					g_pGenericGroupFormationManager);
	}
	((NoArgVirtualProc) (*(void***) pGroup)[0x104 / 4])(pGroup);
	((OneArgVirtualProc) (*(void***) pGroup)[0x110 / 4])(pGroup, 0, 1);
	((void(__fastcall*)(void*, int, void*)) 0x4027e8)(pManager, 0, pGroup);
	nRemaining = 4;
	do {
		pChild = 0;
		pMemory = AllocateVSMemBlock(0x124);
		if (pMemory != 0) {
			pChild =
				((void*(__fastcall*) (void*, int, void*, int, int, int, int) ) 0x40234c)(pMemory,
																						 0,
																						 g_pCachedChunkManagerLevelMode,
																						 nX,
																						 nY,
																						 nZ,
																						 0);
		}
		nX -= 12;
		nY -= 12;
		((NoArgVirtualProc) (*(void***) pChild)[0x104 / 4])(pChild);
		((void(__fastcall*)(void*, int, void*, void*)) 0x4017df)(pManager, 0, pChild, pGroup);
		--nRemaining;
	} while (nRemaining != 0);
	((OneArgVirtualProc) (*(void***) pGroup)[0x154 / 4])(pGroup, 0, (int) g_pGenericGroupFormationManager);
	(void) nUnusedEdx;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_166)
// FUNCTION: LEMBALL 0x00412870
void* __fastcall find_level_mode_entity_by_type_in_rect(void* pObject, int nUnused, short* param_2, int param_3)
{
	return ((void*(__fastcall*) (void*, short*, int) ) 0x401e51)(*(void**) ((char*) pObject + 0x158), param_2, param_3);
	(void) nUnused;
}
#endif
