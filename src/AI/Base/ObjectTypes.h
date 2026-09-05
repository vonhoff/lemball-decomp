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

// Reconstructed labels for terrain object types. These describe the verified
// x86-backed collision silhouettes and height profiles; they are not recovered
// original symbols.
// Direction suffixes use the map convention: +X is SE and +Y is SW.
#define TERRAIN_FLAT_LOW 0x202
#define TERRAIN_FLAT_HIGH 0x206
#define TERRAIN_FLAT_MIDDLE 0x207
#define TERRAIN_SLOPE_SW_STEEP 0x208
#define TERRAIN_FLAT_DEFAULT 0x209
#define TERRAIN_FLAT_SPECIAL 0x20a
#define TERRAIN_SLOPE_SE_STEEP 0x20d
#define TERRAIN_SLOPE_SW_SHALLOW 0x20e
#define TERRAIN_SLOPE_SE_SHALLOW 0x20f
#define TERRAIN_SPECIAL_ONLY 0x210
#define TERRAIN_FLAT_VARIANT_214 0x214
#define TERRAIN_FLAT_VARIANT_215 0x215
#define TERRAIN_FLAT_VARIANT_216 0x216
#define TERRAIN_FLAT_VARIANT_217 0x217
#define TERRAIN_FLAT_VARIANT_219 0x219
#define TERRAIN_FLAT_VARIANT_21A 0x21a

#endif
