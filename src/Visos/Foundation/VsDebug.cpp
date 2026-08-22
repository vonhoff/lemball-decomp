#include "VsDebug.h"

// 68K 0x10213c3a _VSExit__Fi
// STUB: LEMBALL 0x00459970
void VsExit(int p_exitCode)
{
}

// 68K 0x10205a70 _VSRELassert
// STUB: LEMBALL 0x0045b8e0
void VsRelAssert(const char* p_reason, const char* p_file, unsigned int p_line)
{
}

// 68K 0x1010ff88 WriteDebugString2File__FPc
// STUB: LEMBALL 0x004728b0
void WriteDebugString2File(char* p_text)
{
	// STRING: LEMBALL 0x004a2a14 "debug.out"
}

// 68K 0x1011006c _RAWOUT_DebugString__FPc
// STUB: LEMBALL 0x00472910
bool RawOutDebugString(char* p_text)
{
	// STRING: LEMBALL 0x004a27c8 "_RAWOUT_DebugString"
	return 0;
}

// 68K 0x101100a4 _RAWOUT_ErrorString__FPc
// STUB: LEMBALL 0x00472980
bool RawOutErrorString(char* p_text)
{
	// STRING: LEMBALL 0x004a27dc "_RAWOUT_ErrorString"
	return 0;
}

// 68K 0x101100dc _RAWOUT_SysString__FPc
// STUB: LEMBALL 0x004729f0
bool RawOutSysString(char* p_text)
{
	// STRING: LEMBALL 0x004a27f0 "_RAWOUT_SysString"
	return 0;
}

// 68K 0x10100d4a DisplayRelAssert__FPvPvUi
// STUB: LEMBALL 0x004734f0
void DisplayRelAssert(const char* p_reason, const char* p_file, unsigned int p_line)
{
	// STRING: LEMBALL 0x004a2b0c "Release Version Assertion Failure\n"
	// STRING: LEMBALL 0x004a2b30 "Reason: "
	// STRING: LEMBALL 0x004a2b40 "In File: "
	// STRING: LEMBALL 0x004a2b4c "At Line No.: "
	// STRING: LEMBALL 0x004a2b5c "Error"
}
