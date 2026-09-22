#ifndef LEMBALL_AI_NAVIGATION_CNODEMANAGER_H
#define LEMBALL_AI_NAVIGATION_CNODEMANAGER_H

#include "../Base/CPt3.h" // complete type

class CNode;
// SIZE 0x0c
class CNodeManager {
public:
	CNodeManager(int p_arg0);
	CPt3 GetNodePosition(int p_node);
	void Initialise(int p_count);
	void LoadLevel(unsigned char* p_data, unsigned int p_dataSize, unsigned int p_skip);
	void Restart();
	~CNodeManager();

private:
	CNode* m_nodes; // 0x00
	int m_count;    // 0x04
	int m_capacity; // 0x08
};

#endif
