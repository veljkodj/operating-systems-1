#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;

class KernelSem;

class Semaphore {
public:

	Semaphore (int init=1);

	int val () const; // Returns the current value of the semaphore

	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n=0);

	virtual ~Semaphore ();

private:

	KernelSem* myImpl;

};
#endif //_semaphor_h_
