#include "iostream.h"
#include "ksem.h"
#include "pcb.h"
#include "system.h"
#include "SCHEDULE.H"

ID KernelSem::nextId = 1000;

KernelSem::KernelSem (int init) {

	id = nextId++;
	val = init;
	blockedThreads = new List();

};

int KernelSem::getVal () const {
	return val;
}

int KernelSem::wait(Time maxTimeToWait) {

	lock;

	if (--val >= 0) {
		unlock;
		return 1;
	}

	PCB::running->state = BLOCKED;
	PCB::running->mtw = maxTimeToWait;
	blockedThreads->append((PCB*)PCB::running);

	if (maxTimeToWait > 0)
		System::queue->insert((PCB*)PCB::running, maxTimeToWait);

	unlock;

	dispatch();

	if (PCB::running->tu == 1) {
		PCB::running->tu = 0;
		PCB::running->mtw = -1;
		val++;
		return 0;
	} else
		return 1;

}

int KernelSem::signal(int n) {

	lock;

	if (n < 0) {
		unlock;
		return n;
	}

	if (n == 0) {

		if (val++ < 0) {
			PCB* first = blockedThreads->takeFirst();
			if (first->mtw > 0)
				System::queue->removeAndUpdate(first->id);
			first->mtw = -1;
			Scheduler::put(first);
		}

		unlock;
		return 0;

	} else {

		PCB* first = blockedThreads->takeFirst();
		int cntr = 0;
		while (first && cntr < n) {
			if (first->mtw > 0)
				System::queue->removeAndUpdate(first->id);
			first->mtw = -1;
			Scheduler::put(first);
			cntr++;
			first = blockedThreads->takeFirst();
		}

		val += n;
		unlock;
		return cntr;

	}

}

KernelSem::~KernelSem() {

	delete blockedThreads;

}
