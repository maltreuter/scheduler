#include "Process.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <deque>

using namespace std;

class Realtime {
	public:
		bool hard;
		vector<Process> processes;
		deque<Process> run_queue;

		Realtime(bool hard, vector<Process> processes);
		~Realtime();
		int find_earliest_deadline(int clock, int &not_finished);
		int schedule();
};

