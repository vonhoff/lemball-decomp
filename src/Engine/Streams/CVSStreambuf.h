#ifndef LEMBALL_VISOS_FOUNDATION_CVSSTREAMBUF_H
#define LEMBALL_VISOS_FOUNDATION_CVSSTREAMBUF_H

// SIZE 0x18
// VTABLE: LEMBALL 0x00498888
class CVSStreambuf {
public:
	CVSStreambuf(char* p_arg0, int p_arg1);
	virtual ~CVSStreambuf();           // vtable+0x00
	virtual void flush() = 0;          // vtable+0x04
	virtual void sputc(char p_c) = 0;  // vtable+0x08
	virtual void sputs(char* p_s) = 0; // vtable+0x0c
	CVSStreambuf();

public:
	char* m_buffer;          // 0x04
	unsigned int m_capacity; // 0x08
	char* m_cursor;          // 0x0c
	unsigned int m_length;   // 0x10
	unsigned int m_tabWidth; // 0x14
};

// SYNTHETIC: LEMBALL 0x00458e60
// CVSStreambuf::`scalar deleting destructor'

#endif
