#include "ResFont.h"
#include "FontTable.h"
#include "MogRes.h"
#include "ResourceTypeList.h"

// 68K 0x10203950 __ct__8CResFONTFUl
// FUNCTION: LEMBALL 0x0045d7b0
ResFont::ResFont(unsigned long p_resourceId)
	: ResBaseList((ResListHeader*) g_pResourceTypes)
{
	m_animationEntries = 0;
	m_fontEntries = 0;
	m_fontTable = 0;
	DoLoad(p_resourceId);
	m_initialized = 1;
}

// 68K 0x102039d2 __dt__8CResFONTFv
// FUNCTION: LEMBALL 0x0045d810
ResFont::~ResFont()
{
	if (m_animationEntries != 0) {
		delete[] m_animationEntries;
	}
	if (m_fontEntries != 0) {
		delete[] m_fontEntries;
	}
	if (m_fontTable != 0) {
		delete m_fontTable;
	}
}

// 68K 0x10203a7e Load__8CResFONTFUl
// FUNCTION: LEMBALL 0x0045d850
ResFont* ResFont::Load(unsigned int p_resourceId)
{
	ResFont* res = (ResFont*) g_pActiveMogRes->Find(p_resourceId);
	if (res == 0) {
		return (ResFont*) (new ResFont(p_resourceId))->CheckError();
	}
	if (res->m_chunkType != 0x4c495354) {
		res->UnLoad();
		return 0;
	}
	return res;
}

// 68K 0x10203b04 AllocateResources__8CResFONTFUl
// FUNCTION: LEMBALL 0x0045d8b0
void ResFont::AllocateResources(unsigned int p_count)
{
	m_fontEntries = new ResInt[p_count];
	m_animationEntries = new ResZrle[p_count];
}

// 68K 0x10203bb2 GetnVramEntries__8CResFONTFv
// FUNCTION: LEMBALL 0x0045d970
unsigned int ResFont::GetnVramEntries()
{
	unsigned int count = 0;
	if (m_animationEntries->m_initialized != 0) {
		count = 1;
	}
	if (m_fontEntries->m_initialized != 0) {
		count++;
	}
	return count;
}

// 68K 0x10203bf8 DirectResources__8CResFONTFUlRPUcRPUc
// FUNCTION: LEMBALL 0x0045d990
bool ResFont::DirectResources(unsigned int p_index, unsigned char** p_headerCursor, unsigned char** p_dataCursor)
{
	if (!m_fontEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this)) {
		if (!m_animationEntries[p_index].Direct(*p_headerCursor, *p_dataCursor, this)) {
			return false;
		}
	}
	return true;
}

// 68K 0x10203c9e DirectResources__8CResFONTFUlRPUc
// FUNCTION: LEMBALL 0x0045d9f0
bool ResFont::DirectResources(unsigned int p_index, unsigned char** p_cursor)
{
	if (!m_fontEntries[p_index].Direct(*p_cursor, this)) {
		if (!m_animationEntries[p_index].Direct(*p_cursor, this)) {
			return false;
		}
	}
	return true;
}

// 68K 0x10203d38 UnLoadResources__8CResFONTFUlUc
// FUNCTION: LEMBALL 0x0045da50
void ResFont::UnLoadResources(unsigned int p_index, unsigned int p_force)
{
	m_fontEntries[p_index].UnLoadExtData(p_force);
	m_animationEntries[p_index].UnLoadExtData(p_force);
}

// 68K 0x10203da2 UnLoadVramData__8CResFONTFUlUc
// FUNCTION: LEMBALL 0x0045da90
void ResFont::UnLoadVramData(unsigned int p_index, unsigned int p_force)
{
	m_animationEntries[p_index].UnLoadVramData(p_force);
}

// 68K 0x10203df0 ForceLoadVram__8CResFONTFUl
// FUNCTION: LEMBALL 0x0045dab0
bool ResFont::ForceLoadVram(unsigned int p_index)
{
	if (!m_animationEntries[p_index].GetfVramLoaded()) {
		return m_animationEntries[p_index].GetfVramLoaded();
	}
	return true;
}

// 68K 0x10203e66 OnLoad__8CResFONTFv
// FUNCTION: LEMBALL 0x0045daf0
void ResFont::OnLoad()
{
	if (m_fontTable == 0) {
		m_fontTable = new FontTable(this);
	}
}

// 68K 0x10203eb6 ASCIItoZRLE__8CResFONTCFUl
// FUNCTION: LEMBALL 0x0045db20
ResZrle* ResFont::AsciItoZrle(unsigned int p_ascii)
{
	return m_fontTable->GetZrle(p_ascii);
}

// 68K 0x10203ef2 GetSize__8CResFONTCFPCcUl
// FUNCTION: LEMBALL 0x0045db30
VsPoint ResFont::GetSize(const char* p_text, unsigned int p_flags)
{
	int i = 0;
	short height = 0;
	short width = 0;
	if (p_text[0] != '\0') {
		do {
			ResZrle* glyph = AsciItoZrle(p_text[i]);
			if (glyph == 0) {
				glyph = AsciItoZrle('I');
				if (glyph == 0) {
					glyph = m_animationEntries;
				}
			}
			short* psVar = &glyph->m_width;
			if ((p_flags & 0x60) == 0) {
				if (width < glyph->m_x + *psVar) {
					width = glyph->m_x + *psVar;
				}
			}
			else {
				width += *psVar + 1;
			}
			if ((p_flags & 0x180) == 0) {
				if (height < glyph->m_y + psVar[1]) {
					height = psVar[1] + glyph->m_y;
				}
			}
			else {
				height += psVar[1] + 1;
			}
			i++;
		} while (p_text[i] != '\0');
	}
	if ((p_flags & 0x60) != 0) {
		width--;
	}
	if ((p_flags & 0x180) != 0) {
		height--;
	}
	VsPoint result;
	result.m_x = width;
	result.m_y = height;
	return result;
}
