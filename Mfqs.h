#ifndef MFQS_H
#define MFQS_H

#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include "Process.h"
#include "Queue.h"

using namespace std;

class Mfqs {
	public:
		int n_queues;
		int time_quantum;
		int aging_time;
		vector<Process> processes;
		vector<Queue> queues;
		vector<Process> io;

		Mfqs(int n_queues, int time_quantum, int aging_time, vector<Process> processes);
		~Mfqs();
		int queues_empty();
		int add_to_queue_n(Process p, int n);
		int schedule();
};

#endif
