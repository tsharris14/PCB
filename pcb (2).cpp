/*
Kyle Ferrigan, John Wolf, TaCoya Harris
CSCE 420
Dr. Wu
November 12, 2021
OS Process Managment Implementation
*/

#include<iostream>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>
#include <queue>


#define stateQueue queue<Process>
#define LT(x, y) (((x) < (y)) ? (1) : (0))

//#include "stateHandler.cpp" todo fix
using namespace std;

//PCB
//pid, process state, PC, registers, process priority,
//CPU time, i/o info (files, devices, etc)

int pctr = 0;
int sClock = 0; //running time

class Process{
private:
	int pid;
	int burstTime; //for RR scheduling
	int remBurstTime;
	int bound; //cpu bound (longer cpu bursts) or i/o bound (shorter cpu bursts)
	int arrivalTime;
	int pc; //program counter
	int priority;
	//general purpose registers
	int gpr1, gpr2, gpr3, gpr4, gpr5, gpr6, gpr7, gpr8;
	int state; //0 - new, 1 - ready, 2 - waiting, 3 - running, 4 - terminated
	int randInc;//used for more randomized arrival times
	string text; //executable instructions


public:
	//constructor
	Process(){
		randInc = rand() % 11 + 5; //random increment between 5 and 10 after previous process arrival time
		arrivalTime = sClock + rand() % 11;//randomize arrival time
		sClock++;//system timer
		pid = pctr;//unique id
		pctr++;//counts number of processes
		state = 0;
		pc = 0;

		priority = rand() % 5 + 1; //1-5
		bound = rand() % 2; // 0 - cpu bound or 1 - i/o bound


		if (bound == 1) {//cpu bound

		}
		if (bound == 1){

			burstTime = rand() % 5 + 1; //1 - 5 ms bursts
		}
		else{//i/o bound
			burstTime = rand() % 6 + 5; //5 - 10 ms bursts
		}
		remBurstTime = burstTime;//for scheduler
	}
	Process(const Process &obj){//copy constructor
		pid = obj.pid;
		burstTime = obj.pid;
		remBurstTime = obj.remBurstTime;
		priority = obj.priority;
		arrivalTime = obj.arrivalTime;
		pc = obj.pc;
		state = obj.state;
	}

	//~Process();//destructor

	//setters
	void setPid(int x){
		pid = x;
	}
	void setRemBurstTime(int x){
		remBurstTime = x;
	}
	//getters
	int getPid(){
		return pid;
	}
	int getBurstTime(){
		return burstTime;
	}
	int getRemBurstTime() const{
		return remBurstTime;
	}
	int getPriority(){
		return priority;
	}
	int getBound(){
		return bound;
	}
	int getArrivalTime(){
		return arrivalTime;
	}
	int getPC(){
		return pc;
	}
	int getGpr1(){
		return gpr1;
	}
	int getState(){
		return state;
	}
	int getPctr(){
		return pctr;
	}
	int getClock(){
		return sClock;
	}

	//state changing conditions
	void changeState(int c, Process &p){
		switch (c) {
			case 1: {//new -> ready
				state = 1;

				//ready(p); //add process to ready queue todo fix
				//remove from new queue
				cout << "Process " + to_string(p.getPid()) + " new -> ready" << '\n';
				break;
			}
			case 2:{//ready -> running
				//cpu scheduler selected process
				state = 2;
				//running(p); todo fix
				//remove from ready queue
				cout << "Process " + to_string(p.getPid()) + " ready -> running" << '\n';
				break;
			}

			case 3: { //running -> waiting
				state = 3;
				//waiting(p);//add process to waiting queue todo fix
				cout << "Process " + to_string(p.getPid()) + " ready -> waiting" << '\n';
				break;
			}

			case 4: { //running -> terminated
				state = 4;
				//terminated(p); todo fix
				cout << "Process " + to_string(p.getPid()) + " running -> terminated" << '\n';
				break;
			}
		}
	}
};



template <class T>
class node{
public:

	T v; // data
	node<T> *next; // pointer to next node

	node(T x)
	{
		v = x;
		next = NULL;
	}

};

template <class T>
class pqueue{

	// the '<T>' after node indicate that the
	// datatype of template of node should be T
	node<T> *start;
	node<T> *end;

public:

	pqueue()
	{
		start = end = NULL;
	}

	// returns true if queue is empty
	bool empty()
	{
		return start==NULL;
	}

	// insert v at the end of the queue
	void push(T v)
	{
		node<T> *temp = new node<T>(v);
		if(empty())
		{
			start = end = temp;
		}
		else
		{
			end->next = temp;
			end = temp;
		}

	}

	// retuns the first element of the queue
	T front()
	{
		if(empty())
			cout << "empty" << endl;
		else
			return start->v;
	}

	// removes the first element of the queue
	void pop()
	{
		if(empty())
		{
			cout<<"Queue is Empty"<<endl;
		}
		else if(start==end)
		{
			// there is only 1 element in the queue
			delete start;
			start = end = NULL;
		}
		else
		{
			// if there are more than 1 element in the queue
			node<T> *temp = start;
			start = start->next;
			delete temp;
		}
	}
};




