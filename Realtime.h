#ifndef REALTIME_H
#define REALTIME_H

#ifdef DEBUG
#define _DEBUG(x) do { std::cerr << #x << ": "<< x << std::endl; } while(0)
#define _DEBUG2(x) x
#else
#define _DEBUG(x)
#define _DEBUG2(x)
#endif

#include "Process.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <fstream>
#include <tuple>

using namespace std;

class Realtime {
	public:
		bool hard;
		vector<Process> processes;
		deque<Process> run_queue;

		Realtime(bool hard, vector<Process> processes);
		~Realtime();
		int find_earliest_deadline(int clock, int &not_finished);
		vector<tuple<int, int, int>> schedule();
};

#endif
