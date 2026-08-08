#ifndef LEMBALL_TYPED_RESOURCE_OBJECT_H
#define LEMBALL_TYPED_RESOURCE_OBJECT_H

#include "Visos/Generic/MogLoad.h"

struct CResBase : MogLoadStringResourceObject {
	int Direct(int* pDataCursor, void* pOwner);
	int Direct(int* pDescriptorCursor, int* pDataCursor, void* pOwner);
};

#endif
