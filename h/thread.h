#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
typedef unsigned int Time;

const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2;

typedef int ID;

class PCB;

class Thread {
public:

	void start();

	void waitToComplete();

	ID getId();

	virtual ~Thread();

	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:

	friend class PCB;
	friend class System;

	Thread (
		StackSize stackSize = defaultStackSize,
		Time timeSlice = defaultTimeSlice
	);

	virtual void run() {};

private:

	PCB* myPCB;

};

void dispatch();

#endif //_thread_h_
