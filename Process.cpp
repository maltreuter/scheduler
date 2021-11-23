#include "Process.h"

Process::Process() = default;

Process::Process(int pid, int burst, int arrival, int priority, int deadline, int io) {
	this->pid = pid;
	this->burst = burst;
	this->arrival = arrival;
	this->priority = priority;
	this->deadline = deadline;
	this->io = io;
	this->og_burst = burst;
}

Process::~Process() {

}

/**
 * @brief makes a copy of the process
 * 
 * @return Process* the copied process
 */
Process* Process::clone() {
	Process *p = new Process(this->pid, this->burst, this->arrival, this->priority, this->deadline, this->io);
	p->queue = this->queue;
	return p;
}
