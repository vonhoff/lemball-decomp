#ifndef LEMBALL_VISOS_RESOURCES_RESBASELIST_H
#define LEMBALL_VISOS_RESOURCES_RESBASELIST_H

#include "../../Common.h"
#include "ResBase.h" // complete type

// SIZE 0x0c
struct ResListHeader {
	unsigned int m_totalSize;
	unsigned int m_headerSize;
	unsigned int m_bodySize;
};

// SIZE 0x78
// VTABLE: LEMBALL 0x004989c0
class ResBaseList : public ResBase {
public:
	virtual bool ForceLoadVram();                                                              // vtable+0x18
	virtual bool ForceLoadVram(unsigned int p_index);                                          // vtable+0x3c
	virtual bool GetfAnyVramLoaded();                                                          // vtable+0x14
	virtual bool GetfVramLoaded();                                                             // vtable+0x0c
	virtual bool GetfVramSwappable();                                                          // vtable+0x10
	virtual unsigned int GetSizeUsed();                                                        // vtable+0x38
	virtual unsigned int GetnVramEntries();                                                    // vtable+0x48
	virtual void LoadData();                                                                   // vtable+0x1c
	virtual void OnRead(unsigned char* p_source, unsigned char** p_data, unsigned int p_size); // vtable+0x04
	virtual void SetHeader();                                                                  // vtable+0x08
	virtual void SetType();                                                                    // vtable+0x34
	virtual void UnLoadData(unsigned char p_force);                                            // vtable+0x20
	virtual void UnLoadVramData(unsigned char p_force);                                        // vtable+0x24
	virtual void UnLoadVramData(unsigned int p_index, unsigned char p_force);                  // vtable+0x40
	virtual ~ResBaseList();                                                                    // vtable+0x00

private:
	ResListHeader* m_listHeader;    // 0x48
	unsigned char* m_headerData;    // 0x4c
	undefined4 m_unk0x50;           // 0x50
	unsigned int m_vramReady;       // 0x54
	unsigned int m_headerLoaded;    // 0x58
	unsigned int m_bodyLoaded;      // 0x5c
	unsigned int m_vramLoadedCount; // 0x60
	int m_vramEntryCount;           // 0x64
	unsigned int m_vramSwappable;   // 0x68
	unsigned int m_totalSize;       // 0x6c
	unsigned int m_headerSize;      // 0x70
	unsigned int m_bodySize;        // 0x74
};

#endif
