#include "Realtime.h"

using namespace std;

Realtime::Realtime(bool hard, vector<Process> processes) {
	this->hard = hard;
	this->processes = processes;
}

Realtime::~Realtime() {

}

struct comp {
	template<typename P>
	bool operator()(const P &l, const P &r) const {
		return l.deadline < r.deadline;
	}
};

int Realtime::schedule() {
	cout << "scheduling" << endl;
	int not_finished = 0;
	int clock = 0;
	bool occupied = false;
	Process *running = processes[0].clone();
	set<Process, comp> run_queue;

	while(processes.size() || !run_queue.empty() || occupied) {
		// processes arrived
		bool swap = false;
		if(processes[0].arrival == clock && processes[0].deadline < running->deadline) {
			swap = true;
		}
		while(processes.size() && processes[0].arrival == clock) {
			run_queue.insert(processes[0]);
			processes.erase(processes.begin());
		}

		if(swap) {
			run_queue.insert(*running);
			auto it = run_queue.begin();
			auto p = *it;
			running = p.clone();
			run_queue.erase(it);
		}

		if(!occupied) {
			if(!run_queue.empty()) {
				// process running in cpu
				auto it = run_queue.begin();
				auto p = *it;
				running = p.clone();
				run_queue.erase(it);

				occupied = true;
				cout << "add new process to cpu" << endl;
			}
		} else {
			if(running->burst == 0) {
				// process finished burst
				occupied = false;
				cout << "process finished" << endl;
			} else if(running->deadline == clock) {
				not_finished++;
				cout << "process didn't finish before deadline" << endl;
				if(hard) {
					break;
				}
				// didnt finish before deadline
				occupied = false;
			} else {
				running->burst--;
			}
		}

		clock++;
	}

	cout << "number of processes not finished: " << not_finished << endl;

	return 0;
}
