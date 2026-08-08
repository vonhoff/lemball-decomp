#include "AI/CSheepGroup.h"

// FUNCTION: LEMBALL 0x0041f820
int CSheepGroup::Process(void)
{
	int iVar3 = 0;
	(*( void(**)(int)) (*(void***) ((void**) m_pVtable00) + 0x138 / 4))(0x18);
	if (m_nGroupElementCount124 > 0) {
		int i;
		for (i = 0; i < m_nGroupElementCount124; i++) {
			void* piVar1 = m_apChildObjects128[i];
			(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
		}
	}
	((void(__fastcall*)(void*)) 0x403765)(this);
	((void(__fastcall*)(void*)) 0x402577)(this);
	return 0;
}
