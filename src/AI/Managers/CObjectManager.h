#ifndef LEMBALL_AI_MANAGERS_COBJECTMANAGER_H
#define LEMBALL_AI_MANAGERS_COBJECTMANAGER_H

#include "../Base/CGlobalGameObject.h" // complete type
#include "AI/Base/ObjectTypes.h"
#include "CBaseObjectManager.h" // complete type

class AiCoord;
class CAI;
class CSwitch;

// SIZE 0x40
// VTABLE: LEMBALL 0x00494238
class CObjectManager : public CBaseObjectManager {
public:
	CGlobalGameObject* Add(unsigned short p_id,
						   AiCoord p_position,
						   eObjectType p_objectType,
						   unsigned short p_linkedObjectId,
						   eObjectType p_linkedObjectType);
	CGlobalGameObject* AddObject(unsigned short p_id, CGlobalGameObject* p_object, unsigned int p_active);
	CGlobalGameObject* FindObject(int p_id);
	CObjectManager(CAI* p_ai, int p_arg1);
	CSwitch* AddSwitch(unsigned short p_id,
					   int p_x,
					   int p_y,
					   int p_z,
					   int p_message,
					   int p_legacyFirst,
					   int p_legacyLast,
					   int p_legacyAux);
	virtual bool Receive(unsigned short p_message,
						 CGlobalGameObject* p_object,
						 CNetworkMessage* p_networkMessage); // vtable+0x20
	virtual int GetViewData(CViewData* p_viewData);          // vtable+0x24
	virtual void Process();                                  // vtable+0x1c
	virtual void Restart();                                  // vtable+0x18
	virtual ~CObjectManager();                               // vtable+0x14
	void ConvertVer0ToVer1();
	void Initialise(int p_objectCount);
	void LoadLevel(unsigned char* p_data, unsigned long p_length, unsigned int p_append);
	void Remove(CGlobalGameObject* p_object);

private:
	CAI* m_ai;                     // 0x30
	unsigned short m_capacity;     // 0x34
	unsigned short m_count;        // 0x36
	unsigned short m_unk0x38;      // 0x38
	CGlobalGameObject** m_objects; // 0x3c
};

extern CObjectManager* g_pObjectManager;
extern CObjectManager* g_pGenericGroupObjectManager;

// SYNTHETIC: LEMBALL 0x0041c330
// CObjectManager::`scalar deleting destructor'

#endif
