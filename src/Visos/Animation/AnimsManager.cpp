#include "AnimsManager.h"

AnimsManager::AnimsManager()
{
}

// 68K 0x10115d70 FreeVram__13CAnimsManagerFv
// STUB: LEMBALL 0x004358c0
void AnimsManager::FreeVram()
{
}

// 68K 0x1020019c __ct__13CAnimsManagerFP4CGDIUliiiUc
// STUB: LEMBALL 0x00467260
AnimsManager::AnimsManager(Gdi* p_arg0, unsigned long p_arg1, int p_arg2, int p_arg3, int p_arg4, unsigned char p_arg5)
{
}

// 68K 0x10200334 __dt__13CAnimsManagerFv
// STUB: LEMBALL 0x004673d0
AnimsManager::~AnimsManager()
{
}

// 68K 0x1020043c LoadAnims__13CAnimsManagerFUl
// STUB: LEMBALL 0x00467490
void AnimsManager::LoadAnims(unsigned long p_resourceId)
{
}

// 68K 0x102004ea UnLoadAnims__13CAnimsManagerFUl
// STUB: LEMBALL 0x00467500
void AnimsManager::UnLoadAnims(unsigned long p_resourceId)
{
}

// 68K 0x1020055a GetnAnims__13CAnimsManagerFUl
// STUB: LEMBALL 0x00467540
unsigned long AnimsManager::GetnAnims(unsigned long p_resourceId)
{
	return 0;
}

// 68K 0x102005b8 GetAnimSize__13CAnimsManagerFUlUl
// STUB: LEMBALL 0x00467570
VsSize AnimsManager::GetAnimSize(unsigned long p_resourceId, unsigned long p_animIndex)
{
	return *(VsSize*) 0;
}

// 68K 0x1020064e DrawAnim__13CAnimsManagerFRC8CVSPointUlUlP14CAnimFrameBASEP6CRemap
// STUB: LEMBALL 0x00467730
VsRect AnimsManager::DrawAnim(const VsPoint& p_position,
							  unsigned long p_resourceId,
							  unsigned long p_animIndex,
							  AnimFrameBASE* p_frame,
							  Remap* p_remap)
{
	return *(VsRect*) 0;
}

// 68K 0x1020087c ResetPrimitives__13CAnimsManagerFv
// STUB: LEMBALL 0x004678c0
void AnimsManager::ResetPrimitives()
{
}