struct States {stateQueue states[3];};

template<typename T> int getQueueLength(queue<T>* q) {return q->size();} // gets number of elements in queue
template<typename T> bool isQueueEmpty(queue<T>* q) {return q->empty();} // is queue empty?
template<typename T> void swapQueueContents(queue<T>* a, queue<T> b) {a->swap(*b);} // swap contents of queues a and b
template<typename T> queue<T> createQueue() // create a new queue
{
	queue<T> q;
	return q;
}
template<typename T> void enqueue(queue<T>* q, T t) {q->push(t);} // add t to end of queue
template<typename T> T dequeue(queue<T>* q) // remove head from queue and returns it if queue is not empty, else it returns a default constructed T
{
	if(isQueueEmpty<T>(q))
	{
		T* dval = new T;
		T data = *dval;
		delete(dval);
		return data;
	}
	T data = q->front();
	q->pop();
	return data;
}
template<typename T> T peekQueue(queue<T>* q) // peek first element of queue -- MAYBE SHOULD BE CONST
{
	if(isQueueEmpty<T>(q))
	{
		T* dval = new T;
		T data = *dval;
		delete(dval);
		return data;
	}
	return q->front();
}
template<typename T> T getFirstElement(queue<T>* q) {return q->front();}
template<typename T> T getLastElement(queue<T>* q) {return q->back();}

//======================================================================================================

States createStateQueues()
{
	States s;
	for(int i = 0; i < 3; i++) {s.states[i] = createQueue<Process>();}
	return s;
}
bool enqueueState(States* s, const int state, const Process& p){
	if(LT(state, 0) || !LT(state, 3)){
		return 0;
	}
	enqueue(&s->states[state], p);
	return 1;
}
Process dequeueState(States* s, const int state)
{
	if(LT(state, 0) || !LT(state, 3) || isQueueEmpty<Process>(&s->states[state]))
	{
		Process* dval = new Process, p = *dval;
		delete(dval);
		return p;
	}
	return dequeue(&s->states[state]);
}
bool isStateEmpty(States* s, const int state)
{
	if(LT(state, 0) || !LT(state, 3)) {return 0;}
	return isQueueEmpty(&s->states[state]);
}
bool allStatesEmpty(States* s)
{
	int count = 0;
	while(count < 3 && isQueueEmpty(&s->states[count])) {count++;}
	return count == 3;
}
Process peekState(States* s, const int state)
{
	if(LT(state, 0) || !LT(state, 3) || isQueueEmpty(&s->states[state]))
	{
		Process* dval = new Process, p = *dval;
		delete(dval);
		return p;
	}
	return peekQueue(&s->states[state]);
}

//======================================================================================================

/*
global variables
*/
States gl_states = createStateQueues();
Process* current = nullptr;

void waiting(Process&);

/*
create process and push it onto the new queue
*/
void createProcess() {enqueueState(&gl_states, 0, Process());} // change to accept process as an input maybe?

/*
enqueue process into ready queue -- pointless?
*/
void ready(Process& p)
{
	if(&p == current) {current = nullptr;} // if interrupted
}

/*
run a process and block others that try to run
*/
void running(Process& p)
{
	if(current)
	{
		waiting(p);
		cout << "Process " << p.getPid() << "was blocked" << endl;
		return;
	}
	current = &p;
	// run process here
}

/*
put a process in the waiting queue
*/
void waiting(Process& p)
{
	// implement a random interrupt here
	if(&p == current) {current = nullptr;} // if interrupted
	enqueueState(&gl_states, 1, p);
}

/*
terminate a process and put it in the finished queue
*/
void terminated(Process& p)
{
	if(&p == current) {current = nullptr;} // process has finished
	enqueueState(&gl_states, 2, p); // queue of processes that have finished
}

/*
returns true if all of the queues in a States struct are empty
*/
bool areStatesEmpty() {return allStatesEmpty(&gl_states);}

void moveProcess(const int& state, int target = -1)
{
	/*
	add context switch here?
	*/
	if(state == 2) {return;}
	if(target == -1) {enqueueState(&gl_states, state+1, dequeueState(&gl_states, state));} // increment by one
	else {enqueueState(&gl_states, target, dequeueState(&gl_states, state));} // move to target
	return;
}
Process peekAt(const int& state)
{
	if(LT(state, 0) || !LT(state, 3))
	{
		Process* dval = new Process, p = *dval;
		delete(dval);
		return p;
	}
	return peekState(&gl_states, state);
}
Process dequeueAt(const int& state)
{
	if(LT(state, 0) || !LT(state, 3) || isStateEmpty(&gl_states, state))
	{
		Process* dval = new Process, p = *dval;
		delete(dval);
		return p;
	}
	return dequeueState(&gl_states, state);
}
bool checkEmpty(const int state)
{
	if(LT(state, 0) || !LT(state, 3)) {return 0;}
	return isStateEmpty(&gl_states, state);
}


