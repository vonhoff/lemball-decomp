#ifndef LEMBALL_VISOS_FOUNDATION_CVSSTREAMBUF_H
#define LEMBALL_VISOS_FOUNDATION_CVSSTREAMBUF_H

// SIZE 0x18
// VTABLE: LEMBALL 0x00498888
class CVsStreambuf {
public:
	CVsStreambuf(char* p_arg0, int p_arg1);
	virtual ~CVsStreambuf();           // vtable+0x00
	virtual void Flush() = 0;          // vtable+0x04
	virtual void Sputc(char p_c) = 0;  // vtable+0x08
	virtual void Sputs(char* p_s) = 0; // vtable+0x0c
	CVsStreambuf();

public:
	char* m_buffer;          // 0x04
	unsigned int m_capacity; // 0x08
	char* m_cursor;          // 0x0c
	unsigned int m_length;   // 0x10
	unsigned int m_tabWidth; // 0x14
};

// SYNTHETIC: LEMBALL 0x00458e60
// CVsStreambuf::`scalar deleting destructor'

#endif
