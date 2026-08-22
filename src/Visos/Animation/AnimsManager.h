#ifndef LEMBALL_VISOS_ANIMATION_ANIMSMANAGER_H
#define LEMBALL_VISOS_ANIMATION_ANIMSMANAGER_H

#include "../../Common.h"
#include "../Foundation/VsRect.h" // complete type
#include "../Foundation/VsSize.h" // complete type

// SIZE 0x70
// VTABLE: LEMBALL 0x00499620
class AnimsManager {
public:
	AnimsManager(Gdi* p_arg0, unsigned long p_arg1, int p_arg2, int p_arg3, int p_arg4, unsigned char p_arg5);
	VsRect DrawAnim(const VsPoint& p_position,
					unsigned long p_resourceId,
					unsigned long p_animIndex,
					AnimFrameBASE* p_frame,
					Remap* p_remap);
	VsSize GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex);
	unsigned long GetnAnims(unsigned long p_resourceId);
	virtual void FreeVram(); // vtable+0x00
	void LoadAnims(unsigned long p_resourceId);
	void ResetPrimitives();
	void UnLoadAnims(unsigned long p_resourceId);
	~AnimsManager();
	AnimsManager();

private:
	Line* m_linePrimitives;             // 0x04
	undefined m_reserved08[4];          // 0x08
	unsigned int m_resetState;          // 0x0c
	unsigned int m_ownsLinePrimitives;  // 0x10
	undefined m_reserved14[8];          // 0x14
	Gdi* m_gdi;                         // 0x1c
	Gdi* m_previousGdi;                 // 0x20
	ResBase** m_resources;              // 0x24
	short* m_resourceSlots;             // 0x28
	int m_resourceCapacity;             // 0x2c
	int m_resourceIdCount;              // 0x30
	int m_loadedResourceCount;          // 0x34
	unsigned int m_doubleBuffered;      // 0x38
	int m_animCapacity;                 // 0x3c
	int m_animCount;                    // 0x40
	unsigned int m_animDrawMark;        // 0x44
	int m_bufferedAnimCount;            // 0x48
	int m_bufferedZrleCount;            // 0x4c
	unsigned int m_bufferHalf;          // 0x50
	Anim* m_animPrimitives;             // 0x54
	int m_zrleCapacity;                 // 0x58
	int m_zrleCount;                    // 0x5c
	unsigned int m_zrleDrawMark;        // 0x60
	Zrle* m_zrlePrimitives;             // 0x64
	unsigned short m_primitiveSequence; // 0x68
	undefined2 m_reserved6a;            // 0x6a
	unsigned int m_reserved6c;          // 0x6c
};

#endif
