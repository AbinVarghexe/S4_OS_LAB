#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20

typedef struct
{
    int id;
    int arrival_time;
    int temp_burst_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
} Process;

void input_processe(Process Processes[], int n)
{
    for (int i = 0; i < n; i++)
    {
        Processes[i].id = i + 1;
        printf("Enter the details of process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &Processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &Processes[i].burst_time);
        Processes[i].temp_burst_time = Processes[i].burst_time;
    }
}

void rr(Process processes[], int n)
{
    printf("Enter the time quantum: ");
    int time_quantum;
    scanf("%d", &time_quantum);
    int time = 0;
    int completed = 0;
    int i = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    while (1)
    {
        int done = 1;
        for (i = 0; i < n; i++)
        {
            if (processes[i].arrival_time <= time && processes[i].burst_time > 0)
            {
                done = 0;

                if (processes[i].burst_time > time_quantum)
                {
                    time += time_quantum;
                    processes[i].burst_time -= time_quantum;
                }
                else
                {
                    time += processes[i].burst_time;
                    processes[i].burst_time = 0;

                    processes[i].completion_time = time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].temp_burst_time;

                    total_waiting_time += processes[i].waiting_time;
                    total_turnaround_time += processes[i].turnaround_time;

                    completed++;
                }
            }
        }

        if (done)
            break;
    }

    processes[0].waiting_time = total_waiting_time;
    processes[0].turnaround_time = total_turnaround_time;
}

void display(Process processes[], int n)
{
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\tCompletion Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].id,
               processes[i].arrival_time,
               processes[i].temp_burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].completion_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)processes[0].waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)processes[0].turnaround_time / n);
}

int main()
{
    int n, choice;
    Process Processes[MAX];

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    input_processe(Processes, n);
    while (1)
    {
        printf("\nEnter the Choice:\n1. Round Robin\n2. Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            rr(Processes, n);
            display(Processes, n);
            break;
        case 2:
            printf("Exiting...\n");
            exit(0);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

    return 0;
}
