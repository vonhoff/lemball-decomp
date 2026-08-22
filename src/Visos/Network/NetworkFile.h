#ifndef LEMBALL_VISOS_NETWORK_NETWORKFILE_H
#define LEMBALL_VISOS_NETWORK_NETWORKFILE_H

#include "../../Common.h"

// SIZE 0x14
// VTABLE: LEMBALL 0x0049a570
class NetworkFile {
public:
	virtual bool CheckExists(const char* p_filename);                                        // vtable+0x08
	virtual bool Close();                                                                    // vtable+0x0c
	virtual bool Create(const char* p_filename, unsigned char p_mode);                       // vtable+0x04
	virtual bool Delete();                                                                   // vtable+0x14
	virtual bool Delete(const char* p_filename);                                             // vtable+0x10
	virtual bool Lock(unsigned int p_offset, unsigned int p_length);                         // vtable+0x2c
	virtual bool Open(const char* p_filename, unsigned char p_mode, unsigned char p_create); // vtable+0x00
	virtual bool Read(unsigned char* p_data, int p_size);                                    // vtable+0x1c
	virtual bool Seek(int p_position);                                                       // vtable+0x20
	virtual bool UnLock(unsigned int p_offset, unsigned int p_length);                       // vtable+0x30
	virtual bool Write(const unsigned char* p_data, int p_size);                             // vtable+0x18
	virtual char* GetFilename();                                                             // vtable+0x38
	virtual int GetLastError();                                                              // vtable+0x34
	virtual int Tell();                                                                      // vtable+0x28
	virtual unsigned int GetFileSize();                                                      // vtable+0x24

private:
	char* m_filename;        // 0x04
	unsigned int m_closed;   // 0x08
	void* m_handle;          // 0x0c
	unsigned int m_position; // 0x10
};

#endif
