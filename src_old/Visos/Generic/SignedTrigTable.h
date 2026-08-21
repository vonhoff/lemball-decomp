#ifndef LEMBALL_SIGNEDTRIGTABLE_H
#define LEMBALL_SIGNEDTRIGTABLE_H

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

int* LEMBALL_FASTCALL InitializeSignedTrigTable(int* pTrigTableBuffer);
int* LEMBALL_FASTCALL WriteDebugSentinelDword(int* pTarget);

#endif
