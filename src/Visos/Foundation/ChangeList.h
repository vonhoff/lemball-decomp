#ifndef LEMBALL_VISOS_FOUNDATION_CHANGELIST_H
#define LEMBALL_VISOS_FOUNDATION_CHANGELIST_H

#include "../../Common.h"
#include "VsRect.h" // complete type
#include "VsSize.h" // complete type

// SIZE 0x4c
class ChangeList {
public:
	ChangeList(int p_arg0, VsSize p_arg1, VsSize p_arg2);
	ChangeListItem* GetNItem(int p_index);
	bool GetNextArea(unsigned char p_findMark, unsigned char p_itemMark, unsigned char p_replacementMark);
	int GetDrawMark();
	int GetNumItems();
	unsigned int GetArea();
	void Add(const VsRect& p_area);
	void AllocMap();
	void FreeMap();
	void PopActive();
	void PushActive(unsigned char p_mark);
	void Reset();
	void Resize(VsSize p_size);
	void SetDrawMark();
	~ChangeList();

private:
	int m_capacity;                  // 0x00
	int m_itemCount;                 // 0x04
	int m_drawMarkCount;             // 0x08
	ChangeListItem* m_items;         // 0x0c
	unsigned char* m_map;            // 0x10
	VsSize m_viewSize;               // 0x14
	VsSize m_cellSize;               // 0x18
	VsSize m_mapSize;                // 0x1c
	unsigned int m_mapCapacity;      // 0x20
	int m_scanX;                     // 0x24
	int m_scanY;                     // 0x28
	unsigned int m_area;             // 0x2c
	VsRect m_currentArea;            // 0x30
	unsigned char m_activeStack[12]; // 0x38
	int m_activeDepth;               // 0x44
	unsigned char m_activeMark;      // 0x48
};

#endif
