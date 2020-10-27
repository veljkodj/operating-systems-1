#include <iostream.h>
#include "list.h"
#include "PCB.h"
#include "system.h"
#include "SCHEDULE.H"

List::List() {

	tail = head = 0;
	sum = 0;

}

void List::append(PCB* pcb) {

	Elem* newly = new Elem(pcb);

	if (!head)
		tail = head = newly;
	else
		tail = tail->next = newly;

}

void List::insert(PCB* pcb, int time) {

	if (time >= sum) {

		time -= sum;
		sum += time; // :)

		Elem* newly = new Elem(pcb, time);
		if (!head)
			head = newly;
		else
			tail->next = newly;
		tail = newly;

	}
	else {

		Elem* prev = 0;
		Elem* curr = head;
		int s = time;

		while (s - curr->time >= 0) {
			s -= curr->time;
			prev = curr;
			curr = curr->next;
		}

		Elem* newly = new Elem(pcb, s);

		if (!prev) {
			head->time -= newly->time;
			newly->next = head;
			head = newly;
		}
		else {
			curr->time -= newly->time;
			prev->next = newly;
			newly->next = curr;
		}

	}

}

void List::takeAllBackToScheduler() {

	while (head) {
		Elem* temp = head;
		head->pcb->state = READY;
		Scheduler::put(head->pcb);
		head = head->next;
		delete temp;
	}

	tail = 0;

}

void List::moveBackToScheduler() {

	if (head && head->time != 0)
		return;

	while (head && head->time == 0) {
		Elem* temp = head;
		temp->pcb->tu = 1;
		temp->pcb->state = READY;
		Scheduler::put(temp->pcb);
		head = head->next;
		delete temp;
	}

	if (!head)
		tail = head;

}

PCB* List::takeFirst() {

	if (!head) return 0;

	Elem* temp = head;
	head = head->next;
	if (!head) tail = head;
	PCB* t = temp->pcb;
	delete temp;

	return t;

}

void List::decrementCntr() {
	if (!head || (head && head->time == 0)) return;
	head->time--;
	sum--;
}

Thread* List::getThreadById(ID id) {

	Elem* curr = head;

	while (curr && curr->pcb->id != id)
		curr = curr->next;

	if (!curr)
		return 0;

	return curr->pcb->myThread;

}

void List::remove(ID id) {

	Elem* prev = 0, *curr = head;

	while (curr && curr->pcb->id != id) {
		prev = curr;
		curr = curr->next;
	}

	if (!curr) return;

	if (prev)
		prev->next = curr->next;
	else
		head = curr->next;

	delete curr;

	if (!head) tail = head;

}

void List::removeAndUpdate(ID id) {

	Elem *prev = 0, *curr = head;

	while (curr && curr->pcb->id != id) {
		prev = curr;
		curr = curr->next;
	}

	if (!curr) return;

	if (curr == tail) { // :)

		if (prev) {
			tail = prev;
			prev->next = 0;
			sum -= curr->time; // :)
			delete curr;
		} else {
			sum -= curr->time; // :)
			delete curr;
			head = tail = 0;
		}

	} else {

		curr->next->time += curr->time;
		if (prev)
			prev->next = curr->next;
		else
			head = curr->next;
		delete curr;

	}

}

List::~List() {

	if (!head) return;

	while (head) {
		Elem* help = head;
		head = head->next;
		delete help;
	}

	tail = 0;

}
