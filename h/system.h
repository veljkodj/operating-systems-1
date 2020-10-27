#ifndef _system_h_
#define _system_h_

#include "idle.h"
#include "pcb.h"

class Idle;
class PCB;

#define lock System::sysLock();
#define unlock System::sysUnlock();

typedef unsigned int Time;
typedef void interrupt (*PtrInt)(...);

class System {
public:

	static void initialize();
	static void restore();

	static void dispatch();

	static void sysLock();
	static void sysUnlock();

private:

	friend class PCB;
	friend class KernelSem;

	static void interrupt System::timer(...);

	static PtrInt oldTimer;
	static int lck;
	volatile static Time cntr;
	volatile static int disp;
	static IdleThread* idle;
	static PCB* mainThread;
	static List* queue;

};

#endif //_system_h_
