/************ PART 2 of Programming Project ***********/
#include <stdio.h>
#include <stdlib.h>
#include "type.h"
PROC proc[NPROC];
PROC *freeList;
PROC *sleepList;
PROC *readyQueue;
PROC *running;
/****** implement these functions ******/
int tsleep(int event){ }
int twakeup(int event){ }
int texit(int status){ }
int join(int pid, int *status){ }
/****** end of implementations *********/
int init(){ } // SAME AS in PART 1
int do_exit()
{
// for simplicity: exit with pid value as status
texit(running->pid);
}
void task1(void *parm) // task1: demonstrate create-join operations
{
int pid[2];
int i, status;
//printf("task %d create subtasks\n", running->pid);
for (i=0; i<2; i++){ // P1 creates P2, P3
pid[i] = create(func, running->pid);
}
join(5, &status); // try to join with targetPid=5
for (i=0; i<2; i++){ // try to join with P2, P3
pid[i] = join(pid[i], &status);
printf("task%d joined with task%d: status = %d\n",
running->pid, pid[i], status);
}
}
void func(void *parm) // subtasks: enter q to exit
{
char c;
printf("task %d start: parm = %d\n", running->pid, parm);
while(1){
174 4 Concurrent Programming
printList("readyQueue", readyQueue);
printf("task %d running\n", running->pid);
printf("enter a key [c|s|q|j]: ");
c = getchar(); getchar(); // kill \r
switch (c){
case ’c’ : do_create(); break;
case ’s’ : do_switch(); break;
case ’q’ : do_exit(); break;
case ’j’ : do_join(); break;
}
}
}
int create(void (*f)(), void *parm)
{
int i;
PROC *p = dequeue(&freeList);
if (!p){
printf("create failed\n");
return -1;
}
p->status = READY;
p->priority = 1;
p->joinPid = 0;
p->joinPtr = 0;
for (i=1; i<13; i++)
p->stack[SSIZE-i] = 0;
p->stack[SSIZE-1] = (int)parm;
p->stack[SSIZE-2] = (int)do_exit;
p->stack[SSIZE-3] = (int)f;
p->ksp = &p->stack[SSIZE-12];
enqueue(&readyQueue, p);
printList("readyQueue", readyQueue);
printf("task%d created a new task%d\n", running->pid, p->pid);
return p->pid;
}
int main()
{
int i, pid, status;
printf("Welcome to the MT User-Threads System\n");
init();
create((void *)task1, 0);
printf("P0 switch to P1\n");
tswitch();
printf(“All tasks ended: P0 loops\n”);
while(1);
}
