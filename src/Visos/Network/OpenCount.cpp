#include "OpenCount.h"

// 68K 0x101076fc AddData__10COpenCountFv
// FUNCTION: LEMBALL 0x0047b950
void OpenCount::AddData()
{
	Add(m_openCount);
}

// 68K 0x10107730 GetData__10COpenCountFv
// FUNCTION: LEMBALL 0x0047b960
void OpenCount::GetData()
{
	Get(m_openCount);
}

// 68K 0x101067cc __dt__10COpenCountFv
OpenCount::~OpenCount()
{
}

