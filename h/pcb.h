#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "list.h"

enum State { NEW = 0, READY, RUN, END, BLOCKED};

class PCB {
private:

	friend class Thread;
	friend class List;
	friend class System;
	friend class KernelSem;
	friend class KernelEv;

	PCB(
		Thread* _myThread,
		StackSize _stackSize,
		Time _timeSlice
	);

	void start();

	void waitToComplete();

	ID takeId();

	~PCB();

	static ID getRunningId();
	static Thread* getThreadById(ID id);
	static void wrapperRun();

	/**********************************************************************/

	ID id;

	Thread* myThread;

	unsigned* stack;
	unsigned ss, bp, sp;
	StackSize stackSize;

	State state;

	Time timeSlice; // if the timeSlice is equal to 0 this pcb(thread) is executing till its very end

	List* wtc; // wait to complete list

	int tu; // time's up flag. If it is set to 1, the thread is deblocked because time is up
	int mtw; // max time to wait

	static ID nextId;
	static volatile PCB* running;
	static List* allPCBs;

};

#endif //_pcb_h_
