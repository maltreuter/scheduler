#include "Process.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class Realtime {
	public:
		bool hard;
		vector<Process> processes;

		Realtime(bool hard, vector<Process> processes);
		~Realtime();
		int schedule();
};
