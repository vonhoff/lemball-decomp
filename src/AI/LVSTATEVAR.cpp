#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

struct ManagedEntityStateView {
	unsigned char m_abReserved00[0xbc];
	unsigned short m_nStateVariantBC;

	void RequestManagedEntityStateId(int nStateId);
	void RequestManagedEntityStateId(int nStateId, int nVariant);
};

// FUNCTION: LEMBALL 0x00416e00
void ManagedEntityStateView::RequestManagedEntityStateId(int nStateId, int nVariant)
{
	m_nStateVariantBC = nVariant;
	RequestManagedEntityStateId(nStateId);
}
