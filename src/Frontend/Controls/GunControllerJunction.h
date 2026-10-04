#ifndef LEMBALL_FRONTEND_CONTROLS_GUNCONTROLLERJUNCTION_H
#define LEMBALL_FRONTEND_CONTROLS_GUNCONTROLLERJUNCTION_H

#define GUN_SIDE_LEFT 0
#define GUN_SIDE_RIGHT 1
#define GUN_JUNCTION_LEFT GUN_SIDE_LEFT
#define GUN_JUNCTION_RIGHT GUN_SIDE_RIGHT
#define GUN_JUNCTION_BOTH 2
#define GUN_JUNCTION_UNASSIGNED 3

enum {
	GUN_JUNCTION_COORDINATE_UNASSIGNED = -1,
	GUN_JUNCTION_INDEX_NOT_FOUND = -1,
	GUN_CONTROLLER_ABOVE_TOP_BOUNDARY_Y = -1
};

// SIZE 0x20
struct GunControllerJunction {
	int m_leftX;                 // 0x00
	int m_y;                     // 0x04
	int m_rightX;                // 0x08
	int m_direction;             // 0x0c
	unsigned int m_leftMessage;  // 0x10
	unsigned int m_rightMessage; // 0x14
	void* m_leftBinding;         // 0x18
	void* m_rightBinding;        // 0x1c
};

#endif
