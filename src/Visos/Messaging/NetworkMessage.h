#ifndef LEMBALL_VISOS_MESSAGING_NETWORKMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_NETWORKMESSAGE_H

#include "../../Common.h"

// SIZE 0x2c
// VTABLE: LEMBALL 0x004932c8
class NetworkMessage {
public:
	inline NetworkMessage(unsigned long p_messageId = 0) : m_messageId(p_messageId) { Initialise(); }
	friend class GameObjectMess;
	friend class Bullet;
	friend class Headers;
	friend class BaseFrontendProcess;
	bool Set(unsigned char* p_arg0);
	int GetDword();
	unsigned char GetByte();
	unsigned short GetWord();
	virtual bool CheckMessage(const unsigned char* p_arg0); // vtable+0x00
	virtual bool GetHeader();                               // vtable+0x04
	virtual void GetData();                                 // vtable+0x08
	virtual void AddHeader();                               // vtable+0x0c
	virtual void AddData();                                 // vtable+0x10
	virtual ~NetworkMessage();                              // vtable+0x14
	void Add(const char* p_arg0);
	void Add(const unsigned char* p_arg0, int p_arg1);
	void Add(unsigned char p_arg0);
	void Add(unsigned long p_arg0);
	void Add(unsigned short p_arg0);
	void CloseDataStream();
	void ConvertByte();
	void ConvertDword();
	void ConvertWord();
	void CopyDataStream(unsigned char* p_arg0, int p_arg1);
	void Get(const char*& p_arg0);
	void Get(const unsigned char*& p_arg0, int p_arg1);
	void Get(unsigned char& p_arg0);
	void Get(unsigned long& p_arg0);
	void Get(unsigned short& p_arg0);
	void GetCopy(char* p_arg0);
	void GetCopy(unsigned char* p_arg0, int p_arg1);
	void Initialise();
	void OpenDataStream();
	void Send(Connect* p_arg0);

protected:
	unsigned int m_messageId;        // 0x04
	unsigned char* m_buffer;         // 0x08
	unsigned char* m_bufferEnd;      // 0x0c
	unsigned int m_openDepth;        // 0x10
	unsigned int m_ownsBuffer;       // 0x14
	unsigned int m_payloadCapacity;  // 0x18
	unsigned char* m_writeCursor;    // 0x1c
	unsigned char* m_readCursor;     // 0x20
	unsigned int m_headerEnabled;    // 0x24
	unsigned int m_pendingSendCount; // 0x28
};

// SYNTHETIC: LEMBALL 0x0040ac00
// NetworkMessage::`scalar deleting destructor'


#endif

