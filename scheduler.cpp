#include <iostream>

#include "Process.h"

using namespace std;

int main(int argc, char **argv) {
	cout << "Guh Daddy" << endl;

	int scheduler;
	int n_queues = 0;
	int round_robin;
	string input_file;

	// Input file or manual entry
	cout << "Select an input file: ";
	cin >> input_file;

	cout << "Select a scheduler: " << endl;
	cout << "1. Multi-level Feedback Queue Scheduler" << endl;
	cout << "2. Real-Time Scheduler" << endl;
	cin >> scheduler;

	if(scheduler == 1) {
		// MFQS
		cout << "How many queues?" << endl;
		cin >> n_queues;

		// Prompt for round robin or time quantum for first queue

		// Prompt for aging strategy

		cout << "Running MFQS with " << n_queues << " queues and input file: " << input_file << endl;
	} else {
		// Real Time
		// Prompt for hard or soft (giggity)

		cout << "Running RTS with input file: " << input_file << endl;
	}

	Process p(1, 2, 3, 4, 5, 6);
	cout << p.pid << endl;

}
