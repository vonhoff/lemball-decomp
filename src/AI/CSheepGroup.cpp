// FUNCTION: LEMBALL 0x0041f820
int __fastcall CSheepGroup_Process(void* pObject)
{
	int iVar3 = 0;
	(*( void(**)(int)) (*(void***) pObject + 0x138 / 4))(0x18);
	if (*(int*) ((char*) pObject + 0x124) > 0) {
		int i;
		for (i = 0; i < *(int*) ((char*) pObject + 0x124); i++) {
			void* piVar1 = *(void**) ((char*) pObject + 0x128 + i * 4);
			(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
		}
	}
	((void(__fastcall*)(void*)) 0x403765)(pObject);
	((void(__fastcall*)(void*)) 0x402577)(pObject);
	return 0;
}
