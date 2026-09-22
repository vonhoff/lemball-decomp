#include "CRawRead.h"

#include "../Foundation/VsFile.h"

#include <ctype.h>

// GLOBAL: LEMBALL 0x004a1d50
_Filet* g_pMogFile = 0;

#define kPathSeparator '/'

// FUNCTION: LEMBALL 0x0045bbc0
bool CRawRead::Open(char* p_path, char* p_mode)
{
	g_pMogFile = VsOpen(p_path, p_mode);
	return g_pMogFile != 0;
}

// FUNCTION: LEMBALL 0x0045bbe0
unsigned char CRawRead::InputByte()
{
	unsigned char value;
	VsRead(g_pMogFile, &value, sizeof(value));
	return value;
}

// FUNCTION: LEMBALL 0x0045bc40
unsigned int CRawRead::InputDword()
{
	unsigned int value;
	VsRead(g_pMogFile, &value, sizeof(value));
	return value;
}

// FUNCTION: LEMBALL 0x0045bd50
bool CRawRead::NameCmp(char* p_path, char* p_name)
{
	char* name = p_name;
	int matched = 0;
	char* path = p_path;
	do {
		char pathChar = (char) toupper(*path);
		if (pathChar == kPathSeparator) {
			pathChar = 0;
		}
		int nameChar = *name++;
		if ((char) toupper(nameChar) == pathChar) {
			if (*path++ == '\0') {
				matched = 1;
			}
		}
		else {
			break;
		}
	} while (matched == 0);
	return matched;
}
