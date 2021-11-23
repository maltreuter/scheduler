#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

#include "Process.h"
#include "Queue.h"
#include "Mfqs.h"
#include "Realtime.h"

using namespace std;

/**
 * @brief writes the process enter and exit times to a file
 * 
 * @param gantt the process enter and exit times for the cpu
 * @param output_file the name of the output file
 * @return int 
 */
int write_tuple(vector<tuple<int, int, int>> gantt, string output_file) {
	ofstream of;
	of.open(output_file);

	for(tuple<int, int, int> t : gantt)
		of << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << endl;

	of.close();
	return 0;
}

/**
 * @brief read the input file and add the processes to a list
 * 
 * @param input_file the name of the input file
 * @return vector<Process> a list of all processes
 */
vector<Process> get_processes(string input_file) {
	vector<Process> processes;
	int pid, burst, arrival, priority, deadline, io;
	ifstream infile(input_file);

	// int total_burst = 0;

	string first_line;
	getline(infile, first_line);

	while(infile >> pid >> burst >> arrival >> priority >> deadline >> io) {
		if(pid < 0 || burst <= 0 || arrival < 0 || priority < 0 || deadline <= 0 || io < 0 || deadline < arrival) {
			continue;
		} else {
			// total_burst += burst;
			processes.push_back(Process(pid, burst, arrival, priority, deadline, io));
		}
	}

	// cout << "total burst " << total_burst << endl;
	return processes;
}

/**
 * @brief sort processes by arrival time and priority
 * 
 * @param x first process to compare
 * @param y second process to compare
 * @return true if process x comes before process y
 * @return false if process y comes before process x
 */
bool sort_mfqs(Process x, Process y) {
	if(x.arrival == y.arrival) {
		return x.priority < y.priority;
	} else {
		return x.arrival > y.arrival;
	}
}

/**
 * @brief sort processes by arrival time and deadline
 * 
 * @param x first process to compare
 * @param y second process to compare
 * @return true if process x comes before process y
 * @return false if process y comes before process x
 */
bool sort_rt(Process x, Process y) {
	if(x.arrival == y.arrival) {
		return x.deadline > y.deadline;
	} else {
		return x.arrival > y.arrival;
	}
}

int main(int argc, char **argv) {
	int scheduler;
	int n_queues = 0;
	int time_quantum;
	int aging_time;
	int hard_real_time;

	int make_gantt = 0;

	string input_file;
	string output_file = "gantt.txt";

	vector<Process> processes;

	vector<tuple<int, int, int>> gantt;

	// Prompt for making a gantt chart or not
	cout << "Select 1 for Gantt chart, otherwise 0: ";
	cin >> make_gantt;

	// Input file or manual entry
	cout << "Select an input file or type manual: ";
	cin >> input_file;

	// User input
	bool stop = false;
	if(input_file.compare("manual") == 0) {
		while(!stop) {
			int pid, burst, arrival, priority, deadline, io;
			cout << "PID: ";
			cin >> pid;
			cout << "Burst: ";
			cin >> burst;
			cout << "Arrival: ";
			cin >> arrival;
			cout << "Priority: ";
			cin >> priority;
			cout << "Deadline: ";
			cin >> deadline;
			cout << "IO: ";
			cin >> io;

			processes.push_back(Process(pid, burst, arrival, priority, deadline, io));

			cout << "type 0 to add more, 1 to stop: ";
			cin >> stop;
		}
	} else {
		// Get processes from input file
		processes = get_processes(input_file);
	}

	cout << "Select a scheduler: " << endl;
	cout << "1. Multi-level Feedback Queue Scheduler" << endl;
	cout << "2. Real-Time Scheduler" << endl;
	cin >> scheduler;

	if(scheduler == 1) {
		// MFQS
		sort(processes.begin(), processes.end(), sort_mfqs);

		cout << "How many queues? ";
		cin >> n_queues;

		// Prompt for starting time quantum
		cout << "What is time quantum for first queue? ";
		cin >> time_quantum;

		// Prompt for aging strategy
		cout << "What is aging interval? ";
		cin >> aging_time;

		cout << "Running MFQS with " << n_queues << " queues and a time quantum of " << time_quantum << endl;

		// Mfqs constructor
		Mfqs mfqs = Mfqs(n_queues, time_quantum, aging_time, processes);
		gantt = mfqs.schedule();

	} else {
		// RTS
		sort(processes.begin(), processes.end(), sort_rt);

		// Prompt for hard or soft
		cout << "Select 1 for hard real time or 0 for soft real time: ";
		cin >> hard_real_time;

		cout << "Running RTS with input file: " << input_file << " and hard/soft: " << hard_real_time << endl;

		// Realtime constructor
		Realtime rt = Realtime(hard_real_time, processes);
		gantt = rt.schedule();

	}

	// Write to gantt.txt
	if(make_gantt && gantt.size() > 0) {
		write_tuple(gantt, output_file);
	}
}
