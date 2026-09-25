#ifndef LEMBALL_AI_OBJECTS_CSWITCH_H
#define LEMBALL_AI_OBJECTS_CSWITCH_H

#include "../Base/AiCoord.h"
#include "../Base/CBaseGlobalObject.h"
#include "SwitchEntry.h"

// MINIMUM SIZE 0x260
// VTABLE: LEMBALL 0x00494a18
class CSwitch : public CBaseGlobalObject {
public:
	CSwitch(AiCoord& p_arg0, swMessage p_arg1, int p_arg2, int p_arg3, int p_arg4);
	virtual AiCoord ActivatePosition();              // vtable+0x2c
	virtual bool Activate(CGameObject* p_object);    // vtable+0x18
	virtual bool Process();                          // vtable+0x14
	virtual int Usage();                             // vtable+0x28
	virtual void ConvertVer0ToVer1();                // vtable+0xc4
	virtual void DoActivate();                       // vtable+0x10c
	virtual void GetViewData(CViewData& p_viewData); // vtable+0x0c
	virtual void Restart();                          // vtable+0x104
	virtual ~CSwitch();                              // vtable+0x00
	void AddEntry(swMessage p_message, unsigned short p_objectId);
	unsigned char* Load(unsigned char*& p_data);
	void Throw();

private:
	unsigned int m_scoreAwarded; // 0x144
	unsigned short m_switchId;   // 0x148
	unsigned int m_legacyType;   // 0x14c
	int m_legacyFirst;           // 0x150
	int m_legacyLast;            // 0x154
	int m_legacyAux;             // 0x158
	unsigned short m_entryCount; // 0x15c
	SwitchEntry m_entries[32];   // 0x160
};

// SYNTHETIC: LEMBALL 0x0041dd70
// CSwitch::`scalar deleting destructor'

extern unsigned short g_wNextSwitchIndex;

#endif
