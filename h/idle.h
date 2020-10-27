#ifndef _idle_h_
#define _idle_h_

#include "Thread.h"

class Thread;

class IdleThread: public Thread {
protected:

	friend class System;

	IdleThread();

	void run();

};

#endif //_idle_h_
