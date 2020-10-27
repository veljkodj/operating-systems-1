#include <dos.h>

#include "pcb.h"
#include "system.h"
#include "SCHEDULE.h"
#include "iostream.h"

ID PCB::nextId = -2; // -2 is for idle and -1 is for mainThread; user threads starts at 0
volatile PCB* PCB::running = 0;
List* PCB::allPCBs = new List();

PCB::PCB(Thread* _myThread, StackSize _stackSize, Time _timeSlice) {

	id = nextId++;

	myThread = _myThread;

	timeSlice = _timeSlice;

	state = NEW;

	wtc = new List();

	stackSize = _stackSize;
	if (stackSize > 65536) stackSize = 65536;
	stackSize /= sizeof(unsigned);

	stack = new unsigned[stackSize];
	stack[stackSize - 1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 2] = FP_SEG(wrapperRun);
	stack[stackSize - 3] = FP_OFF(wrapperRun);
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = sp;
	#endif

	allPCBs->append(this);

	tu = 0;
	mtw = -1; //initially

}

void PCB::start() {

	state = READY;

	Scheduler::put(this);

}

void PCB::wrapperRun() {

	running->myThread->run();

	lock;
	running->state = END;
	running->wtc->takeAllBackToScheduler();
	unlock;

	dispatch();

}

void PCB::waitToComplete() {

	if (this == running || this->state == END || this->state == NEW || this == System::idle)
		return;

	lock;
	running->state = BLOCKED;
	wtc->append((PCB*)running);
	unlock;

	dispatch();

}

ID PCB::takeId() {

	return id;

}

ID PCB::getRunningId() {

	return ((PCB*)running)->id;

}

Thread* PCB::getThreadById(ID id) {

	return allPCBs->getThreadById(id);

}

PCB::~PCB() {

	delete stack;
	delete wtc;

}
