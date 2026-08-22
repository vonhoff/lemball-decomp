#include "ResMovie.h"

// 68K 0x10204158 Load__9CResMOVIEFUl
// STUB: LEMBALL 0x0045de70
ResMovie* ResMovie::Load(unsigned int p_resourceId)
{
	return 0;
}

// 68K 0x102041e0 __ct__9CResMOVIEFUl
// STUB: LEMBALL 0x0045df20
ResMovie::ResMovie(unsigned long p_arg0)
{
}

// 68K 0x1020425c __dt__9CResMOVIEFv
// STUB: LEMBALL 0x0045df70
ResMovie::~ResMovie()
{
}

// 68K 0x102042e6 AllocateResources__9CResMOVIEFUl
// STUB: LEMBALL 0x0045dfa0
void ResMovie::AllocateResources(unsigned int p_count)
{
}

// 68K 0x10204396 DirectResources__9CResMOVIEFUlRPUcRPUc
// STUB: LEMBALL 0x0045e060
bool ResMovie::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	return 0;
}

// 68K 0x1020443c DirectResources__9CResMOVIEFUlRPUc
// STUB: LEMBALL 0x0045e0c0
bool ResMovie::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	return 0;
}

// 68K 0x102044d6 UnLoadResources__9CResMOVIEFUlUc
// STUB: LEMBALL 0x0045e120
void ResMovie::UnLoadResources(unsigned int p_index, unsigned char p_force)
{
}
