#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_001)
// FUNCTION: LEMBALL 0x0040F7E0
int __fastcall poll_managed_entity_stream_dirty_flag(void* pObject)
{
	char* pThis;
	int* pDirty;
	int nState;
	pThis = (char*) pObject;
	pDirty = (int*) (pThis + 0x164);

	if (((*(unsigned long*) (pThis + 0x194) ^ *(unsigned long*) (pThis + 0x9c)) & 0xfffff000UL) == 0 && *pDirty == 0) {
		*pDirty = 0;
	}
	else {
		*pDirty = 1;
	}
	if (((*(unsigned long*) (pThis + 0x198) ^ *(unsigned long*) (pThis + 0xa0)) & 0xfffff000UL) == 0 && *pDirty == 0) {
		*pDirty = 0;
	}
	else {
		*pDirty = 1;
	}
	if (((*(unsigned long*) (pThis + 0x19c) ^ *(unsigned long*) (pThis + 0xa4)) & 0xfffff000UL) == 0 && *pDirty == 0) {
		*pDirty = 0;
	}
	else {
		*pDirty = 1;
	}

	nState = *(int*) (pThis + 0xb8);
	if (nState == 0) {
		if (*(int*) (pThis + 0x1a4) == 0 && *pDirty == 0) {
			*pDirty = 0;
		}
		else {
			*pDirty = 1;
		}
	}
	else if (nState != 1) {
		if (*(int*) (pThis + 0x1a4) == nState && *pDirty == 0) {
			*pDirty = 0;
		}
		else {
			*pDirty = 1;
		}
		if (*(short*) (pThis + 0x1a8) == *(short*) (pThis + 0xbc) && *pDirty == 0) {
			*pDirty = 0;
		}
		else {
			*pDirty = 1;
		}
		if (*(int*) (pThis + 0x1ac) == *(int*) (pThis + 0x94) && *pDirty == 0) {
			*pDirty = 0;
		}
		else {
			*pDirty = 1;
		}
	}
	if (nState != 1) {
		*(int*) (pThis + 0x1a4) = nState;
	}

	if (*(int*) (pThis + 0x1b0) == *(int*) (pThis + 0x98) && *pDirty == 0) {
		*pDirty = 0;
	}
	else {
		*pDirty = 1;
	}

	*(unsigned long*) (pThis + 0x194) = *(unsigned long*) (pThis + 0x9c);
	*(short*) (pThis + 0x1a0) = *(short*) (pThis + 0xb4);
	*(unsigned long*) (pThis + 0x198) = *(unsigned long*) (pThis + 0xa0);
	*(unsigned long*) (pThis + 0x19c) = *(unsigned long*) (pThis + 0xa4);
	*(int*) (pThis + 0x1b0) = *(int*) (pThis + 0x98);
	*(short*) (pThis + 0x1a8) = *(short*) (pThis + 0xbc);
	*(int*) (pThis + 0x1ac) = *(int*) (pThis + 0x94);
	return *pDirty;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_010)
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

// FUNCTION: LEMBALL 0x0041f2e0
void __fastcall load_shpg_chunk_objects(void* pThis, int, unsigned char* pData, unsigned int cbData, int fExisting)
{
	typedef void(LEMBALL_FASTCALL * NoArgProc)(void*);
	typedef void(LEMBALL_FASTCALL * OneArgProc)(void*, int, int);
	typedef void*(__cdecl * AllocateProc)(unsigned int);
	typedef void*(LEMBALL_FASTCALL * ConstructGroupProc)(void*, int, void*, void*, void*);
	typedef void*(LEMBALL_FASTCALL * ConstructChildProc)(void*, int, void*, int, int, int, int);
	typedef void(LEMBALL_FASTCALL * AddGroupProc)(void*, int, void*);
	typedef void(LEMBALL_FASTCALL * AttachChildProc)(void*, int, void*, void*);
	unsigned int nRecords = cbData / 6;
	if (fExisting == 0) {
		while (nRecords != 0) {
			unsigned int nChildren = pData[0];
			unsigned int nFormation = pData[1];
			unsigned int nX = *(unsigned short*) (pData + 2);
			unsigned int nY = *(unsigned short*) (pData + 4);
			void* pMemory = ((AllocateProc) 0x0045A780)(0x16c);
			void* pGroup = 0;
			if (pMemory != 0) {
				pGroup = ((ConstructGroupProc) 0x00402E2D)(pMemory,
														   0,
														   *(void**) 0x004A782C,
														   *(void**) 0x004A7830,
														   *(void**) 0x004A7834);
			}
			void* pVtable = *(void**) pGroup;
			((NoArgProc) ((void**) pVtable)[0x104 / 4])(pGroup);
			((AddGroupProc) 0x004027E8)(pThis, 0, pGroup);
			((OneArgProc) ((void**) pVtable)[0x110 / 4])(pGroup, 0, nFormation);
			while (nChildren != 0) {
				pMemory = ((AllocateProc) 0x0045A780)(0x124);
				void* pChild = 0;
				if (pMemory != 0) {
					pChild = ((ConstructChildProc) 0x0040234C)(pMemory, 0, *(void**) 0x004A782C, nX, nY, 0, 0);
				}
				nX -= 0x14;
				nY -= 0x14;
				((NoArgProc) (*(void***) pChild)[0x104 / 4])(pChild);
				((AttachChildProc) 0x004017DF)(pVtable, 0, pChild, pGroup);
				--nChildren;
			}
			((OneArgProc) ((void**) pVtable)[0x154 / 4])(pGroup, 0, *(int*) 0x004A7834);
			pData += 6;
			--nRecords;
		}
	}
}
#endif
