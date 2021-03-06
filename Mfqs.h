#ifndef MFQS_H
#define MFQS_H

#ifdef DEBUG
#define _DEBUG(x) do { std::cerr << #x << ": "<< x << std::endl; } while(0)
#define _DEBUG2(x) x
#else
#define _DEBUG(x)
#define _DEBUG2(x)
#endif

#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <tuple>

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
		void add_processes(int clock, int &num_io);
		void check_aging(int clock);
		void do_io();
		vector<tuple<int, int, int>> schedule();
};

#endif
