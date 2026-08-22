#ifndef LEMBALL_CONTROL_GAME_DEMO_H
#define LEMBALL_CONTROL_GAME_DEMO_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type

// SIZE 0x58
// VTABLE: LEMBALL 0x00493100
class Demo : public BaseQueueHandler {
public:
	Demo(int p_arg0);
	bool LoadBuffer();
	bool SendNextPacket(int p_packetIndex);
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual ~Demo();                            // vtable+0x04
	void CleanUp();
	void GameIsOver();
	void GetUserPacket(unsigned char* p_data, unsigned long& p_size);
	void Process();
	void Reset();
	void SetDemoMode(unsigned char p_enabled);

private:
	unsigned char* m_buffer;     // 0x10
	int m_sourceId;              // 0x14
	int m_bytesRemaining;        // 0x18
	unsigned char* m_readCursor; // 0x1c
	char* m_filePath;            // 0x20
	ResBin* m_resource;          // 0x24
	int m_currentResourceId;     // 0x28
	int m_firstResourceId;       // 0x2c
	int m_resourceCount;         // 0x30
	PvWnd* m_window;             // 0x34
	unsigned int m_startTime;    // 0x38
	unsigned int m_duration;     // 0x3c
	short m_offsetX;             // 0x40
	short m_offsetY;             // 0x42
	int m_packetIndex;           // 0x44
	unsigned int m_state48;      // 0x48
	unsigned int m_demoMode;     // 0x4c
	unsigned int m_gameOver;     // 0x50
	unsigned int m_state54;      // 0x54
};

// Confirmed class-scoped globals.
extern int g_nDemoMode;
#endif
