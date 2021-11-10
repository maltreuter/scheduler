#ifndef MFQS_H
#define MFQS_H

using namespace std;

class mfqs {
	int n_queues;
	int time_quantum;
	vector<Process> processes;

	public:
		mfqs(int n_queues, int time_quantum, vector<Process> processes) {
			this->n_queues = n_queues;
			this->time_quantum = time_quantum;
			this->processes = processes;
		}
};

#endif
