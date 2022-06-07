# scheduler
CS 452 Operating Systems - Assignment 2 - Process Scheduler Simulation

This assignment simulates two different process scheduling algorithms: Realtime Scheduler, and Multilevel Feedback Queue Scheduler.
Our code was stress tested with a file containing 1 million processes (1m_processes) and another file with 500k processes (500k). We optimized our code by removing processes from the end of our vector (O(1)) instead of from the front of the vector (O(n)).

We also included a gannt chart that will work for smaller test files. Run GanntChart.java in the GanntChart directory after running a simulation.

## Usage
Compile, clean up
```
make
make debug /* includes all couts */
make clean
```

Run
```
./scheduler

/* Select an input file or enter processes manually */
/* Choose 0 or 1 for Realtime or MFQS algorithms */
/* Realtime: 0 or 1 for hard or soft (both run earliest deadline first) */
/* MFQS: Choose number of queues, time quantum for the first queue, and the aging interval */
```

## To Do
- [x] Menu
- [x] Multi level feedback queue
- [x] Real time
- [x] Statistics
- [x] User entered processes
- [x] Gannt chart



guh\
buh, even, if you will
