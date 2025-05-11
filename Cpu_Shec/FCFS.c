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

void fcfs(Process processes[], int n)
{
    // Sort processes based on arrival time
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].arrival_time > processes[j + 1].arrival_time)
            {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < n; i++)
    {
        // If arrival time is greater than current time, update time
        if (processes[i].arrival_time > time)
        {
            time = processes[i].arrival_time;
        }

        // Process execution
        time += processes[i].temp_burst_time;
        processes[i].completion_time = time;
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].temp_burst_time;

        // Update total waiting and turnaround times
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    // Store the total values for calculating averages
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
    int n;
    Process Processes[MAX];

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    input_processe(Processes, n);
    
    fcfs(Processes, n);
    display(Processes, n);

    return 0;
}
