#ifndef LEMBALL_GAMEVIEW_LOADING_LOADANIMCALLBACKS_H
#define LEMBALL_GAMEVIEW_LOADING_LOADANIMCALLBACKS_H

// SIZE 0x04
// VTABLE: LEMBALL 0x00497c94
class CCdLoadAnimDraw {
public:
	virtual void Draw() = 0;
};

// SIZE 0x04
// VTABLE: LEMBALL 0x00497c98
class CCdLoadAnimProgress {
public:
	virtual void Draw(short p_progress) = 0;
};

#endif
