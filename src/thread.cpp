#include "thread.h"
#include "pcb.h"
#include "system.h"
#include "iostream.h"


Thread::Thread (StackSize stackSize, Time timeSlice) {

	lock;
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock;

}

void Thread::start() {

	lock;
	myPCB->start();
	unlock;

}

void Thread::waitToComplete() {

	myPCB->waitToComplete();

}

ID Thread::getId() {

	return myPCB->takeId();

}

ID Thread::getRunningId() {

	return PCB::getRunningId();

}
Thread* Thread::getThreadById(ID id) {

	return PCB::getThreadById(id);

}

Thread::~Thread() {

	waitToComplete();
	delete myPCB;

}

void dispatch() {

	System::dispatch();

}
