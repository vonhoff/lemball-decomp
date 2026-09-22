#ifndef LEMBALL_VIEWS_ANIMATION_LEMMINGANIMSLOADSUBOBJECT_H
#define LEMBALL_VIEWS_ANIMATION_LEMMINGANIMSLOADSUBOBJECT_H

class CCdLoadAnim;
// SIZE 0x44
class LemmingAnimsLoadSubobject {
private:
	CCdLoadAnim* m_loadAnim; // 0x08
	int m_nonCacheState;     // 0x1c
	int m_loadProgress;      // 0x20
};

#endif
