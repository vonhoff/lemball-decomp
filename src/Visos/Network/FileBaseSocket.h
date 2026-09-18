#ifndef LEMBALL_VISOS_NETWORK_FILEBASESOCKET_H
#define LEMBALL_VISOS_NETWORK_FILEBASESOCKET_H

class Headers;

// SIZE 0x0c
class FileBaseSocket {
public:
	FileBaseSocket();

private:
	friend class FileConnect;
	friend class FileBroadcast;
	friend class FileReadSocket;
	friend class FileWriteSocket;
	Headers* m_file;           // 0x00
	unsigned int m_unk0x04;    // 0x04
	unsigned int m_dataOffset; // 0x08: packet-data offset after the serialized headers
};

#endif
