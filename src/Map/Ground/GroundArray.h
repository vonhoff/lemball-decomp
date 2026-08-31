#ifndef LEMBALL_MAP_GROUND_GROUNDARRAY_H
#define LEMBALL_MAP_GROUND_GROUNDARRAY_H

#include "../../Common.h"
#include "Ground.h"

// SIZE 0x10
class GroundArray {
public:
	unsigned short GetZ(int p_x, int p_y);
	void Clear();
	void SetCollision(int p_x, int p_y, int p_collision);

	friend class Bullet;
	friend class PlayerLemming;
	friend class Tower;
	friend class Enemy;
	friend class Ammo;
	friend class Key;
	friend class Balloon;
	friend class Catapult;
	friend class Collectable;
	friend class Crate;
	friend class Duplicator;
	friend class Map;
	friend class Ball;

private:
	unsigned int m_reserved; // 0x00
	Ground* m_ground;        // 0x04
	int m_width;             // 0x08
	int m_height;            // 0x0c
};

#endif
