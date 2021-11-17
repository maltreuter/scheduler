#include "Process.h"

#include <iostream>
#include <vector>

using namespace std;

class Realtime {
	public:
		bool hard;
		vector<Process> processes;
		vector<Process> run_queue;

		Realtime(bool hard, vector<Process> processes);
		~Realtime();
		bool sort_deadline(Process x, Process y) const;
		int schedule();
};
