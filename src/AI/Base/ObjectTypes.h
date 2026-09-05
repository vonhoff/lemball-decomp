#ifndef LEMBALL_AI_BASE_OBJECTTYPES_H
#define LEMBALL_AI_BASE_OBJECTTYPES_H

// Values established by x86-backed constructors and ObjectManager::Add.
// Keep eObjectType as int (Common.h); these names do not change its ABI.
#define OBJECT_BULLET 3
#define OBJECT_CATAPULT 4
#define OBJECT_AMMO 5
#define OBJECT_TOWER 0xd
#define OBJECT_CRATE 0x11
#define OBJECT_SWITCH 0x14
#define OBJECT_TRAP_DOOR 0x18
#define OBJECT_DUPLICATOR 0x1c

// Reconstructed labels for terrain object types. These combine the verified
// x86-backed renderer resources with the collision silhouettes and height
// profiles; they are not recovered original symbols.
// Direction suffixes use the map convention: +X is SE and +Y is SW.
#define TERRAIN_TREE 0x202
#define TERRAIN_BLOX_1 0x206
#define TERRAIN_BLOX_2 0x207
#define TERRAIN_BLOX_3_SLOPE_SW_STEEP 0x208
#define TERRAIN_BLOX_4 0x209
#define TERRAIN_BLOX_5 0x20a
#define TERRAIN_BLOX_6 0x20b
#define TERRAIN_BLOX_7 0x20c
#define TERRAIN_BLOX_8_SLOPE_SE_STEEP 0x20d
#define TERRAIN_BLOX_14_SLOPE_SW_SHALLOW 0x20e
#define TERRAIN_BLOX_15_SLOPE_SE_SHALLOW 0x20f
#define TERRAIN_ANIM 0x210
#define TERRAIN_FLAME 0x215
#define TERRAIN_ELECTRIC 0x216
#define TERRAIN_EMBERS 0x217
#define TERRAIN_CONVEYOR_VARIANT_A 0x219
#define TERRAIN_CONVEYOR_VARIANT_B 0x21a

#endif
