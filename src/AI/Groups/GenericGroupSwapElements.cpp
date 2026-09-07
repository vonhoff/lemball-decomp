#include "GenericGroup.h"

// 68K 0x1060c68c SwapElements__13CGenericGroupFP11CGameObjectP11CGameObject
// FUNCTION: LEMBALL 0x0041dfc0
void GenericGroup::SwapElements(GameObject* p_first, GameObject* p_second)
{
	int firstIndex;
	int secondIndex = -1;
	int index = 0;

	if (m_elementCount > index) {
		do {
			GameObject* element = m_elements[index];
			if (p_first == element) {
				firstIndex = index;
			}
			if (element == p_second) {
				secondIndex = index;
			}
			index++;
		} while (index < m_elementCount);
	}
	m_elements[firstIndex] = p_second;
	m_elements[secondIndex] = p_first;
}
