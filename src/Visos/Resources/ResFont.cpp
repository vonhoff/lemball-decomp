#include "ResFont.h"

// 68K 0x10203950 __ct__8CResFONTFUl
// STUB: LEMBALL 0x0045d7b0
ResFont::ResFont(unsigned long p_arg0)
{
}

// 68K 0x102039d2 __dt__8CResFONTFv
// STUB: LEMBALL 0x0045d810
ResFont::~ResFont()
{
}

// 68K 0x10203a7e Load__8CResFONTFUl
// STUB: LEMBALL 0x0045d850
ResFont* ResFont::Load(unsigned int p_resourceId)
{
	return 0;
}

// 68K 0x10203b04 AllocateResources__8CResFONTFUl
// STUB: LEMBALL 0x0045d8b0
void ResFont::AllocateResources(unsigned int p_count)
{
}

// 68K 0x10203bb2 GetnVramEntries__8CResFONTFv
// STUB: LEMBALL 0x0045d970
unsigned int ResFont::GetnVramEntries()
{
	return 0;
}

// 68K 0x10203bf8 DirectResources__8CResFONTFUlRPUcRPUc
// STUB: LEMBALL 0x0045d990
bool ResFont::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	return 0;
}

// 68K 0x10203c9e DirectResources__8CResFONTFUlRPUc
// STUB: LEMBALL 0x0045d9f0
bool ResFont::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	return 0;
}

// 68K 0x10203d38 UnLoadResources__8CResFONTFUlUc
// STUB: LEMBALL 0x0045da50
void ResFont::UnLoadResources(unsigned int p_index, unsigned char p_force)
{
}

// 68K 0x10203da2 UnLoadVramData__8CResFONTFUlUc
// STUB: LEMBALL 0x0045da90
void ResFont::UnLoadVramData(unsigned int p_index, unsigned char p_force)
{
}

// 68K 0x10203df0 ForceLoadVram__8CResFONTFUl
// STUB: LEMBALL 0x0045dab0
bool ResFont::ForceLoadVram(unsigned int p_index)
{
	return 0;
}

// 68K 0x10203e66 OnLoad__8CResFONTFv
// STUB: LEMBALL 0x0045daf0
void ResFont::OnLoad()
{
}

// 68K 0x10203eb6 ASCIItoZRLE__8CResFONTCFUl
// STUB: LEMBALL 0x0045db20
ResZrle* ResFont::AsciItoZrle(unsigned int p_ascii)
{
	return 0;
}

// 68K 0x10203ef2 GetSize__8CResFONTCFPCcUl
// STUB: LEMBALL 0x0045db30
VsPoint ResFont::GetSize(const char* p_text, unsigned int p_flags)
{
	return *(VsPoint*) 0;
}
