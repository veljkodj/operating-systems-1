#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:

	Event (IVTNo ivtNo);

	void wait ();

	~Event();

protected:

	friend class KernelEv;

	void signal();

private:

	KernelEv* myImpl;

};

#endif //_event_h_
