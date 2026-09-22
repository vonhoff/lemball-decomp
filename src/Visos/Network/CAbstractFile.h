#ifndef LEMBALL_VISOS_NETWORK_CABSTRACTFILE_H
#define LEMBALL_VISOS_NETWORK_CABSTRACTFILE_H

// Descriptive name for the abstract file interface at 0x0049a5e8.
// SIZE 0x0c
// VTABLE: LEMBALL 0x0049a5e8
class CAbstractFile {
public:
	~CAbstractFile()
	{
		if (m_filename != 0) {
			operator delete(m_filename);
		}
	}
	CAbstractFile() : m_filename(0) { m_closed = 1; }
	virtual bool Open(const char* p_filename, unsigned char p_mode, int p_create) = 0; // vtable+0x00
	virtual bool Create(const char* p_filename, unsigned char p_mode) = 0;             // vtable+0x04
	virtual bool CheckExists(const char* p_filename) = 0;                              // vtable+0x08
	virtual bool Close() = 0;                                                          // vtable+0x0c
	virtual bool Delete() = 0;                                                         // vtable+0x14
	virtual bool Delete(const char* p_filename) = 0;                                   // vtable+0x10
	virtual bool Write(const unsigned char* p_data, int p_size) = 0;                   // vtable+0x18
	virtual bool Read(unsigned char* p_data, int p_size) = 0;                          // vtable+0x1c
	virtual bool Seek(int p_position) = 0;                                             // vtable+0x20
	virtual unsigned int GetFileSize() = 0;                                            // vtable+0x24
	virtual int Tell() = 0;                                                            // vtable+0x28
	virtual bool Lock(unsigned int p_offset, unsigned int p_length) = 0;               // vtable+0x2c
	virtual bool UnLock(unsigned int p_offset, unsigned int p_length) = 0;             // vtable+0x30
	virtual int GetLastError() = 0;                                                    // vtable+0x34
	virtual char* GetFilename() = 0;                                                   // vtable+0x38

protected:
	char* m_filename;      // 0x04
	unsigned int m_closed; // 0x08
};

#endif
