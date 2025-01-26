#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turnaround_time;
} Process;

int main() {
    int n, total_waiting_time = 0, total_turnaround_time = 0;
    Process *processes;

    printf("Enter Total Number of processes : ");
    scanf("%d", &n);

    processes = (Process*)malloc(n * sizeof(Process));

    printf("\n Enter Process Arrival Time, Burst Time and Priority \n"); 
    for(int i = 0; i < n; i++) {
        processes[i].id = i;
        printf("P[%d] Arrival Time: ", i);
        scanf("%d", &processes[i].arrival_time);
        printf("P[%d] Burst Time: ", i);
        scanf("%d", &processes[i].burst_time);
        printf("P[%d] Priority: ", i);
        scanf("%d", &processes[i].priority);
    }

    // Sort processes based on priority (higher number indicates higher priority)
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(processes[i].priority < processes[j].priority) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    processes[0].waiting_time = 0;
    for(int i = 1; i < n; i++) {
        processes[i].waiting_time = 0;
        for(int j = 0; j < i; j++) {
            processes[i].waiting_time += processes[j].burst_time;
        }
    }

    printf("\n Process \tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time");

    for(int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        printf("\n P[%d] \t\t %d \t\t %d \t\t %d \t\t %d \t\t %d", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].priority, processes[i].waiting_time, processes[i].turnaround_time);
    }

    printf("\n\n Average Waiting Time: %f", (float)total_waiting_time / n); 
    printf("\n Average Turnaround Time: %f \n", (float)total_turnaround_time / n);

    // Print Gantt Chart
    printf("\nGantt Chart:\n");
    for(int i = 0; i < n; i++) {
        printf(" P[%d] ", processes[i].id);
        if (i < n-1) printf("->");
    }
    printf("\n");

    for(int i = 0; i < n; i++) {
        printf(" %d ", processes[i].waiting_time);
        if (i < n-1) printf("   ");
    }
    printf(" %d\n", processes[n-1].turnaround_time);

    return 0;
}