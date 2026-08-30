#include "ObjectManager.h"

// 68K 0x1061a098 __ct__14CObjectManagerFP3CAIi
// STUB: LEMBALL 0x0041af60
ObjectManager::ObjectManager(Ai* p_arg0, int p_arg1)
{
}

// 68K 0x1061a136 Restart__14CObjectManagerFv
// STUB: LEMBALL 0x0041afd0
void ObjectManager::Restart()
{
}

// 68K 0x1061a24c Initialise__14CObjectManagerFi
// STUB: LEMBALL 0x0041b0c0
void ObjectManager::Initialise(int p_objectCount)
{
}

// 68K 0x1061a36c AddObject__14CObjectManagerFUsP17CGlobalGameObjectUc
// STUB: LEMBALL 0x0041b2a0
GlobalGameObject* ObjectManager::AddObject(unsigned short p_id, GlobalGameObject* p_object, unsigned char p_active)
{
	return 0;
}

// 68K 0x1061a440 Add__14CObjectManagerFUs7AICOORD11eObjectTypeUs11eObjectType
// STUB: LEMBALL 0x0041b370
GlobalGameObject* ObjectManager::Add(unsigned short p_id,
									 AiCoord p_position,
									 eObjectType p_objectType,
									 unsigned short p_linkedObjectId,
									 eObjectType p_linkedObjectType)
{
	return 0;
}

// 68K 0x1061aa4e AddSwitch__14CObjectManagerFUsiii9swMessageiii
// STUB: LEMBALL 0x0041b740
Switch* ObjectManager::AddSwitch(unsigned short p_id,
								 int p_x,
								 int p_y,
								 int p_z,
								 int p_message,
								 int p_legacyFirst,
								 int p_legacyLast,
								 int p_legacyAux)
{
	return 0;
}

// 68K 0x1061ab26 Process__14CObjectManagerFv
// STUB: LEMBALL 0x0041b7d0
void ObjectManager::Process()
{
}

// 68K 0x1061abea GetViewData__14CObjectManagerFP9CViewData
// STUB: LEMBALL 0x0041b830
int ObjectManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1061ace6 FindObject__14CObjectManagerFi
// STUB: LEMBALL 0x0041b8f0
GlobalGameObject* ObjectManager::FindObject(int p_id)
{
	return 0;
}

// 68K 0x1061ad72 Remove__14CObjectManagerFP17CGlobalGameObject
// STUB: LEMBALL 0x0041b9b0
void ObjectManager::Remove(GlobalGameObject* p_object)
{
}

// 68K 0x1061ade4 LoadLevel__14CObjectManagerFPUcUlUc
// STUB: LEMBALL 0x0041bbc0
void ObjectManager::LoadLevel(unsigned char* p_data, unsigned long p_length, unsigned char p_append)
{
}

// 68K 0x1061b180 ConvertVer0ToVer1__14CObjectManagerFv
// STUB: LEMBALL 0x0041bec0
void ObjectManager::ConvertVer0ToVer1()
{
}

// 68K 0x1061b1fe Receive__14CObjectManagerFUsP17CGlobalGameObjectP15CNetworkMessage
// STUB: LEMBALL 0x0041bf00
bool ObjectManager::Receive(unsigned short p_message, GlobalGameObject* p_object, NetworkMessage* p_networkMessage)
{
	return 0;
}

// 68K 0x1061a2c2 __dt__14CObjectManagerFv
ObjectManager::~ObjectManager()
{
}

// GLOBAL: LEMBALL 0x004a7830
ObjectManager* g_pGenericGroupObjectManager;
