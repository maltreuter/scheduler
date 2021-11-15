#ifndef QUEUE_H
#define QUEUE_H

#include <queue>

using namespace std;

class Queue {
	public:
		int time_quantum;
		queue<Process> q;
		bool rr = 1;
};

#endif
