#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//every process has: arrival time, burst time, and pid
//i've added a flag that indicates whether the process is done or not
typedef int bool;//c doesn't have a boolean type
#define true 1
#define false 0

typedef struct Proc {
	int pid;
	int arrival_time;
	int burst_time;
	bool done;
} Proc;
//finds the location of the next process
Proc* findNextProcess(Proc** proc, int num_of_proc, int current_time) {
	int i = 0;
	int next;
	Proc* next_index = proc[0];
	bool flag = true;
	while (flag) {//find first arrival time that's bigger than the current time
		if (!proc[i]->done) {
			if (proc[i]->arrival_time > current_time) {
				next = proc[i]->arrival_time;
				next_index = proc[i];
				flag = false;
			}
		}
		i++;
		if (i >= num_of_proc&&flag)
			return next_index;
	}

	for (i = 0; i < num_of_proc;i++) {//find the next arrival time
		if (proc[i]->arrival_time > current_time && !proc[i]->done) {
			if (proc[i]->arrival_time < next) {
				next = proc[i]->arrival_time;
				next_index = proc[i];
			}
		}
	}
	return next_index;
}
//finds the location of the first process
Proc* findFirstProcess(Proc** proc, int num_of_proc) {
	int i = 0;
	int first_time = proc[0]->arrival_time;
	Proc* first = proc[0];
	while (proc[i]->done) {//find the first process that's not finished
		first_time = proc[i + 1]->arrival_time;
		first = proc[i + 1];
		i++;
	}
	for (i = 0; i < num_of_proc;i++) {
		if (!proc[i]->done) {//if the process is not done yet
			if (proc[i]->arrival_time < first_time) {
				first_time = proc[i]->arrival_time;
				first = proc[i];
			}
		}
	}
	return first;
}
/*Proc* findNextProcessInTemp(Proc** temp_proc, int num_of_temp, int id) {
	int i = 0;
	int location = -1;
	for (i = 0;i < num_of_temp;i++)
		if (temp_proc[i]->pid == id)
			location = i;
	return temp_proc[i + 1];
}*/
int findProcessLocation(Proc** proc, int num_of_proc, int pid) {
	int location;
	int i;
	for (i = 0; i < num_of_proc;i++) {
		if (proc[i]->pid == pid) {
			location = i;
			i = num_of_proc;
		}
	}
	return location;
}
bool inTheArray(Proc** proc,int num_of_proc, int element_id) {//checks if an element exists in an array
	int i;
	for (i = 0;i < num_of_proc;i++) 
		if (proc[i]->pid == element_id) 
			return true;
	return false;
	
}
void deleteProc(Proc** proc, int num_of_proc, int id) {
	int location;
	int i;
	int j;
	for (i = 0; i < num_of_proc;i++) {
		if (proc[i]->pid == id) {
			location = i;
			i = num_of_proc;
		}
	}
	for (j = location; j < num_of_proc - 1;j++)
		proc[j] = proc[j + 1];
}
//finds the location of the process with the smallest burst time
Proc* findBest(Proc** proc, int num_of_proc) {
	int best = proc[0]->burst_time;
	Proc* best_index = proc[0];
	int i;
	for (i = 0;i < num_of_proc;i++) {
		if (proc[i]->burst_time < best) {
			best = proc[i]->burst_time;
			best_index = proc[i];
		}
	}
	return best_index;
}
printWaitingTime(Proc** proc,int* finishing_time, int num_of_proc) {
	int* waiting_time = (int*)malloc(num_of_proc * sizeof(int));//the waiting time for each process
	int i;
	int j;
	for (i = 0; i < num_of_proc;i++) {
		waiting_time[i] = finishing_time[i] - proc[i]->arrival_time - proc[i]->burst_time;
	}
	for(j=0; j<num_of_proc;j++)
		printf("Process %d waiting time: %d\n", (j + 1), waiting_time[j]);
}
void FCFS(Proc** proc, int num_of_proc) {
	int* waiting_time = (int*)malloc(num_of_proc * sizeof(int));//waiting time for each process
	int time_passed = 0;
	Proc* current;
	int start_time, i;
	int temp_num = num_of_proc;
	while (temp_num > 0) {//while there are still processes to be processed
		current = findFirstProcess(proc, num_of_proc);
		if (current->arrival_time > time_passed)//if this process arrived after 
			time_passed = current->arrival_time;
		start_time = time_passed;
		time_passed += current->burst_time;
		current->done = true;
		printf("#%d:[%d]-[%d]\n", current->pid, start_time, time_passed);
		waiting_time[current->pid - 1] = time_passed - current->arrival_time - current->burst_time;
		temp_num--;
	}
	for (i = 0;i < num_of_proc;i++)
		printf("Process %d waiting time: %d\n", (i + 1), waiting_time[i]);
	free(waiting_time);//delete memory
}

