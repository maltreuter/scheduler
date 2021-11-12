#ifndef MFQS_H
#define MFQS_H

#include <queue>

using namespace std;

class mfqs {
	public:
		int n_queues;
		int time_quantum;
		vector<Process> processes;
		vector<queue<Process>> queues;

		mfqs(int n_queues, int time_quantum, vector<Process> processes) {
			this->n_queues = n_queues;
			this->queues = vector<queue<Process>>(n_queues);
			this->time_quantum = time_quantum;
			this->processes = processes;
		}

		bool queues_empty() {
			bool empty = true;
			for(int i = 0; i < queues.size(); i++) {
				if(queues[i].size() > 0) {
					empty = false;
					break;
				}
			}
			return empty;
		}

		int schedule() {
			int clock = 0;

			// while processes and !queues_empty

			queues[0].push(processes[0]);

			cout << "inserted into queue 0: " << queues[0].front().pid << endl;
			cout << "test shit: " << queues_empty() << endl;

			return 0;
		}
};

#endif
