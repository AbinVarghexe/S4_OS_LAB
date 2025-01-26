#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

int main() {
    int n, avwt = 0, avtat = 0;
    Process *processes;

    printf("Enter Total Number of processes : ");
    scanf("%d", &n);

    processes = (Process*)malloc(n * sizeof(Process));

    printf("\n Enter Process Arrival Time and Burst Time \n"); 
    for(int i = 0; i < n; i++) {
        processes[i].id = i;
        printf("P[%d] Arrival Time: ", i);
        scanf("%d", &processes[i].arrival_time);
        printf("P[%d] Burst Time: ", i);
        scanf("%d", &processes[i].burst_time);
    }

    processes[0].waiting_time = 0;
    for(int i = 1; i < n; i++) {
        processes[i].waiting_time = 0;
        for(int j = 0; j < i; j++) {
            processes[i].waiting_time += processes[j].burst_time;
        }
    }

    printf("\n Process \tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time");

    for(int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
        avwt += processes[i].waiting_time;
        avtat += processes[i].turnaround_time;
        printf("\n P[%d] \t\t %d \t\t %d \t\t %d \t\t %d", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    avwt /= n;
    avtat /= n;
    printf("\n\n Average Waiting Time: %d", avwt); 
    printf("\n Average Turnaround Time: %d \n", avtat);

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