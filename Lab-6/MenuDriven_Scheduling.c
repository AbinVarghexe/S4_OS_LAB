#include <stdio.h>
#include <stdlib.h>


//===================================== Defining Structure ========================================

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;

// Function prototypes
void fcfs(Process *processes, int n);
void sjf(Process *processes, int n);
void priority_scheduling(Process *processes, int n);
void round_robin(Process *processes, int n, int time_quantum);

int main() {
    int n, choice, time_quantum;
    Process *processes;

    while (1) {
//===================================== Menu ========================================

        printf("\nChoose Scheduling Algorithm:\n");
        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. Priority\n");
        printf("4. Round Robin\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 5) {
            printf("Exiting program.\n");
            break;
        }

        // Algorithm selection section
        switch(choice) {
            case 1:
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
                fcfs(processes, n);
                free(processes);
                break;
            case 2:
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
                sjf(processes, n);
                free(processes);
                break;
            case 3:
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
                priority_scheduling(processes, n);
                free(processes);
                break;
            case 4:
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
                    processes[i].remaining_time = processes[i].burst_time;
                }
                printf("Enter Time Quantum: ");
                scanf("%d", &time_quantum);
                round_robin(processes, n, time_quantum);
                free(processes);
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }

    return 0;
}

//============================================ FCFS Scheduling ============================================

void fcfs(Process *processes, int n) {
    int total_waiting_time = 0, total_turnaround_time = 0;

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
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        printf("\n P[%d] \t\t %d \t\t %d \t\t %d \t\t %d", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
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
}

//============================================ SJF Scheduling =====================================

void sjf(Process *processes, int n) {
    int total_waiting_time = 0, total_turnaround_time = 0;

    // Sort processes based on burst time
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(processes[i].burst_time > processes[j].burst_time) {
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

    printf("\n Process \tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time");

    for(int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        printf("\n P[%d] \t\t %d \t\t %d \t\t %d \t\t %d", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
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
}

//========================================= Priority Scheduling =======================================

void priority_scheduling(Process *processes, int n) {
    int total_waiting_time = 0, total_turnaround_time = 0;

    // Sort processes based on priority
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
}

//========================================== Round Robin Scheduling ==================================

void round_robin(Process *processes, int n, int time_quantum) {
    int time = 0, remain = n, flag = 0, wait_time = 0, turnaround_time = 0;

    printf("\n\nProcess\t|Arrival Time|Turnaround Time|Waiting Time\n\n");

    for(int i = 0; remain != 0;) {
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
}

//===================================== End of Program ========================================