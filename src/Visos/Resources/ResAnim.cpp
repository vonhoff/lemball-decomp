#include "ResAnim.h"

// 68K 0x1020374e Load__8CResANIMFUl
// STUB: LEMBALL 0x0045d610
ResAnim* ResAnim::Load(unsigned int p_resourceId)
{
	return 0;
}

// 68K 0x10203818 AllocateResources__8CResANIMFUl
// STUB: LEMBALL 0x0045d6a0
void ResAnim::AllocateResources(unsigned int p_count)
{
}

// 68K 0x1020388c UnLoadVramData__8CResANIMFUlUc
// STUB: LEMBALL 0x0045d700
void ResAnim::UnLoadVramData(unsigned int p_index, unsigned char p_force)
{
}

// 68K 0x102038da ForceLoadVram__8CResANIMFUl
// STUB: LEMBALL 0x0045d720
bool ResAnim::ForceLoadVram(unsigned int p_index)
{
	return 0;
}

// 68K 0x101160fa GetnVramEntries__8CResANIMFv
// FUNCTION: LEMBALL 0x0045e720
unsigned int ResAnim::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialized != 0) {
		count = 1;
	}
	return count;
}

// 68K 0x10116134 DirectResources__8CResANIMFUlRPUc
// STUB: LEMBALL 0x0045e730
bool ResAnim::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	return 0;
}

// 68K 0x10116182 DirectResources__8CResANIMFUlRPUcRPUc
// STUB: LEMBALL 0x0045e750
bool ResAnim::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	return 0;
}

// 68K 0x101161d8 UnLoadResources__8CResANIMFUlUc
// STUB: LEMBALL 0x0045e780
void ResAnim::UnLoadResources(unsigned int p_index, unsigned char p_force)
{
}

// 68K 0x1011608a __dt__8CResANIMFv
// SYNTHETIC: LEMBALL 0x0045e7a0
// ResAnim::`scalar deleting destructor'
ResAnim::~ResAnim()
{
}
