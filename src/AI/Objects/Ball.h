#ifndef LEMBALL_AI_OBJECTS_BALL_H
#define LEMBALL_AI_OBJECTS_BALL_H

#include "../../Common.h"
#include "../Base/AiCoord.h"    // complete type
#include "../Base/GameObject.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x00495460
class Ball : public GameObject {
public:
	Ball();
	virtual bool Move();                      // vtable+0x44
	virtual bool Process();                   // vtable+0x14
	virtual void HitBullet(Bullet* p_bullet); // vtable+0x54
	virtual void Delete();                    // vtable+0xc8
	virtual void Restart();                   // vtable+0x104
	virtual ~Ball();                          // vtable+0x00
	void LoadLevel(unsigned char** p_data);
	void Set(AiCoord p_start, AiCoord p_destination, int p_speed);
	void SetHeightCorrect();
	void StartMovement(unsigned char p_direction);

private:
	AiCoord m_destination;    // 0x124
	unsigned short m_speed;   // 0x130
	unsigned int m_enabled;   // 0x134
	unsigned int m_direction; // 0x138
};

// SYNTHETIC: LEMBALL 0x00422350
// Ball::`scalar deleting destructor'

#endif