void RoundRobin(Proc** proc, int num_of_proc, int time_quantom) {
	int proc_done = 0, i, time_passed = 0, time_passed2 = 0;;
	Proc* temp_next = proc[0];
	int* finishing_time = (int*)malloc(num_of_proc * sizeof(int));//the time in which the process has finished working (for each process)
	Proc** proc_copy = (Proc*)malloc(num_of_proc * sizeof(Proc*));
	Proc** temp_proc = (Proc*)malloc(num_of_proc * sizeof(Proc*));
	Proc* current = proc[0];
	int num_of_temp = 0, num_of_proc_passed = 1;
	int previous_current_id;
	int sum_burst_time = 0;
	bool time_flag = true, end_flag = false, time_flag2=false;
	copyProcesses(proc, proc_copy, num_of_proc);
	for (i = 0; i < num_of_proc;i++) {
		sum_burst_time+= proc[i]->burst_time;
	}
	while (proc_done < num_of_proc) {
		if (current->burst_time > time_quantom) {
			current->burst_time -= time_quantom;
			time_passed += time_quantom;
		}
		else {
			time_passed += current->burst_time;
			current->burst_time =0;
			current->done = true;
			proc_done++;
			finishing_time[findProcessLocation(proc, num_of_proc, current->pid)] = time_passed;
		}
		
		previous_current_id = current->pid;
		if (!current->done && !inTheArray(temp_proc, num_of_temp, current->pid)) {
			temp_proc[num_of_temp] = current;
			num_of_temp++;
		}
		deleteProc(temp_proc, num_of_temp, current->pid);
		num_of_temp--;
		if (num_of_temp == 0 && findNextProcess(proc, num_of_proc, temp_next->arrival_time)->arrival_time - time_passed > 0&& current->done) {

			temp_next = findNextProcess(proc, num_of_proc, temp_next->arrival_time);
			temp_proc[num_of_temp] = temp_next;
			num_of_temp++;
			num_of_proc_passed++;
			time_flag2 = true;
		}
		if (findNextProcess(proc, num_of_proc, temp_next->arrival_time)->arrival_time <= time_passed&&num_of_proc_passed<num_of_proc) {
			while (findNextProcess(proc, num_of_proc, temp_next->arrival_time)->arrival_time <= time_passed && num_of_proc_passed < num_of_proc) {
				temp_next = findNextProcess(proc, num_of_proc, temp_next->arrival_time);
				temp_proc[num_of_temp] = temp_next;
				num_of_temp++;
				num_of_proc_passed++;
			}
		}
		if (!current->done) {
			temp_proc[num_of_temp] = current;
			num_of_temp++;
		}
		time_flag = true;
		current = temp_proc[0];//the beggining of the queue
		if (previous_current_id != current->pid || (num_of_proc_passed == num_of_proc && num_of_temp < 2 && (time_passed == sum_burst_time||current->done))) {
			if (time_passed == sum_burst_time)
				previous_current_id = current->pid;
			printf("#%d:[%d]-[%d]\n", previous_current_id, time_passed2, time_passed);
		}
		else
			time_flag = false;
		if (time_flag)
			time_passed2 = time_passed;
		//in case there is a time space between one process to the time that's passed
		if (time_flag2) {
			time_passed += temp_next->arrival_time - time_passed;
			current = temp_next;
			time_passed2 = time_passed;
			time_flag2 = false;
		}

	}
	printWaitingTime(proc_copy, finishing_time, num_of_proc);
}
void copyProcesses(Proc** proc,Proc** proc_copy, int num_of_proc) {
	int i;
	for (i = 0; i < num_of_proc;i++) {
		proc_copy[i] = (Proc*)malloc(sizeof(Proc));
		proc_copy[i]->arrival_time = proc[i]->arrival_time;
		proc_copy[i]->burst_time = proc[i]->burst_time;
		proc_copy[i]->pid = proc[i]->pid;
	}
}
void SJF(Proc** proc, int num_of_proc) {
	static int done_proc = 0;
	static int time_passed = 0;
	int num_of_temp = 0;
	int num_of_proc_passed = 1;
	int i;
	Proc** proc_copy = (Proc*)malloc(num_of_proc * sizeof(Proc*));
	copyProcesses(proc, proc_copy, num_of_proc);
	Proc** temp_proc = (Proc*)malloc(num_of_proc * sizeof(Proc*));
	int* finishing_time = (int*)malloc(num_of_proc * sizeof(int));//the time in which the process has finished working (for each process)
	Proc* first = findFirstProcess(proc, num_of_proc);
	bool flag = false;
	int start_time = 0;
	int time_passed2 = 0;
	bool flag2 = true;
	bool flag_end = false;
	bool time_flag = true;
	int previous_first_id = -1;
	Proc* next = findNextProcess(proc, num_of_proc, first->arrival_time);
	while (done_proc < num_of_proc) {
		while ((time_passed <= next->arrival_time || flag) && flag2) {
			if (first->burst_time == 0) {//if burst time is over - it means the process is done
				if (!first->done) {
					first->done = true;
					done_proc++;
					if (num_of_temp != 0) {
						deleteProc(temp_proc, num_of_temp, first->pid);
						num_of_temp--;
					}
					finishing_time[first->pid - 1] = time_passed;
					flag = false;
					flag2 = false;
				}
			}
			else {
				if (time_passed < next->arrival_time || flag) {
					first->burst_time--;
					time_passed++;
				}
				else
					flag2 = false;
			}
		}

		flag2 = true;
		//adding the first process to the temporary list of processes that are in the appropriate interval
		if (!first->done && !inTheArray(temp_proc, num_of_temp, first->pid)) {
			temp_proc[num_of_temp] = first;
			num_of_temp++;
		}
		num_of_proc_passed++;
		if (time_passed >= next->arrival_time && !inTheArray(temp_proc, num_of_temp, next->pid) && next->burst_time > 0) {
			temp_proc[num_of_temp] = next;
			num_of_temp++;
		}
		previous_first_id = first->pid;
		if (num_of_temp == 0)
			first = next;
		//if we're at the end of the list - it means there's no 'next' process
		else
			first = findBest(temp_proc, num_of_temp);

		if (num_of_proc_passed < num_of_proc) {
			if (time_passed >= next->arrival_time)
				next = findNextProcess(proc, num_of_proc, next->arrival_time);
			else
				num_of_proc_passed--;
		}
		else {
			if (time_passed >= next->arrival_time)
				flag = true;
		}
		if (num_of_proc_passed >= num_of_proc) {
			flag_end = true;
		}
		time_flag = true;
		//printing the time
		if (previous_first_id != first->pid || (num_of_proc_passed >= num_of_proc && num_of_temp<2))
			printf("#%d:[%d]-[%d]\n", previous_first_id, time_passed2, time_passed);
		else
			time_flag = false;
		//in case there is a time space between one process to the time that's passed
		if (num_of_temp == 0 && next->arrival_time - time_passed>0)
			time_passed += next->arrival_time - time_passed;
		if (time_flag)
			time_passed2 = time_passed;
	}
	printWaitingTime(proc_copy, finishing_time, num_of_proc);
}


int main() {
	int num_of_proc, type_of_alg,time_quantom;
	int counter = 0;
	Proc** processes;
	scanf("%d", &num_of_proc);
	processes = (Proc**)malloc(num_of_proc * sizeof(Proc*));
	while (counter < num_of_proc) {   //recieve and store proccess's data 
		processes[counter] = (Proc*)malloc(sizeof(Proc));
		processes[counter]->pid = counter + 1;
		processes[counter]->done = false;
		scanf("%d %d", &processes[counter]->arrival_time, &processes[counter]->burst_time);
		counter++;
	}

	scanf("%d", &type_of_alg);//get the type of algorithm
	/*
	-- 1 - FCFS - First Come First Served
	-- 2 - Round Robin
	-- 3 - SJF Shortest Job First
	*/


	if (type_of_alg == 1)//FCFS
		FCFS(processes, num_of_proc);

	if (type_of_alg == 2) {//Round Robin
		scanf("%d", &time_quantom);
		RoundRobin(processes, num_of_proc, time_quantom);
	}
	if (type_of_alg == 3) {//SJF
		SJF(processes, num_of_proc);
	}
	scanf("%d", type_of_alg);//get the type of algorithm

}
