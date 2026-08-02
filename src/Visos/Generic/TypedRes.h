#ifndef LEMBALL_TYPED_RESOURCE_OBJECT_H
#define LEMBALL_TYPED_RESOURCE_OBJECT_H

#include "Visos/Generic/MogLoad.h"

struct MogLoadTypedResourceObject : MogLoadStringResourceObject {
	int LoadTypedResourceObjectFromStreamDescriptor(int* pDataCursor, void* pOwner);
	int LoadTypedResourceObjectFromBufferDescriptor(int* pDescriptorCursor, int* pDataCursor, void* pOwner);
};

#endif
