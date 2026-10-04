#ifndef LEMBALL_VIEWS_ANIMATION_CLEMMINGANIMSLOADVIEW_H
#define LEMBALL_VIEWS_ANIMATION_CLEMMINGANIMSLOADVIEW_H

class CCDLoadAnim;
// SIZE 0x44
class CLemmingAnimsLoadView {
private:
	CCDLoadAnim* m_loadAnim; // 0x08
	int m_nonCacheState;     // 0x1c
	int m_loadProgress;      // 0x20
};

#endif
