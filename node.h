#ifndef NODE_H_   /* Include guard */
#define NODE_H_
struct Job
{
	/*
	A 10 J=1 M=5 S=4 R=3 P=1
	The example above states that job number 1 with priority 1 arrives at time 10, requires 5 units
	of main memory, holds no more than 4 devices at any point during execution(max demand), and
	runs for 3 units of time
	*/
	int arrive_time;	//10
	int job_num;		//1
	int mem_req;		//5
	int dev_req;		//4
	int run_time;		//3
	int queue_priority; //1
	struct Job *next;
	int completion_time;//0
	int dev_owned;		//0
	int time_left;		//3
};

void insert_hold_2(struct Job * node);
void insertFIFO2(struct Job **queue, struct Job * node);
void insert_sub(int arr_t, int job_n, int mem_r, int dev_r, int run_t, int queue_p, int time);
struct Job * pop(struct Job **queue);
void printList(struct Job *queue);
void insert_ready(struct Job * node);
void pop_sub();
void pop_wait();
void fill_ready();
void insert_fin(struct Job * node);

#endif // NODE_H_

