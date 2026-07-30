#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/WIN32.H"

extern "C" DWORD WINAPI timeGetTime(void);

class GameSpriteOwnerEventQueueInterface {
public:
	virtual void Reserved0(void) = 0;
	virtual void Reserved1(void) = 0;
	virtual void QueueEvent(void* pEvent) = 0;
};

struct GameSpriteOwnerStateEvent {
	short m_nType00;
	short m_nReserved02;
	int m_dwTimestamp04;
	int m_nPayload08;
	void* m_pOwner0C;
	int m_nReserved10;
};

// FUNCTION: LEMBALL 0x00468260
void LEMBALL_FASTCALL NotifySpriteOwnerVariantCEvent0D(void* pObject)
{
	GameSpriteOwnerStateEvent Event;

	if (*(void**) ((char*) pObject + 0xf4) != 0) {
		Event.m_dwTimestamp04 = (int) timeGetTime();
		Event.m_nPayload08 = *(int*) ((char*) pObject + 0xcc);
		Event.m_pOwner0C = pObject;
		Event.m_nType00 = 0x0d;
		((GameSpriteOwnerEventQueueInterface*) *(void**) ((char*) pObject + 0xf4))->QueueEvent(&Event);
	}
}

// FUNCTION: LEMBALL 0x004682b0
void LEMBALL_FASTCALL NotifySpriteOwnerVariantCEvent0E(void* pObject)
{
	GameSpriteOwnerStateEvent Event;

	if (*(void**) ((char*) pObject + 0xf4) != 0) {
		Event.m_dwTimestamp04 = (int) timeGetTime();
		Event.m_nPayload08 = *(int*) ((char*) pObject + 0xcc);
		Event.m_pOwner0C = pObject;
		Event.m_nType00 = 0x0e;
		((GameSpriteOwnerEventQueueInterface*) *(void**) ((char*) pObject + 0xf4))->QueueEvent(&Event);
	}
}
