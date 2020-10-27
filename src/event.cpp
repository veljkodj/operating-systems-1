#include "kevent.h"
#include "system.h"

Event::Event(IVTNo ivtNo) {

	lock;
	myImpl = new KernelEv(ivtNo);
	unlock;

}

void Event::wait() {

	if (myImpl) myImpl->wait();

}

void Event::signal() {

	if (myImpl) myImpl->signal();

}

Event::~Event() {

	lock;
	delete myImpl;
	unlock;

}

