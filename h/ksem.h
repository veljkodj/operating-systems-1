#ifndef _ksem_h_
#define _ksem_h_

#include "semaphor.h"
#include "list.h"

typedef int ID;

class KernelSem {
private:

	friend class Semaphore;

	KernelSem (int init=1);

	int getVal () const;

	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n=0);

	virtual ~KernelSem();

	/**********************************************************************/

	static ID nextId;
	ID id;

	int val;
	List* blockedThreads;

};

#endif //_ksem_h_
