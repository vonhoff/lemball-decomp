#include "AI/CIce.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

// FUNCTION: LEMBALL 0x0042d550
void CIce::Switch(void)
{
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
}
