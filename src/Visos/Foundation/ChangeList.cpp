#include "ChangeList.h"

// 68K 0x10210cf8 __ct__11CChangeListFi7CVSSize7CVSSize
// STUB: LEMBALL 0x004669a0
ChangeList::ChangeList(int p_arg0, VsSize p_arg1, VsSize p_arg2)
{
}

// 68K 0x10210e4c FreeMap__11CChangeListFv
// STUB: LEMBALL 0x00466a90
void ChangeList::FreeMap()
{
}

// 68K 0x10210e98 AllocMap__11CChangeListFv
// STUB: LEMBALL 0x00466ac0
void ChangeList::AllocMap()
{
}

// 68K 0x10210eea Resize__11CChangeListF7CVSSize
// STUB: LEMBALL 0x00466af0
void ChangeList::Resize(VsSize p_size)
{
}

// 68K 0x10210f9a Reset__11CChangeListFv
// STUB: LEMBALL 0x00466b60
void ChangeList::Reset()
{
}

// 68K 0x10211012 __dt__11CChangeListFv
// STUB: LEMBALL 0x00466b90
ChangeList::~ChangeList()
{
}

// 68K 0x10211070 PushActive__11CChangeListFUc
// FUNCTION: LEMBALL 0x00466bc0
void ChangeList::PushActive(unsigned char p_mark)
{
	m_activeStack[m_activeDepth] = m_activeMark;
	m_activeDepth = m_activeDepth + 1;
}

// 68K 0x102110ce PopActive__11CChangeListFv
// FUNCTION: LEMBALL 0x00466bd0
void ChangeList::PopActive()
{
	m_activeDepth = m_activeDepth - 1;
	m_activeMark = m_activeStack[m_activeDepth];
}

// 68K 0x10211106 Add__11CChangeListFRC7CVSRect
// STUB: LEMBALL 0x00466be0
void ChangeList::Add(const VsRect& p_area)
{
}

// 68K 0x1021122a SetDrawMark__11CChangeListFv
// STUB: LEMBALL 0x00466d10
void ChangeList::SetDrawMark()
{
}

// 68K 0x1021128c GetArea__11CChangeListFv
// FUNCTION: LEMBALL 0x00466d30
unsigned int ChangeList::GetArea()
{
	return m_area;
}

// 68K 0x102112b8 GetNextArea__11CChangeListFUcUcUc
// STUB: LEMBALL 0x00466d40
bool ChangeList::GetNextArea(unsigned char p_findMark, unsigned char p_itemMark, unsigned char p_replacementMark)
{
	return 0;
}

// 68K 0x10211504 GetNumItems__11CChangeListFv
// STUB: LEMBALL 0x00466ef0
int ChangeList::GetNumItems()
{
	return 0;
}

// 68K 0x10211630 GetNItem__11CChangeListFi
// FUNCTION: LEMBALL 0x00467000
ChangeListItem* ChangeList::GetNItem(int p_index)
{
	if (m_itemCount == -1) {
		GetNumItems();
	}
	return m_items + p_index;
}

// 68K 0x1021167a GetDrawMark__11CChangeListFv
// FUNCTION: LEMBALL 0x00467020
int ChangeList::GetDrawMark()
{
	if (m_map == 0) {
		return 0;
	}
	if (m_itemCount == -1) {
		GetNumItems();
	}
	return m_drawMarkCount;
}
