extern void* g_pActiveManagedEntityOwner;

// FUNCTION: LEMBALL 0x0041fad0
void __fastcall CSheep_HitMine(void* pObject)
{
	((void(__fastcall*)(void*, int)) 0x402f22)(g_pActiveManagedEntityOwner, 0x96);
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
}
