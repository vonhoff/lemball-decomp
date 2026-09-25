#ifndef LEMBALL_VISOS_NETWORK_CFILEBASESOCKET_H
#define LEMBALL_VISOS_NETWORK_CFILEBASESOCKET_H

class CHeaders;

// SIZE 0x0c
class CFileBaseSocket {
public:
	CFileBaseSocket();

private:
	friend class CFileConnect;
	friend class CFileBroadcast;
	friend class CFileReadSocket;
	friend class CFileWriteSocket;
	CHeaders* m_file;          // 0x00
	unsigned int m_headersOffset; // 0x04
	unsigned int m_dataOffset;    // 0x08
};

#endif
