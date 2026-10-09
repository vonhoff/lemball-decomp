#ifndef LEMBALL_VISOS_FOUNDATION_CCHANGELIST_H
#define LEMBALL_VISOS_FOUNDATION_CCHANGELIST_H

#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
class tagCHANGERECT;

// SIZE 0x4c
class CChangeList {
public:
	CChangeList(int p_capacity, CVSSize p_viewSize, CVSSize p_cellSize);
	tagCHANGERECT* GetNItem(int p_index);
	bool GetNextArea(unsigned char p_findMark, bool p_itemMark, unsigned char p_replacementMark);
	int GetDrawMark();
	int GetNumItems();
	unsigned int GetArea();
	void Add(const CVSRect& p_area);
	void AddWithActiveMark(const CVSRect& p_area, unsigned char p_mark);
	void AllocMap();
	void FreeMap();
	void PopActive();
	void PushActive(unsigned char p_mark);
	void Reset();
	void Resize(CVSSize p_size);
	void SetDrawMark();
	~CChangeList();

private:
	int m_capacity;                  // 0x00
	int m_itemCount;                 // 0x04
	int m_drawMarkCount;             // 0x08
	tagCHANGERECT* m_items;          // 0x0c
	unsigned char* m_map;            // 0x10
	CVSSize m_viewSize;              // 0x14
	CVSSize m_cellSize;              // 0x18
	CVSSize m_mapSize;               // 0x1c
	unsigned int m_mapCapacity;      // 0x20
	int m_scanX;                     // 0x24
	int m_scanY;                     // 0x28
	unsigned int m_area;             // 0x2c
	CVSRect m_currentArea;           // 0x30
	unsigned char m_activeStack[12]; // 0x38
	int m_activeDepth;               // 0x44
	unsigned char m_activeMark;      // 0x48
};

#endif
