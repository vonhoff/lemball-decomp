#ifndef LEMBALL_VISOS_GENERIC_CSURFACE_H
#define LEMBALL_VISOS_GENERIC_CSURFACE_H

#include "ENGINE/CORE/COMMON.H"
#include "ENGINE/GDI/VSGDI.H"

struct VsGdiResourceGeometryLinkNode {
	void* m_pTarget;
	VsGdiResourceGeometryLinkNode* m_pPrev;
	VsGdiResourceGeometryLinkNode* m_pNext;
};

struct VsGdiResourceGeometryLinkList {
	VsGdiResourceGeometryLinkNode* m_pTail;
	VsGdiResourceGeometryLinkNode* m_pHead;
	int m_cNodes;
};

class CPVGDIBitmap {
public:
	void* m_pVtable;
	int* m_pRows;
	int m_nRowByteOffset;
	void* m_pBackingBase;
	int m_nReserved10;
	int m_iLogicalRow;
	int m_nBackingOrigin;
	int m_nStride;
	int m_cbBorderRow;
	int m_nReserved24;
	int m_cRowCapacity;
	short m_aGeometry2C[10];

	void* Construct(void);
	void Destroy(void);
	void Free(void);
	void Initialise(void);
	void CreateLinePtrs(void);
	void ResetLinePtrs(void);
	void SetLinePtrs(void);
	void Scroll(short* pRect, short* pDestinationPoint);
	void SetSize(short* pOut, short* pSize, int nWidth);
	void SetBitsBase(int nStride, int nOrigin);
	void GetRects(short* pRect, short** ppFirstRect, short** ppSecondRect);
	void ResetScroll(void);
};

class CPVSurface {
public:
	void* m_pVtable;
	short m_aGeometry04[14];
	void* m_pLinkedTarget;
	short m_aGeometry24[4];
	int m_nReserved2C;
	int m_fForwardingEnabled;
	int m_nQueuedState;
	int m_nReserved38;
	int m_fInitialized;
	int m_nReserved40;

	void* Construct(void);
	int HasZBuff(void);
	void Resize(const short* pSize);
	int HasBackBuff(void);
	void SetWorldWidth(int nWidth);
	int GetWorldWidth(void);
	void SetDontUpdateRect(const VsGdiRect& rect);
};

class CPVScrollableSurface {
public:
	CPVGDIBitmap m_RowBuffer;
	void* m_pCPVSurfaceVbtable;

	void SetWorldWidth(int nWidth);
	int GetWorldWidth(void);
};

class CPVBackBuffSurface {
public:
	void* m_pVtable;
	void* m_pCPVSurfaceVbtable;
	CPVGDIBitmap m_RowBuffer;
	int m_fActive;
	void* m_pBackingBuffer;
	short m_nWidth;
	short m_nHeight;

	void FreeBackBuff(void);
	void AllocateBackBuff(void);
	void EnableBackBuff(int fEnabled);
	void ResizeBackBuff(void);
	int HasBackBuff(void);
};

class CPVZBuffSurface {
public:
	void* m_pVtable;
	void* m_pCPVSurfaceVbtable;
	CPVGDIBitmap m_RowBuffer;
	int m_fActive;
	void* m_pBackingBuffer;
	short m_nWidth;
	short m_nHeight;

	void FreeZBuff(void);
	void AllocateZBuff(void);
	void EnableZBuff(unsigned char fEnabled);
	void ResizeZBuff(void);
	int HasZBuff(void);
};

class CSurface {
public:
	CPVScrollableSurface m_ScrollableSurface;
	CPVZBuffSurface m_ZBuffSurface;
	CPVBackBuffSurface m_BackBuffSurface;
	short m_nReservedEC;
	short m_nReservedEE;
	void* m_pBackingBitmap;
	int m_nReservedF4;
	void* m_pDisplayBinding;
	unsigned char m_abReservedFC[0x42c];
	VsGdiResourceGeometryLinkList m_ChildTargetLinks;
	unsigned char m_abCriticalSection[0x18];
	int m_fCriticalSectionReady;
	int m_nActiveUploadState;
	int m_nReserved554;
	int m_nCPVSurfaceVtordisp;
	CPVSurface m_PVSurface;

	CSurface* ConstructResourceGeometryHelperTarget(int nWrappedParam, int fConstructCPVSurface);
	CSurface* Construct(const VsGdiRect* pRect, void* pWrappedTarget, int fConstructCPVSurface);
	void AddToChangeList(short* pRect);
	void ToScreen(CSurface* pPeerTarget);
	void ResetScroll(void);
	void SetLinePtrs(void);
	void* GetChangeList(void);
	void AttachPalette(void* pPalette);
	void Resize(const VsGdiRect& extent);
};

DECOMP_SIZE_ASSERT(CPVGDIBitmap, 0x40);
DECOMP_SIZE_ASSERT(CPVSurface, 0x44);
DECOMP_SIZE_ASSERT(CPVScrollableSurface, 0x44);
DECOMP_SIZE_ASSERT(CPVBackBuffSurface, 0x54);
DECOMP_SIZE_ASSERT(CPVZBuffSurface, 0x54);
DECOMP_SIZE_ASSERT(CSurface, 0x5a0);

#endif
