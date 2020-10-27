#ifndef _list_h_
#define _list_h_

#include "pcb.h"

class Thread;

typedef int ID;

class List {
private:

	struct Elem {

		PCB* pcb;
		int time;
		Elem* next;

		Elem(PCB* p, int t = -1, Elem* n = 0) : pcb(p), time(t), next(n) {}

		~Elem() {}

	};

	Elem *head, *tail;
	int sum;

public:

	List();

	void append(PCB* pcb);
	void insert(PCB* pcb, int time);

	void takeAllBackToScheduler();
	void moveBackToScheduler();
	PCB* takeFirst();

	void decrementCntr();

	Thread* getThreadById(ID id);

	void remove(ID id);
	void removeAndUpdate(ID id);

	~List();

};

#endif // _list_h_
