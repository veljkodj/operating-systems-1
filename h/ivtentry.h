#ifndef _intentry_h_
#define _intentry_h_

#include "event.h"

typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:

	friend class KernelEv;

	IVTEntry(IVTNo, pInterrupt);

	void callOldRoutine();

	~IVTEntry();

	IVTNo entryNum;
	KernelEv* myKernelEvent;
	pInterrupt oldRoutine, newRoutine;

	static IVTEntry* get(IVTNo);
	static IVTEntry* IVTP[256];

};

#endif //_intentry_h_
