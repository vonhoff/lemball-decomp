#ifndef LEMBALL_AICOORD_H
#define LEMBALL_AICOORD_H

struct AICOORD {
	int x;
	int y;
	int z;

	AICOORD() {}
	AICOORD(const AICOORD& source);
};

#endif
