#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int turnaround;
} Process;

void print_gantt(int *pids, int *starts, int *ends, int size) {
    printf("\nGantt Chart:\n");
    for(int i=0; i<size; i++) printf("| P%d ", pids[i]);
    printf("|\n");
    for(int i=0; i<size; i++) printf("%d    ", starts[i]);
    printf("%d\n", ends[size-1]);
}

void sjf(Process *p, int n) {
    int time=0, completed=0, *gantt=NULL, *starts=NULL, *ends=NULL;
    int size=0, prev=-1, start=0;
    
    for(int i=0; i<n; i++) p[i].remaining = p[i].burst;
    
    while(completed < n) {
        int idx = -1, min = 1e9;
        for(int i=0; i<n; i++) {
            if(p[i].arrival <= time && p[i].remaining > 0 && 
               p[i].remaining < min) {
                min = p[i].remaining;
                idx = i;
            }
        }
        
        if(idx != -1) {
            if(prev != p[idx].id) {
                if(prev != -1) {
                    size++; ends = realloc(ends, size*sizeof(int)); ends[size-1] = time;
                }
                size++;
                gantt = realloc(gantt, size*sizeof(int)); gantt[size-1] = p[idx].id;
                starts = realloc(starts, size*sizeof(int)); starts[size-1] = time;
                prev = p[idx].id;
            }
            time++;
            p[idx].remaining--;
            
            if(p[idx].remaining == 0) {
                completed++;
                p[idx].turnaround = time - p[idx].arrival;
                p[idx].waiting = p[idx].turnaround - p[idx].burst;
            }
        } else {
            time++;
        }
    }
    
    print_gantt(gantt, starts, ends, size);
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
        printf("Process %d (arrival burst): ", i+1);
        scanf("%d %d", &p[i].arrival, &p[i].burst);
        p[i].id = i+1;
    }
    
    sjf(p, n);
    free(p);
    return 0;
}