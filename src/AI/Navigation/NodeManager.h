#ifndef LEMBALL_SCAFFOLD_AI_NAVIGATION_NODEMANAGER_H
#define LEMBALL_SCAFFOLD_AI_NAVIGATION_NODEMANAGER_H

#include "../../Common.h"
#include "../Base/Pt3.h" // complete type

// SIZE 0x0c
class NodeManager {
public:
	NodeManager(int p_arg0);
	Pt3 GetNodePosition(int p_node);
	void Initialise(int p_count);
	void LoadLevel(unsigned char* p_data, unsigned int p_dataSize, unsigned char p_skip);
	void Restart();
	~NodeManager();

private:
	Node* m_nodes;  // 0x00
	int m_count;    // 0x04
	int m_capacity; // 0x08
};

#endif
