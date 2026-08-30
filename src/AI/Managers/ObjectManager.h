#ifndef LEMBALL_AI_MANAGERS_OBJECTMANAGER_H
#define LEMBALL_AI_MANAGERS_OBJECTMANAGER_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type
#include "../Base/AiCoord.h"                      // complete type

// SIZE 0x40
// VTABLE: LEMBALL 0x00494238
class ObjectManager : public NetworkMessage {
public:
	GlobalGameObject* Add(unsigned short p_id,
						  AiCoord p_position,
						  eObjectType p_objectType,
						  unsigned short p_linkedObjectId,
						  eObjectType p_linkedObjectType);
	GlobalGameObject* AddObject(unsigned short p_id, GlobalGameObject* p_object, unsigned char p_active);
	GlobalGameObject* FindObject(int p_id);
	ObjectManager(Ai* p_arg0, int p_arg1);
	Switch* AddSwitch(unsigned short p_id,
					  int p_x,
					  int p_y,
					  int p_z,
					  int p_message,
					  int p_legacyFirst,
					  int p_legacyLast,
					  int p_legacyAux);
	virtual bool Receive(unsigned short p_message,
						 GlobalGameObject* p_object,
						 NetworkMessage* p_networkMessage); // vtable+0x20
	virtual int GetViewData(ViewData* p_viewData);          // vtable+0x24
	virtual void Process();                                 // vtable+0x1c
	virtual void Restart();                                 // vtable+0x18
	virtual ~ObjectManager();                               // vtable+0x14
	void ConvertVer0ToVer1();
	void Initialise(int p_objectCount);
	void LoadLevel(unsigned char* p_data, unsigned long p_length, unsigned char p_append);
	void Remove(GlobalGameObject* p_object);

private:
	unsigned int m_messageType;   // 0x2c
	Ai* m_ai;                     // 0x30
	unsigned short m_capacity;    // 0x34
	unsigned short m_count;       // 0x36
	undefined4 m_unk0x38;         // 0x38
	GlobalGameObject** m_objects; // 0x3c
};

extern ObjectManager* g_pGenericGroupObjectManager;

// SYNTHETIC: LEMBALL 0x0041c330
// ObjectManager::`scalar deleting destructor'

#endif
