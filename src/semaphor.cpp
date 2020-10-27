#include "semaphor.h"
#include "ksem.h"
#include "system.h"

Semaphore::Semaphore(int init) {

	lock;
	myImpl = new KernelSem(init);
	unlock;

}

int Semaphore::val() const {

	lock;
	int ret = myImpl->getVal();
	unlock;

	return ret;

}

int Semaphore::wait(Time maxTimeToWait) {

	int ret = myImpl->wait(maxTimeToWait);

	return ret;

}

int Semaphore::signal(int n) {

	lock;
	int ret = myImpl->signal(n);
	unlock;

	return ret;

}

Semaphore::~Semaphore () {
	lock;
	delete myImpl;
	unlock;
}