/*
handles a simulated error, holds the process for a random number of milliseconds before placing it into the ready queue
*/
void handleInterrupt(const Process& p)
{
	cout << "Handling interrupt for process" << endl;
	float start = time(0);
	int r = rand()%5+1;
	float t = ((float) r);
	while(difftime(time(0), start) < t) {}
	cout << "I/O request complete" << endl;
	enqueueState(&gl_states, 0, p);
	return;
}


class Scheduler{
private:
	int quantumValue; //quantum value for RR
	pqueue<struct Process> *rQueue; //Pointer to ready queue in pcb

	int executing(Process curProc, int *procStatus, int maxTime){//will return amount of time taken if interrupted
		//int maxTime input is the max time that can be executed for this process it is the minimum of quantum value or remaining burst length,
		int rnd = rand() % 100 + 1; //1 to 100
		int rnd2 = rand() % 100 + 1;//1 to 100
		int interruptionChance = 30; //chance process will be interrupted
		int terminatingChance = 40; //subset of above, percent chance that an interruption is a terminating one
		if (rnd<interruptionChance){//determines if process will be interrupted

			if (rnd2<terminatingChance){//process interrupted by terminating command
				printf("Process %d Interrupted by a Terminating Command\n", curProc.getPid());
				*procStatus = 1;
				maxTime = rand() % maxTime; //randomize amount of time processed
				return maxTime;
			}
			else{//process interrupted by normal interrupt
				printf("Process %d Interrupted Normally\n", curProc.getPid());
				*procStatus = 2;
				maxTime = rand() % maxTime;//randomize amount of time processed
				return maxTime;
			}
		}

		else{//no interruption process completes successfully
			return maxTime;
		}
	}

public:
	// Constructor
	Scheduler(pqueue<Process> *readyQueueIn, int quantumValueIn){
		quantumValue = quantumValueIn;
		rQueue = readyQueueIn;
	}

	//Round Robin Logic
	void tick(int* sysTime){//runs for a unit of quantum value
		if (!rQueue->empty()){//make sure queue is not empty
			Process curProc = rQueue->front();
			int increment; //The amount of time that will be added to system clock
			int procStatus = 0; // 0 = normal, 1 = Terminating Interrupt, 2 = Other Interrupt
			if (*sysTime >= curProc.getArrivalTime()) {//if at least at the processes' arrival time, might not be necessary as long as the ready queue is not corrupted (eg. has arrival times that are later than current time)
				curProc.changeState(2, curProc);//process is now running
				printf("Process %d Now Running\n", curProc.getPid());
				if (curProc.getRemBurstTime() < quantumValue) {//if quantum is larger than the remaining burst time
					increment = curProc.getRemBurstTime();//take note of remaining burst time
					increment = executing(curProc, &procStatus, increment);//simulate possible process interrupts
					curProc.setRemBurstTime(0); //set remaining burst time to 0
					*sysTime = *sysTime + increment; //update system clock to reflect time taken
					if (procStatus < 2){
						curProc.changeState(4, curProc);//process is now terminated
						terminated(curProc);//terminate process
						rQueue->pop();//remove old proc from front
					}
					else{
						curProc.changeState(3,curProc);//process waiting
						waiting(curProc);
						rQueue->push(curProc);//add finished proc to end of queue
						rQueue->pop();//remove old proc from front
					}
				}
				else {//quantum smaller or equal to remaining burst time
					increment = executing(curProc,&procStatus,quantumValue);//simulate possible process interrupts
					curProc.setRemBurstTime((curProc.getRemBurstTime() - increment)); //remove quantum from remaining burst time
					*sysTime = *sysTime + increment;
					if (curProc.getRemBurstTime() <= 0){//if finished with process and burst time is 0
						curProc.changeState(4, curProc);//process is now terminated
						terminated(curProc);//terminate process
						rQueue->pop();//remove old proc from front
					}
					else{//if process still has remaining burst time
						curProc.changeState(1, curProc);//process is now back to being ready
						ready(curProc);
						rQueue->push(curProc);//put modified proc to rear of queue
						rQueue->pop();//remove from front of queue
					}
				}
			}
			else{
				printf("Process Arrival Time is in the future!, waiting...\n");
				rQueue->push(rQueue->front());
				rQueue->pop();
				sClock++;
			}
		}
		else{
			printf("Queue is empty!\n");
		}
	}//end tick
};//end class

int main(){
	srand (time(NULL));
	pqueue<Process> q;

	Process p0;
	Process p1;
	Process p2;
	Process p3;
	Process p4;
	Process p5;
	Process p6;
	Process p7;
	Process p8;
	Process p9;

	q.push(p0);
	q.push(p1);
	q.push(p2);
	q.push(p3);
	q.push(p4);
	q.push(p5);
	q.push(p6);
	q.push(p7);
	q.push(p8);
	q.push(p9);

	Scheduler n(&q,2);
	for (int i = 0; i<20; i++){
		if(!q.empty()){
			cout<<"Queue Front: "<<"pid: "<<q.front().getPid()<< " state: " << q.front().getState() << " burst: " << q.front().getRemBurstTime() << " arrival: " << q.front().getArrivalTime() << endl;
		}
		n.tick(&sClock);
	}
}