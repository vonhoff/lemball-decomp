#include "RawRead.h"

#include <ctype.h>

#include "../Foundation/VsFile.h"

// GLOBAL: LEMBALL 0x004a1d50
_Filet* g_pMogFile = 0;

#define kPathSeparator '/'

// 68K 0x10201774 Open__8CRawReadFPcPc
// FUNCTION: LEMBALL 0x0045bbc0
bool RawRead::Open(char* p_path, char* p_mode)
{
	g_pMogFile = VsOpen(p_path, p_mode);
	return g_pMogFile != 0;
}

// 68K 0x102017b8 InputByte__8CRawReadFv
// FUNCTION: LEMBALL 0x0045bbe0
unsigned char RawRead::InputByte()
{
	unsigned char value;
	VsRead(g_pMogFile, &value, sizeof(value));
	return value;
}

// 68K 0x102017f2 InputDword__8CRawReadFv
// FUNCTION: LEMBALL 0x0045bc40
unsigned int RawRead::InputDword()
{
	unsigned int value;
	VsRead(g_pMogFile, &value, sizeof(value));
	return value;
}

// 68K 0x1020182c NameCmp__8CRawReadFPcPc
// FUNCTION: LEMBALL 0x0045bd50
bool RawRead::NameCmp(char* p_path, char* p_name)
{
	register char* name = p_name;
	register int matched = 0;
	do {
		char pathChar = (char) toupper(*p_path);
		if (pathChar == kPathSeparator) {
			pathChar = 0;
		}
		if ((char) toupper(*name++) != pathChar) {
			break;
		}
		if (*p_path++ == '\0') {
			matched = 1;
		}
	} while (matched == 0);
	return matched;
}
