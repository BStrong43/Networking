#ifndef __ANIMAL3D_NETWORKEVENT_H
#define __ANIMAL3D_NETWORKEVENT_H

#include "a3_NetGameMessages.h"

class a3_NetworkEvent
{
protected:
	a3_NetGameMessages mId;
	
	a3_NetworkEvent(a3_NetGameMessages id)
		:mId(id) {}
public:
	virtual ~a3_NetworkEvent() = default;
	
	a3_NetGameMessages getId() const { return mId; }
};

#endif