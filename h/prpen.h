
#ifndef _prpen_h_
#define _prpen_h_

#include "ivtentry.h"
#include "kevent.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...){\
	if (newEntry##numEntry.myKernelEvent) newEntry##numEntry.myKernelEvent->signal();\
	if(callOld==1 /*&& newEntry##numEntry.oldRoutine*/)\
		newEntry##numEntry.callOldRoutine();\
	dispatch();\
}

#endif //_prpen_h_
