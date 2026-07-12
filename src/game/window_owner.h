#ifndef LEMBALL_GAME_WINDOW_OWNER_H
#define LEMBALL_GAME_WINDOW_OWNER_H

#include "engine/common.h"

extern int g_nLiveWindowOwnerBaseCount;
extern void *g_pRootZrleGeometryOwnerRegistry;
extern void *g_pRootGeometryOwnerVtable;
extern void *g_pWindowOwnerBaseVtable;

void *LEMBALL_FASTCALL ConstructRootGeometryOwner(void *pOwner);

#endif
