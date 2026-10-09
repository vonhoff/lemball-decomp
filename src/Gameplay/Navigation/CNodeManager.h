#ifndef LEMBALL_AI_NAVIGATION_CNODEMANAGER_H
#define LEMBALL_AI_NAVIGATION_CNODEMANAGER_H

#include "Gameplay/Geometry/CPt3.h"

class CNode;
struct tagLoadNodeInformation;
// SIZE 0x0c
class CNodeManager {
public:
	CNodeManager(int p_capacity);
	CPt3 GetNodePosition(int p_node);
	void GetNodeIntegerPosition(int p_node, int* p_x, int* p_y);
	int AddNode(int p_x, int p_y);
	void Initialise(int p_count);
	void LoadLevel(tagLoadNodeInformation* p_data, unsigned long p_dataSize, unsigned int p_skip);
	void Restart();
	~CNodeManager();

private:
	CNode* m_nodes; // 0x00
	int m_count;    // 0x04
	int m_capacity; // 0x08
};

#endif
