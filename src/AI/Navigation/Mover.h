#ifndef LEMBALL_SCAFFOLD_AI_NAVIGATION_MOVER_H
#define LEMBALL_SCAFFOLD_AI_NAVIGATION_MOVER_H

#include "../../Common.h"
#include "../Base/GlobalGameObject.h" // complete type
#include "../Base/Move3d.h"           // complete type

// SIZE 0x1a0
// VTABLE: LEMBALL 0x00496a30
class Mover : public GlobalGameObject {
public:
	Mover();
	bool GetOn(GameObject* p_object);
	bool IsAt(int p_x, int p_y, int& p_height);
	bool IsOn(const AiCoord& p_position);
	unsigned int FindObjectsOnTopOfMe();
	virtual bool Process();    // vtable+0x14
	virtual void DoActivate(); // vtable+0x10c
	virtual void Restart();    // vtable+0x104
	virtual ~Mover();          // vtable+0x00
	void Initialise();
	void MoveObjects(int p_deltaX, int p_deltaY, int p_deltaZ);
	void Set(unsigned short p_id, int p_pathId, unsigned char p_movementMode, int p_startNode, int p_nodeCount);
	void SetPos();
	void SetUpNextNode(unsigned int p_time);
	void StopObjectsMoving();
	void Switch();
	void VerifyObjects();

private:
	unsigned int m_active;          // 0x138
	unsigned int m_moving;          // 0x13c
	unsigned int m_findOccupants;   // 0x140
	unsigned int m_movementMode;    // 0x144
	unsigned int m_switchRequested; // 0x148
	Move3d m_motion;                // 0x14c
	int m_startNode;                // 0x168
	int m_nodeCount;                // 0x16c
	int m_currentNode;              // 0x170
	int m_objectCount;              // 0x174
	GameObject* m_objects[10];      // 0x178
};

#endif
