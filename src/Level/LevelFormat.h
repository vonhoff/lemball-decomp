#ifndef LEMBALL_LEVEL_LEVELFORMAT_H
#define LEMBALL_LEVEL_LEVELFORMAT_H

enum eLevelFormatVersion {
	LEVEL_VERSION_UNVERSIONED = 0,
	LEVEL_VERSION_LAST_WITHOUT_OBJECT_IDS = 1,
	LEVEL_VERSION_LAST_WITHOUT_SERIALIZED_SWITCH_DATA = 1,
	LEVEL_VERSION_WITH_LIFTS = 3,
	LEVEL_VERSION_LAST_WITHOUT_DOOR_TYPES = 2,
	LEVEL_VERSION_WITH_PLAYER_COUNTS = 4,
	LEVEL_VERSION_WITH_LIFT_ENDPOINTS = 5,
	LEVEL_VERSION_LAST_WITHOUT_MOVER_PATHS = 5,
	LEVEL_VERSION_WITH_AMMO_COUNTS = 8,
	LEVEL_VERSION_WITH_REPEATABLE_INVISIBLE_SWITCHES = 9,
	LEVEL_VERSION_WITH_INITIAL_ICE_SWITCH_STATE = 10,
	LEVEL_VERSION_CURRENT = LEVEL_VERSION_WITH_INITIAL_ICE_SWITCH_STATE
};

#define LEVEL_BLOCK_ALIGNMENT 4
#define LEVEL_BLOCK_ALIGNMENT_MASK (LEVEL_BLOCK_ALIGNMENT - 1)
#define LEVEL_BLOCK_AI 0x41492020
#define LEVEL_BLOCK_GROUND_ANIMS 0x414e494d
#define LEVEL_BLOCK_BALLS 0x42414c4c
#define LEVEL_BLOCK_BALLOON_POSTS 0x424f4f4e
#define LEVEL_BLOCK_COLLECTABLES 0x434f4c4c
#define LEVEL_BLOCK_DEFAULT_BLOX 0x44454654
#define LEVEL_BLOCK_DOORS 0x444f4f52
#define LEVEL_BLOCK_END 0x454e443f
#define LEVEL_BLOCK_ENEMY_GROUPS 0x454e4d59
#define LEVEL_BLOCK_FLAGS 0x464c4147
#define LEVEL_BLOCK_GROUND_SURFACE 0x47445346
#define LEVEL_BLOCK_OBJECTS 0x474d4f42
#define LEVEL_BLOCK_HANDS 0x48414e44
#define LEVEL_BLOCK_ICE 0x49434520
#define LEVEL_BLOCK_INVISIBLE_SWITCHES 0x494e5653
#define LEVEL_BLOCK_LASERS 0x4c415352
#define LEVEL_BLOCK_LIFTS 0x4c494654
#define LEVEL_BLOCK_MINES 0x4d494e45
#define LEVEL_BLOCK_MOVERS 0x4d4f5645
#define LEVEL_BLOCK_NAME 0x4e414d45
#define LEVEL_BLOCK_NETWORK_STARTS 0x4e455457
#define LEVEL_BLOCK_NODES 0x4e4f4445
#define LEVEL_BLOCK_PAINT_GUNS 0x5047554e
#define LEVEL_BLOCK_PLAYER_GROUPS 0x504c4153
#define LEVEL_BLOCK_PLAYER_STARTS 0x504c5331
#define LEVEL_BLOCK_ROCKETS 0x524f434b
#define LEVEL_BLOCK_SHEEP_GROUPS 0x53485047
#define LEVEL_BLOCK_SLINKIES 0x534c4e4b
#define LEVEL_BLOCK_TRAMPOLINES 0x5452414d

// SIZE 0x08
struct tagLoadBlockHeader {
	unsigned int m_type; // 0x00
	unsigned int m_size; // 0x04
};

// SIZE 0x0c
struct tagLoadEnemyData {
	unsigned short m_x;      // 0x00
	unsigned short m_y;      // 0x02
	unsigned char m_facing;  // 0x04
	unsigned char m_action0; // 0x05
	unsigned char m_rule0;   // 0x06
	unsigned char m_action1; // 0x07
	unsigned char m_rule1;   // 0x08
	unsigned char m_action2; // 0x09
	unsigned char m_rule2;   // 0x0a
};

// SIZE 0x06
struct tagLoadSheepData {
	unsigned char m_sheepCount;     // 0x00
	unsigned char m_formationIndex; // 0x01
	unsigned short m_x;             // 0x02
	unsigned short m_y;             // 0x04
};

struct tagLoadGroundSurfaceData {
	unsigned short m_width;
	unsigned short m_height;
};

struct tagLoadDefaultBlox {
	unsigned short m_objectType;
	unsigned short m_objectData;
};

// SIZE 0x02
struct tagLoadNodeInformation {
	unsigned short m_nodeCount; // 0x00
};

// SIZE 0x06
struct tagLoadNodeData {
	unsigned short m_x;              // 0x00
	unsigned short m_y;              // 0x02
	unsigned short m_neighbourCount; // 0x04
};

// SIZE 0x04
struct tagLoadNodeNeighbour {
	unsigned short m_node; // 0x00
	unsigned short m_cost; // 0x02
};

#endif
