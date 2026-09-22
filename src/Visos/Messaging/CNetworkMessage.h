#ifndef LEMBALL_VISOS_MESSAGING_CNETWORKMESSAGE_H
#define LEMBALL_VISOS_MESSAGING_CNETWORKMESSAGE_H

class CConnect;

// SIZE 0x2c
// VTABLE: LEMBALL 0x004932c8
class CNetworkMessage {
public:
	inline CNetworkMessage(unsigned long p_messageId = 0)
	{
		m_messageId = p_messageId;
		Initialise();
	}
	friend class CGameObjectMess;
	friend class CGlobalGameObject;
	friend class CAI;
	friend class CBullet;
	friend class CHeaders;
	friend class CBaseFrontendProcess;
	friend class CBaseObjectManager;
	friend class CConnect;
	friend class CPBNetworkGame;
	friend class CWriteSocket;
	friend class CReadSocket;
	friend class CFileReadSocket;
	friend class CFileWriteSocket;
	friend class CFileConnect;
	friend class CFileBroadcast;
	friend class CNetworkManager;
	bool Set(unsigned char* p_data);
	int GetDWORD();
	unsigned char GetBYTE();
	unsigned short GetWORD();
	virtual bool CheckMessage(const unsigned char* p_arg0); // vtable+0x00
	virtual bool GetHeader();                               // vtable+0x04
	virtual void GetData() = 0;                             // vtable+0x08
	virtual void AddHeader();                               // vtable+0x0c
	virtual void AddData() = 0;                             // vtable+0x10
	virtual ~CNetworkMessage();                             // vtable+0x14
	void Add(const char* p_text);
	void Add(const unsigned char* p_data, int p_length);
	void Add(unsigned char p_value);
	void Add(unsigned long p_value);
	void Add(unsigned short p_value);
	void CloseDataStream();
	void ConvertBYTE();
	void ConvertDWORD();
	void ConvertWORD();
	void CopyDataStream(unsigned char* p_buffer, int p_offset);
	void Get(const char*& p_text);
	void Get(const unsigned char*& p_data, int p_length);
	void Get(unsigned char& p_value);
	void Get(unsigned long& p_value);
	void Get(unsigned short& p_value);
	void GetCopy(char* p_buffer);
	void GetCopy(unsigned char* p_buffer, int p_length);
	void Initialise();
	void OpenDataStream();
	void Send(CConnect* p_connection);

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
// CNetworkMessage::`scalar deleting destructor'

#endif
