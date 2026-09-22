#ifndef LEMBALL_AI_OBJECTS_CBALL_H
#define LEMBALL_AI_OBJECTS_CBALL_H

#include "../Base/AiCoord.h"     // complete type
#include "../Base/CGameObject.h" // complete type

// SIZE 0x13c
// VTABLE: LEMBALL 0x00495460
class CBall : public CGameObject {
public:
	CBall();
	virtual bool Move();                       // vtable+0x44
	virtual bool Process();                    // vtable+0x14
	virtual void HitBullet(CBullet* p_bullet); // vtable+0x54
	virtual void Delete();                     // vtable+0xc8
	virtual void Restart();                    // vtable+0x104
	void LoadLevel(unsigned char*& p_data);
	void Set(AiCoord p_start, AiCoord p_destination, int p_speed);
	void SetHeightCorrect();
	void StartMovement(unsigned int p_direction);

private:
	AiCoord m_destination;    // 0x124
	unsigned short m_speed;   // 0x130
	unsigned int m_enabled;   // 0x134
	unsigned int m_direction; // 0x138
};

// SYNTHETIC: LEMBALL 0x00422350
// CBall::`scalar deleting destructor'

#endif
