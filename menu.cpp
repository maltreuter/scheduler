#include <iostream>
#include <fstream>
#include <vector>

#include "Process.h"
#include "mfqs.h"

using namespace std;

vector<Process> get_processes(string input_file) {
	vector<Process> processes;
	int pid, burst, arrival, priority, deadline, io;
	ifstream infile(input_file);

	string first_line;
	getline(infile, first_line);

	while(infile >> pid >> burst >> arrival >> priority >> deadline >> io) {
		processes.push_back(Process(pid, burst, arrival, priority, deadline, io));
	}

	return processes;
}

int main(int argc, char **argv) {
	cout << "Guh Daddy" << endl;

	int scheduler;
	int io;
	int n_queues = 0;
	int time_quantum;
	int hard_real_time;
	string input_file;
	vector<Process> processes;

	// Input file or manual entry
	cout << "Select an input file: ";
	cin >> input_file;
	processes = get_processes(input_file);

	/*
	for(Process p : processes)
		cout << "pid = " << p.pid << " arrival = " << p.arrival << endl;
	*/

	cout << "Select a scheduler: " << endl;
	cout << "1. Multi-level Feedback Queue Scheduler" << endl;
	cout << "2. Real-Time Scheduler" << endl;
	cin >> scheduler;

	// Prompt for I/O timing
	cout << "How many clock cycles for i/o interrupt? ";
	cin >> io;

	if(scheduler == 1) {
		// MFQS
		cout << "How many queues? ";
		cin >> n_queues;

		// Prompt for starting time quantum
		cout << "What is time quantum for first queue? ";
		cin >> time_quantum;

		// Prompt for aging strategy

		cout << "Running MFQS with " << n_queues << " queues and " << time_quantum << " time quantum" << endl;

		// mfqs constructor
		mfqs guh = mfqs(n_queues, time_quantum, processes);
	} else {
		// Real Time

		// Prompt for hard or soft (giggity)
		cout << "Select 1 for hard real time or 0 for soft real time: ";
		cin >> hard_real_time;

		cout << "Running RTS with input file: " << input_file << " and hard/soft: " << hard_real_time << endl;
	}
}
