#ifndef LEMBALL_VISOS_ANIMATION_CANIMSMANAGER_H
#define LEMBALL_VISOS_ANIMATION_CANIMSMANAGER_H

#include "../Foundation/CVsRect.h"
#include "../Foundation/CVsSize.h"
struct CVsPoint;

class CGDI;
class CAnimFrameBASE;
class CRemap;
class CResZRLE;
class CResBase;
class CLine;
class CAnim;
class CZRLE;

// SIZE 0x70
// VTABLE: LEMBALL 0x00499620
class CAnimsManager {
public:
	CAnimsManager(CGDI* p_gdi,
				  unsigned long p_resourceIdCount,
				  int p_resourceCapacity,
				  int p_animCapacity,
				  int p_zrleCapacity,
				  unsigned int p_doubleBuffered);
	CVsRect DrawAnim(const CVsPoint& p_position,
					 unsigned long p_resourceId,
					 unsigned long p_drawFlags,
					 CAnimFrameBASE* p_frame,
					 CRemap* p_remap);
	CVsSize GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex);
	unsigned long GetnAnims(unsigned long p_resourceId);
	virtual void FreeVram(); // vtable+0x00
	void LoadAnims(unsigned long p_resourceId);
	void ResetPrimitives();
	CVsRect* DrawAnimOnGdi(CVsRect* p_bounds,
						   CGDI* p_gdi,
						   const CVsPoint& p_position,
						   unsigned long p_resourceId,
						   unsigned long p_drawFlags,
						   CAnimFrameBASE* p_frame,
						   CRemap* p_remap);
	CResZRLE* ResolveAnimFrameData(unsigned long p_resourceId, CAnimFrameBASE* p_frame);
	void DetachGdi(CGDI* p_gdi);
	void UnLoadAnims(unsigned long p_resourceId);
	~CAnimsManager();
	CAnimsManager();

	friend class CBaseFrontendDrawer;
	friend class C2D;
	friend class CGraphicButton;
	friend class CGunController;
	friend class CHiliteController;
	friend class CPasswordDrawer;
	friend class CLemmingAnimsManager;

private:
	CLine* m_linePrimitives;            // 0x04
	char m_reserved08[4];               // 0x08
	unsigned int m_resetState;          // 0x0c
	unsigned int m_ownsLinePrimitives;  // 0x10
	char m_reserved14[8];               // 0x14
	CGDI* m_gdi;                        // 0x1c
	CGDI* m_previousGdi;                // 0x20
	CResBase** m_resources;             // 0x24
	short* m_resourceSlots;             // 0x28
	int m_resourceCapacity;             // 0x2c
	int m_resourceIdCount;              // 0x30
	int m_loadedResourceCount;          // 0x34
	unsigned int m_doubleBuffered;      // 0x38
	int m_animCapacity;                 // 0x3c
	int m_animCount;                    // 0x40
	char m_animDrawMark[4];             // 0x44
	int m_bufferedAnimCount;            // 0x48
	int m_bufferedZrleCount;            // 0x4c
	unsigned int m_bufferHalf;          // 0x50
	CAnim* m_animPrimitives;            // 0x54
	int m_zrleCapacity;                 // 0x58
	int m_zrleCount;                    // 0x5c
	char m_zrleDrawMark[4];             // 0x60
	CZRLE* m_zrlePrimitives;            // 0x64
	unsigned short m_primitiveSequence; // 0x68
	unsigned short m_reserved6a;        // 0x6a
	unsigned int m_reserved6c;          // 0x6c
};

#endif
