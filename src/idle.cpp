#include <iostream.h>

#include "idle.h"
#include "system.h"

IdleThread::IdleThread() : Thread(defaultStackSize, 1) {}

void IdleThread::run() {

	for (int i = 0; i < 10; i++) {

		for (int j = 0; j < 30000; j++)
			for (int k = 0; k < 30000; k++);

		if (i == 9) i = -1;

	}

}
