#include <kernel.h>
#include <stdio.h>
#include <ctype.h>
#include <thread.h>
#define SLOTX 6
#define CYCLEX 4
#define SLOT_T 5000

int cycle=0, slot=0;

//Prototypes of all processes which will be uses for creating threads.
void* task1(void*);
void* task2(void*);
void* task3(void*);
void* burn(void*);

//Table full of integer value each corresponding to a task, with 4 being burn.
int ttable[SLOTX][CYCLEX] =
{
      {1, 1, 2, 2},
      {3, 3, 3, 3},
      {1, 1, 2, 2},
      {3, 3, 3, 3},
      {2, 2, 1, 1},
      {2, 2, 4, 4}
};

shellcmd xsh_schedule(int nargs, char* args[]){

	//The name of each task is declared because it is a required argument of create().
	char* t1Name = "task1";
	char* t2Name = "task2";
	char* t3Name = "task3";
	char* burnName = "burn";

//	tid_typ t1ID = create(&task1, 8178, 1, t1Name, 1, (void*) 1);
//	tid_typ t2ID = create(&task2, 8178, 1, t2Name, 1, (void*) 2);
//	tid_typ t3ID = create(&task3, 8178, 1, t3Name, 1, (void*) 3);
//	tid_typ burnID = creare(&burn, 8178, 1, burnName, 1, (void*) 4);

	fprintf(stdout,"\nStarting tasks.\n");
	fprintf(stdout,"\nFrame size = 4.\nHyper-period = 24.\n");
	int index = 1;

	while(index > 0){
//		//We go through each element of the table and match it with its corresponding process, then create a thread.
		for(slot=0;slot<SLOTX;slot++){
			for(cycle = 0;cycle<CYCLEX;cycle++){
				if(ttable[slot][cycle] == 1){
					ready(create(&task1, 8178, 1, t1Name, 1, (void*) index), 1);
					//Thread will sleep increasingly more to not disturb memory.
					sleep(index);
				}
				else if(ttable[slot][cycle] == 2){
					ready(create(&task2, 8178, 1, t2Name, 1, (void*) index), 1);
					sleep(index);
				}
				else if(ttable[slot][cycle] == 3){
					ready(create(&task3, 8178, 1, t3Name, 1, (void*) index), 1);
					sleep(index);
				}
				else{
					ready(create(&burn, 8178, 1, burnName, 1, (void*) index), 1);
					sleep(index);
				}
				index++;
			}
		}

	}
	//Reset index once it reaches a certain value so that the period of sleep for a thread does not become too long.
	if (index == 240){
		index = 1;
//		fprintf(stdout, "TEST");
	}
        return 0;
}

//Task corresponding to the number 1 of the table.
void* task1(void* x){
	//We make an int from thr typecast of the function's parameter and use it in the printing of the task name. 
	int task;
	task = (int) x;

	fprintf(stdout, "\nRunning task 1.\n");

	//This line is so that everytime we are on a new frame the line is printed at the appropriate moment because of the
	//nature of the threads.
	if((task)%24 == 0){
		fprintf(stdout, "\nStarting new hyper-period.\n");
	}
	//Same as previous line but for hyper-period.
	if((task)%4 == 0){
                fprintf(stdout, "\nStarting new frame.\n");
        }

	return NULL;
}

//Task corresponding to the number 2 of the table.
void* task2(void* x){
	int task;
	task = (int) x;

	fprintf(stdout, "\nRunning task 2.\n");

	if((task)%24 == 0){
		fprintf(stdout, "\nStarting new hyper-period.\n");
	}
	if((task)%4 == 0){
                fprintf(stdout, "\nStarting new frame.\n");
        }

	return NULL;
}

//Task corresponding to the number 3 of the table.
void* task3(void* x){
	int task;
	task = (int) x;

	fprintf(stdout, "\nRunning task 3.\n");

	if((task)%24 == 0){
		fprintf(stdout, "\nStarting new hyper-period.\n");
	}
	else if((task)%4 == 0){
                fprintf(stdout, "\nStarting new frame.\n");
        }

	return NULL;
}

//Task corresponding to the number 4 of the table.
void* burn(void* x){
	fprintf(stdout, "\nBurn.\n");
	int task;
	task = (int) x;
	if((task)%24 == 0){
		fprintf(stdout, "\nStarting new hyper-period.\n");
	}
	else if((task)%4 == 0){
                fprintf(stdout, "\nStarting new frame.\n");
        }

	return NULL;
}
