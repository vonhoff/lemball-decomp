#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"

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
	int m_nFrame10;
};

class GameSpriteOwnerVariantC {
public:
	unsigned char m_abData[0xcc];
	int m_nStateCC;                          // 0xcc  (event payload)
	unsigned char m_abReservedD0[0x8];
	int m_nActiveFrameD8;                    // 0xd8  (set to 1 when inactive)
	unsigned char m_abReservedDC[0x18];
	void* m_pEventQueueF4;                   // 0xf4  (event queue, null-checked)
	int m_nQueueActiveF8;                    // 0xf8  (==0 -> activate)

	int MapSpriteOwnerEventToFrameSlot(int nEvent);
	void NotifyFrameEventC(int nEvent);
	void NotifyFrameEventB(int nEvent);
};

// FUNCTION: LEMBALL 0x00468000
int GameSpriteOwnerVariantC::MapSpriteOwnerEventToFrameSlot(int nEvent)
{
	switch (nEvent) {
	case 0:
	case 3:
		return 0;
	case 1:
	case 4:
		return 1;
	case 2:
	case 5:
		return 2;
	}
	return 6;
}

// FUNCTION: LEMBALL 0x00468180
void GameSpriteOwnerVariantC::NotifyFrameEventC(int nEvent)
{
	GameSpriteOwnerStateEvent Event;
	int nFrame;

	if (m_nQueueActiveF8 == 0) {
		m_nActiveFrameD8 = 1;
	}
	if (m_pEventQueueF4 != 0) {
		nFrame = MapSpriteOwnerEventToFrameSlot(nEvent);
		Event.m_dwTimestamp04 = (int) timeGetTime();
		Event.m_nPayload08 = m_nStateCC;
		Event.m_nType00 = 0x0c;
		Event.m_pOwner0C = this;
		Event.m_nFrame10 = nFrame;
		((GameSpriteOwnerEventQueueInterface*) m_pEventQueueF4)->QueueEvent(&Event);
	}
}

// FUNCTION: LEMBALL 0x004681f0
void GameSpriteOwnerVariantC::NotifyFrameEventB(int nEvent)
{
	GameSpriteOwnerStateEvent Event;
	int nFrame;

	if (m_nQueueActiveF8 == 0) {
		m_nActiveFrameD8 = 1;
	}
	if (m_pEventQueueF4 != 0) {
		nFrame = MapSpriteOwnerEventToFrameSlot(nEvent);
		Event.m_dwTimestamp04 = (int) timeGetTime();
		Event.m_nPayload08 = m_nStateCC;
		Event.m_nType00 = 0x0b;
		Event.m_pOwner0C = this;
		Event.m_nFrame10 = nFrame;
		((GameSpriteOwnerEventQueueInterface*) m_pEventQueueF4)->QueueEvent(&Event);
	}
}

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
