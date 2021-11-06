#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	cout << "Guh Daddy" << endl;

	int scheduler;
	int n_queues = 0;
	string input_file;

	cout << "Select an input file: ";
	cin >> input_file;

	cout << "Select a scheduler: " << endl;
	cout << "1. Multi-level Feedback Queue Scheduler" << endl;
	cout << "2. Real-Time Scheduler" << endl;
	cin >> scheduler;

	if(scheduler == 1) {
		cout << "How many queues?" << endl;
		cin >> n_queues;

		cout << "Running MFQS with " << n_queues << " queues and input file: " << input_file << endl;
	} else {
		cout << "Running RTS with input file: " << input_file << endl;
	}


}
