#ifndef __ANIMAL3D_NETWORKINBOUNDEVENT_H
#define __ANIMAL3D_NETWORKINBOUNDEVENT_H

class a3_NetworkInboundEvent
{
protected:
	a3_NetGameMessages messageId;

	a3_NetworkInboundEvent(a3_NetGameMessages id);
public:
	virtual void execute() = 0;
};

#endif