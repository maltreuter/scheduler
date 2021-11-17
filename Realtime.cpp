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
	int size = processes.size();
	int not_finished = 0;
	int finished = 0;
	int clock = 0;
	bool occupied = false;
	Process *running = processes[0].clone();
	set<Process, comp> run_queue;

	while(processes.size() || !run_queue.empty() || occupied) {
		// processes arrived
		bool swap = false;
		if(processes.size() && processes[0].arrival == clock && occupied && processes[0].deadline < running->deadline) {
			swap = true;
		}
		while(processes.size() && processes[0].arrival == clock) {
			run_queue.insert(processes[0]);
			processes.erase(processes.begin());
		}

		if(swap) {
			cout << "swapping processes" << endl;
			run_queue.insert(*running);
			auto it = run_queue.begin();
			auto p = *it;
			running = p.clone();
			run_queue.erase(it);
		}

		if(!occupied) {
			if(!run_queue.empty()) {
				// process running in cpu
				for(auto i : run_queue) {
					cout << "i.deadline: " << i.deadline << endl;
					cout << "clock: " << clock << endl;
					if(i.deadline > clock) {
						running = i.clone();
						occupied = true;
						cout << "add new process to cpu" << endl;
						run_queue.erase(i);
						break;
					} else {
						not_finished++;
						cout << "process didn't finish before deadline 1" << endl;
						if(!run_queue.empty()) 
						run_queue.erase(i);
						cout << "did it get here?" << endl;
					}
				}
				
				//should we add a process to cpu and decrement burst in one tick?
			}
		} else {
			if(running->deadline <= clock) {
				//is deadline inclusive?
				not_finished++;
				cout << "process didn't finish before deadline 2" << endl;
				if(hard) {
					break;
				}
				// didnt finish before deadline
				occupied = false;
			} else {
				running->burst--;
				if(running->burst == 0) {
					// process finished burst
					occupied = false;
					finished++;
					cout << "process finished" << endl;
				}
			}
		}

		clock++;
	}

	cout << "number of processes: " << size << endl;
	cout << "number of processes finished: " << finished << endl;
	cout << "number of processes not finished: " << not_finished << endl;
	cout << "finished + not finished: " << finished + not_finished << endl;

	return 0;
}
