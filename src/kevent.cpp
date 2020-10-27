#include "system.h"
#include "event.h"
#include "kevent.h"
#include "ivtentry.h"
#include "pcb.h"
#include "SCHEDULE.h"

KernelEv::KernelEv(IVTNo entry) {

	entryNum = entry;
	owner = (PCB*)PCB::running;
	value = 0; // event did not occur
	IVTEntry::get(entryNum)->myKernelEvent = this;

}

void KernelEv::wait() {

	if (owner != PCB::running ) return;

	if (value == 0) {
		owner->state = BLOCKED;
		dispatch();
	} else
		value = 0;

}

void KernelEv::signal() {

	lock;

	if(value == 0 && owner->state == BLOCKED) {
		owner->state = READY;
		Scheduler::put(owner);
	} else
		value = 1;

	unlock;

}

KernelEv::~KernelEv() {

	lock;
	if(IVTEntry::get(entryNum)) IVTEntry::get(entryNum)->myKernelEvent=0;
	unlock;

}
