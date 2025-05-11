#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival;
    int burst;
    int waiting;
    int turnaround;
} Process;

void print_gantt(int *pids, int *starts, int *ends, int n) {
    printf("\nGantt Chart:\n");
    for(int i=0; i<n; i++) printf("| P%d ", pids[i]);
    printf("|\n");
    for(int i=0; i<n; i++) printf("%d    ", starts[i]);
    printf("%d\n", ends[n-1]);
}

void fcfs(Process *p, int n) {
    int time = 0, *gantt = malloc(n*sizeof(int));
    int *starts = malloc(n*sizeof(int)), *ends = malloc(n*sizeof(int));
    
    for(int i=0; i<n; i++) {
        if(time < p[i].arrival) time = p[i].arrival;
        starts[i] = time;
        gantt[i] = p[i].id;
        time += p[i].burst;
        ends[i] = time;
        p[i].turnaround = ends[i] - p[i].arrival;
        p[i].waiting = starts[i] - p[i].arrival;
    }
    
    print_gantt(gantt, starts, ends, n);
    printf("\nProcess\tArrival\tBurst\tWait\tTAT\n");
    float avg_w=0, avg_tat=0;
    for(int i=0; i<n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].arrival, 
              p[i].burst, p[i].waiting, p[i].turnaround);
        avg_w += p[i].waiting;
        avg_tat += p[i].turnaround;
    }
    printf("\nAverages: Wait=%.2f TAT=%.2f\n", avg_w/n, avg_tat/n);
    
    free(gantt); free(starts); free(ends);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process *p = malloc(n*sizeof(Process));
    
    for(int i=0; i<n; i++) {
        printf("Process %d (Arrival Time : ): ", i+1);
        scanf("%d", &p[i].arrival);
        printf("Process %d (Burst Time : ", i+1);
        scanf("%d", &p[i].burst);
        p[i].id = i+1;
    }
    
    // Sort by arrival time
    for(int i=0; i<n; i++)
        for(int j=0; j<n-i-1; j++)
            if(p[j].arrival > p[j+1].arrival) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
    
    fcfs(p, n);
    free(p);
    return 0;
}