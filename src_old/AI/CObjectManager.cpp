#include "AI/AICoord.h"
#include "Visos/Generic/Memory.h"

class CObjectManager {
public:
	void* Add(short nId,
				AICOORD Position,
				int nType,
				unsigned short nSlotId,
				int nSubtype);
	void* Add(short nId,
			  int nX,
			  int nY,
			  int nZ,
			  int nType,
			  unsigned short nSlotId,
			  int nSubtype);
	void LoadLevel(unsigned short* pData, unsigned int nSize, int fReuseState);
};

// FUNCTION: LEMBALL 0x0041b370
void* CObjectManager::Add(short nId,
						 AICOORD Position,
						 int nType,
						 unsigned short nSlotId,
						 int nSubtype)
{
	void* pChild = 0;
	void* pObject = 0;

	switch (nType) {
	case 4:
	case 5:
	case 0x0d: {
		int nSize = nType == 5 ? 0x148 : 0x144;
		pObject = AllocateVSMemBlock(nSize);
		if (pObject != 0) {
			((void(__fastcall*)(void*, int, int, int, int)) 0x40153c)(pObject, 0, nType, 0, 0);
			((void(__fastcall*)(void*, int)) 0x4032ce)((char*) pObject + 0x138, 0);
			*(void**) pObject = (void*) 0x493580;
			((void(__fastcall*)(void*, int, AICOORD*)) 0x401046)(
				(char*) pObject + 0x138, 0, &Position);
			if (nType == 4) {
				*(void**) pObject = (void*) 0x494640;
			}
			else if (nType == 5) {
				*(void**) pObject = (void*) 0x494268;
			}
			else {
				*(void**) pObject = (void*) 0x494788;
			}
		}
		break;
	}
	case 0x11:
		if (nSubtype < 0x18) {
			if (nSubtype < 0x15) {
				if (nSubtype == 4) {
					pChild = AllocateVSMemBlock(0x144);
					if (pChild != 0) {
						((void(__fastcall*)(void*, int, int, int, int)) 0x40153c)(pChild, 0, 4, 0, 0);
						((void(__fastcall*)(void*, int)) 0x4032ce)((char*) pChild + 0x138, 0);
						*(void**) pChild = (void*) 0x493580;
						((void(__fastcall*)(void*, int, AICOORD*)) 0x401046)(
							(char*) pChild + 0x138, 0, &Position);
						*(void**) pChild = (void*) 0x494640;
					}
				}
			}
			else {
				pChild = AllocateVSMemBlock(0x150);
				if (pChild != 0) {
					((void(__fastcall*)(void*, int, AICOORD*, int)) 0x402e3c)(
						pChild, 0, &Position, nSubtype);
					*(int*) ((char*) pChild + 0x144) = 0xaa55aa55;
					*(int*) ((char*) pChild + 0x148) = 0xaa55aa55;
					*(int*) ((char*) pChild + 0x14c) = 0xaa55aa55;
					*(void**) pChild = (void*) 0x4943b0;
				}
			}
		}
		else if (nSubtype < 0x10000 && nSubtype != 0xffff) {
			switch (nSubtype) {
			case 0x27:
			case 0x29:
			case 0x2b:
			case 0x2d:
				pObject = AllocateVSMemBlock(0x150);
				if (pObject != 0) {
					((void(__fastcall*)(void*, int, AICOORD*, int)) 0x402e3c)(
						pObject, 0, &Position, nSubtype);
					((void(__fastcall*)(void*, int)) 0x4032ce)((char*) pObject + 0x144, 0);
					*(void**) pObject = (void*) 0x4944f8;
				}
				break;
			}
		}
		if (pChild != 0) {
			((void(__fastcall*)(void*, int, unsigned short)) 0x402293)(pChild, 0, nSlotId);
		}
		pObject = AllocateVSMemBlock(0x150);
		if (pObject != 0) {
			pObject = ((void*(__fastcall*)(void*, int, AICOORD*, void*, unsigned short)) 0x402950)(
				pObject, 0, &Position, pChild, nSlotId);
		}
		break;
	case 0x14:
		pObject = AllocateVSMemBlock(0x260);
		if (pObject != 0) {
			pObject = ((void*(__fastcall*)(void*, int, AICOORD*, int, int, int, int)) 0x402702)(
				pObject, 0, &Position, 0, 0, 0, 0);
		}
		break;
	case 0x15:
	case 0x16:
	case 0x17:
		pObject = AllocateVSMemBlock(0x150);
		if (pObject != 0) {
			((void(__fastcall*)(void*, int, AICOORD*, int)) 0x402e3c)(
				pObject, 0, &Position, nType);
			*(int*) ((char*) pObject + 0x144) = 0xaa55aa55;
			*(int*) ((char*) pObject + 0x148) = 0xaa55aa55;
			*(int*) ((char*) pObject + 0x14c) = 0xaa55aa55;
			*(void**) pObject = (void*) 0x4943b0;
		}
		break;
	case 0x1c:
		pObject = AllocateVSMemBlock(0x144);
		if (pObject != 0) {
			pObject = ((void*(__fastcall*)(void*, int, AICOORD*)) 0x403887)(
				pObject, 0, &Position);
		}
		break;
	case 0x27:
	case 0x29:
	case 0x2b:
	case 0x2d:
		pObject = AllocateVSMemBlock(0x150);
		if (pObject != 0) {
			((void(__fastcall*)(void*, int, AICOORD*, int)) 0x402e3c)(
				pObject, 0, &Position, nType);
			*(int*) ((char*) pObject + 0x144) = 0xaa55aa55;
			*(int*) ((char*) pObject + 0x148) = 0xaa55aa55;
			*(int*) ((char*) pObject + 0x14c) = 0xaa55aa55;
			*(void**) pObject = (void*) 0x4944f8;
		}
		break;
	}

	((void(__fastcall*)(void*, int)) (*(void***) pObject)[0x104 / 4])(pObject, 0);
	*(void**) ((char*) pObject + 0x60) = this;
	if (pChild != 0) {
		*(void**) ((char*) pChild + 0x60) = this;
		((void(__fastcall*)(void*, int)) (*(void***) pChild)[0x104 / 4])(pChild, 0);
	}
	return ((void*(__fastcall*)(void*, int, short, void*, int)) 0x402cac)(this, 0, nId, pObject, 1);
}
// FUNCTION: LEMBALL 0x0041bbc0
void CObjectManager::LoadLevel(unsigned short* pData, unsigned int, int fReuseState)
{
	unsigned short nCount = *pData++;
	int nSearchIndex = 0;
	if (fReuseState == 0) {
		((void(__fastcall*)(void*, int, int)) 0x403242)(this, 0, nCount);
	}
	while (nCount-- != 0) {
		unsigned short nId;
		if (*(unsigned short*) (*(int*) ((char*) this + 0x30) + 0x54) < 2) {
			nId = ((unsigned short(__cdecl*)(void)) 0x40214e)();
		}
		else {
			nId = *pData++;
		}
		int nX = (unsigned int) *pData++ << 12;
		int nY = (unsigned int) *pData++ << 12;
		int nZ = (unsigned int) *pData++ << 12;
		unsigned short nType = *pData++;
		++pData;

		switch (nType) {
		case 4:
		case 0x0d:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x1c:
		case 0x27:
		case 0x29:
		case 0x2b:
		case 0x2d:
			if (fReuseState == 0) {
				Add(nId, nX, nY, nZ, nType, 0xffff, 0xffff);
			}
			break;
		case 5: {
			unsigned short nVariant = 0;
			if (*(unsigned short*) (*(int*) ((char*) this + 0x30) + 0x54) >= 8) {
				nVariant = *pData++;
			}
			if (fReuseState == 0) {
				void* pObject = Add(nId, nX, nY, nZ, nType, 0xffff, 0xffff);
				*(unsigned short*) ((char*) pObject + 0x144) = nVariant;
			}
			break;
		}
		case 0x11: {
			unsigned short nSubtype = *pData++;
			unsigned short nSlotId;
			if (*(unsigned short*) (*(int*) ((char*) this + 0x30) + 0x54) < 2) {
				nSlotId = ((unsigned short(__cdecl*)(void)) 0x40214e)();
			}
			else {
				nSlotId = *pData++;
			}
			if (fReuseState == 0) {
				Add(nId, nX, nY, nZ, 0x11, nSlotId, nSubtype);
			}
			break;
		}
		case 0x14:
			if (*(unsigned short*) (*(int*) ((char*) this + 0x30) + 0x54) < 2) {
				unsigned short nParam5 = *pData++;
				unsigned short nParam6 = *pData++;
				unsigned short nParam7 = *pData++;
				unsigned short nParam8 = *pData++;
				if (fReuseState == 0) {
					((void(__fastcall*)(void*, int, unsigned short, int, int, int, int, int, int, int)) 0x4030d0)(
						this, 0, nId, nX, nY, nZ, nParam5, nParam6, nParam7, nParam8);
				}
			}
			else {
				void* pObject = 0;
				if (fReuseState == 0) {
					pObject = Add(nId, nX, nY, nZ, nType, 0xffff, 0xffff);
				}
				else {
					void** ppObject = *(void***) ((char*) this + 0x3c) + nSearchIndex;
					while (nSearchIndex < *(unsigned short*) ((char*) this + 0x36)) {
						pObject = *ppObject++;
						++nSearchIndex;
						if (*(int*) ((char*) pObject + 0x64) == 0x14) {
							break;
						}
					}
				}
				((void(__fastcall*)(void*, int, unsigned short**)) 0x401695)(pObject, 0, &pData);
			}
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x0041b310
#pragma auto_inline(off)
void* CObjectManager::Add(short nId,
						  int nX,
						  int nY,
						  int nZ,
						  int nType,
						  unsigned short nSlotId,
						  int nSubtype)
{
	AICOORD Position;

	Position.x = nX << 12;
	Position.y = nY << 12;
	Position.z = nZ << 12;
	return Add(nId, Position, nType, nSlotId, nSubtype);
}
#pragma auto_inline(on)
