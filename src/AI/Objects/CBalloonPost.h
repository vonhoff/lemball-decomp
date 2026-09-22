#ifndef LEMBALL_AI_OBJECTS_CBALLOONPOST_H
#define LEMBALL_AI_OBJECTS_CBALLOONPOST_H

#include "../Base/AiCoord.h" // complete type
#include "../Base/ObjectTypes.h"

class CAi;
class CMap;
class CTheBalloonPost;
class CViewData;
// SIZE 0x4c
class CBalloonPost {
public:
	CBalloonPost(CAi* p_arg0, CMap* p_arg1);
	bool FindPost(eObjectType p_objectType, AiCoord& p_position);
	int GetViewData(CViewData* p_viewData);
	void Process();
	void LoadLevel(unsigned char* p_data, int p_dataSize, unsigned char p_skip);
	void Restart();
	~CBalloonPost();
	void ActivatePostAtPosition(int p_x, int p_y, int p_z, eObjectType p_type);
	void DeactivatePost(CTheBalloonPost* p_post);

private:
	unsigned short m_activeMask; // 0x00
	AiCoord m_positions[4];      // 0x04
	CTheBalloonPost* m_posts[4]; // 0x34
	CAi* m_ai;                   // 0x44
	CMap* m_map;                 // 0x48
};

#endif
