#include <iostream.h>
#include <stdio.h>
#include <dos.h>

#include "event.h"
#include "system.h"
#include "ivtentry.h"
#include "kevent.h"

IVTEntry* IVTEntry::IVTP[256];

IVTEntry::IVTEntry(IVTNo _ivtN, pInterrupt _newRoutine) {

	lock;

	entryNum = _ivtN;
	newRoutine = _newRoutine;
	#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(entryNum);
	setvect(entryNum, newRoutine);
	#endif
    IVTEntry::IVTP[entryNum] = this;

    unlock;

}

IVTEntry* IVTEntry::get(IVTNo entry) {
	return IVTEntry::IVTP[entry];
}

void IVTEntry::callOldRoutine() {
	if (oldRoutine)
		(*oldRoutine)();
}

IVTEntry::~IVTEntry() {

	lock;

	#ifndef BCC_BLOCK_IGNORE
	setvect(entryNum, oldRoutine);
	#endif
	newRoutine = 0;
	oldRoutine = 0;

	unlock;

	IVTEntry::IVTP[entryNum]->myKernelEvent = 0;
	//IVTEntry::IVTP[entryNum] = 0;

}


