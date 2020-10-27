#ifndef _kevent_h_
#define _kevent_h_

#include "pcb.h"
#include "event.h"
#include "ivtentry.h"

class KernelEv {
public:

	void signal();

private:

	friend class Event;

	KernelEv(IVTNo);

	void wait();

	~KernelEv();

	int value;
	IVTNo entryNum;
	PCB* owner;
	//Event* event;

};

#endif // _kevent_h_
