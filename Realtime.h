#include "Process.h"

#include <iostream>
#include <vector>

using namespace std;

class Realtime {
	public:
		bool hard;
		vector<Process> processes;
		queue<Process> run_queue;

		Realtime(bool hard, vector<Process> processes);
		~Realtime();
		int schedule();
};
