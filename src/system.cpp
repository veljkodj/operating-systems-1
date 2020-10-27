#include <dos.h>
#include <iostream.h>

#include "system.h"
#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.H"

int System::lck = 0;
volatile Time System::cntr = defaultTimeSlice;
volatile int System::disp = 0;
IdleThread* System::idle = 0;
PCB* System::mainThread = 0;
PtrInt System::oldTimer = 0;
List* System::queue = 0;


void System::sysLock() {
	lck++;
}

void System::sysUnlock() {
	if ((--lck == 0) && (disp == 1))
		dispatch();
}

void System::initialize() {

	#ifndef BCC_BLOCK_IGNORE
	asm cli
	oldTimer = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldTimer);
	#endif

	idle = new IdleThread();
	PCB* pcb = Scheduler::get();

	PCB::running = (volatile PCB*) new PCB(0, 0, defaultTimeSlice);
	mainThread = (PCB*)PCB::running;
	PCB::running->state = RUN;

	queue = new List();

	#ifndef BCC_BLOCK_IGNORE
	asm sti
	#endif

}

void System::restore() {

	#ifndef BCC_BLOCK_IGNORE
	asm cli
	#endif

	delete queue;

	mainThread->state = END;
	delete mainThread;

	idle->myPCB->state = END;
	delete idle;

	#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldTimer);
	asm sti
	#endif

}

void System::dispatch() {

	#ifndef BCC_BLOCK_IGNORE
	asm cli
	#endif

	disp = 1;
	timer();

	#ifndef BCC_BLOCK_IGNORE
	asm sti
	#endif

}

extern void tick();

void interrupt System::timer(...) {

	static volatile unsigned int tss, tbp, tsp;

	if (!disp) {

		tick();

		#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
		#endif

		queue->decrementCntr();
		queue->moveBackToScheduler();

		if (PCB::running->timeSlice != 0)
			cntr--;

	}

	if ((cntr == 0 && PCB::running->timeSlice != 0) || disp) {

		if (!lck) {

			disp = 0;

			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if ((PCB::running != idle->myPCB) && (PCB::running->state == RUN)) {
				PCB::running->state = READY;
				Scheduler::put((PCB*)PCB::running);
			}


			PCB::running = Scheduler::get();
			if (PCB::running == 0)
				PCB::running = idle->myPCB;

			PCB::running->state = RUN;

			cntr = PCB::running->timeSlice;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif

		} else
			disp = 1;

	}

}
