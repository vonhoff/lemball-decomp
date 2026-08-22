#ifndef LEMBALL_VIEWS_ANIMATION_LEMMINGANIMSLOADSUBOBJECT_H
#define LEMBALL_VIEWS_ANIMATION_LEMMINGANIMSLOADSUBOBJECT_H

#include "../../Common.h"

// SIZE 0x44
class LemmingAnimsLoadSubobject {
public:
	// No functions mapped yet.

private:
	CdLoadAnim* m_loadAnim;       // 0x08
	unsigned int m_nonCacheState; // 0x1c
	unsigned int m_loadProgress;  // 0x20
};

#endif
