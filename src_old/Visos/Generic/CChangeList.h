#ifndef LEMBALL_CCHANGELIST_H
#define LEMBALL_CCHANGELIST_H

#include "Visos/Generic/CVSRect.h"
#include "Visos/Generic/CVSSize.h"

struct CChangeListItem {
	CVSRect m_Rect;
	int m_nState;
};

class CChangeList;
// VC4 ABI bridges retain exact unscoped callers around the typed class.
void __fastcall InitializeHelperUploadStatePending(int nChangeList);
void __fastcall PromoteHelperUploadStateToActive(int nChangeList);
void __fastcall PopHelperUploadStateMarker(void* pChangeList);

class CChangeList {
public:
	CChangeList(int cItems, CVSSize Size, CVSSize CellSize);
	~CChangeList();

	void FreeMap(void);
	void AllocMap(void);
	void Resize(CVSSize Size);
	void Reset(void) { InitializeHelperUploadStatePending((int) (unsigned long) this); }
	void PushActive(unsigned char nUnused);
	void PopActive(void) { PopHelperUploadStateMarker(this); }
	void Add(const CVSRect& Rect);
	void SetDrawMark(const CVSRect& Rect, unsigned char nMark);
	void GetArea(void) { PromoteHelperUploadStateToActive((int) (unsigned long) this); }
	int GetNextArea(unsigned char nTarget, unsigned char nState, unsigned char nReplacement);
	int GetNumItems(void);
	CChangeListItem* GetNItem(int iItem);
	int GetDrawMark(void);

private:
	friend void __fastcall InitializeHelperUploadStatePending(int nChangeList);
	friend void __fastcall PromoteHelperUploadStateToActive(int nChangeList);
	friend void __fastcall PopHelperUploadStateMarker(void* pChangeList);

	int m_cCapacity00;
	int m_cItems04; // -1 indicates an unbuilt list
	int m_iDrawMark08; // index of the first active rect
	CChangeListItem* m_pItems0C;
	unsigned char* m_pMap10;
	CVSSize m_Size14;
	CVSSize m_CellSize18;
	CVSSize m_MapSize1C; // units are cells, not pixels
	int m_cMapEntries20;
	int m_iScanX24;
	int m_iScanY28;
	int m_nArea2C;
	CVSRect m_Area30;
	unsigned char m_anActive38[12];
	int m_cActive44;
	unsigned char m_nActive48;
	unsigned char m_abReserved49[3];
};

typedef char CChangeListItem_size_must_be_0x0C[(sizeof(CChangeListItem) == 0x0c) ? 1 : -1];
typedef char CChangeList_size_must_be_0x4C[(sizeof(CChangeList) == 0x4c) ? 1 : -1];

#endif
