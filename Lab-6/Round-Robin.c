#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;

int main() {
    int n, time, remain, flag = 0, time_quantum;
    int wait_time = 0, turnaround_time = 0;
    Process *processes;

    printf("Enter Total Process : \t ");
    scanf("%d", &n);
    remain = n;

    processes = (Process*)malloc(n * sizeof(Process));

    for(int i = 0; i < n; i++) {
        processes[i].id = i;
        printf("Enter Arrival Time for Process Number %d: ", i);
        scanf("%d", &processes[i].arrival_time);
        printf("Enter Burst Time for Process Number %d: ", i);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("Enter Time Quantum:\t");
    scanf("%d", &time_quantum);
    printf("\n\nProcess\t|Arrival Time|Turnaround Time|Waiting Time\n\n");

    int i;
    for(time = 0, i = 0; remain != 0;) {
        if(processes[i].remaining_time <= time_quantum && processes[i].remaining_time > 0) {
            time += processes[i].remaining_time;
            processes[i].remaining_time = 0;
            flag = 1;
        } else if(processes[i].remaining_time > 0) {
            processes[i].remaining_time -= time_quantum;
            time += time_quantum;
        }
        if(processes[i].remaining_time == 0 && flag == 1) {
            remain--;
            processes[i].waiting_time = time - processes[i].arrival_time - processes[i].burst_time;
            processes[i].turnaround_time = time - processes[i].arrival_time;
            printf("P[%d]\t|\t%d\t|\t%d\t|\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].turnaround_time, processes[i].waiting_time);
            wait_time += processes[i].waiting_time;
            turnaround_time += processes[i].turnaround_time;
            flag = 0;
        }
        if(i == n-1)
            i = 0;
        else if(processes[i+1].arrival_time <= time)
            i++;
        else
            i = 0;
    }

    printf("\nAverage Waiting Time= %f\n", wait_time*1.0/n);
    printf("Avg Turnaround Time = %f\n", turnaround_time*1.0/n);

    // Print Gantt Chart
    printf("\nGantt Chart:\n");
    for(int i = 0; i < n; i++) {
        printf(" P[%d] ", processes[i].id);
        if (i < n-1) printf("->");
    }
    printf("\n");

    for(int i = 0; i < n; i++) {
        printf(" %d ", processes[i].arrival_time);
        if (i < n-1) printf("   ");
    }
    printf(" %d\n", time);

    return 0;
}