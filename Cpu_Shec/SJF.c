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

void sjf(Process processes[], int n)
{
    // Create a copy of processes to preserve the original burst times
    Process p[MAX];
    for (int i = 0; i < n; i++)
    {
        p[i] = processes[i];
        p[i].burst_time = p[i].temp_burst_time;  // Reset burst time to original
    }

    int time = 0;
    int completed = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    while (completed < n)
    {
        int shortest_job = -1;
        int min_burst_time = 9999;

        // Find the process with the smallest burst time among the arrived processes
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival_time <= time && p[i].burst_time > 0)
            {
                if (p[i].burst_time < min_burst_time)
                {
                    min_burst_time = p[i].burst_time;
                    shortest_job = i;
                }
            }
        }

        // If no process is found, increment time
        if (shortest_job == -1)
        {
            time++;
            continue;
        }

        // Execute the shortest job
        time += p[shortest_job].burst_time;
        p[shortest_job].completion_time = time;
        p[shortest_job].turnaround_time = p[shortest_job].completion_time - p[shortest_job].arrival_time;
        p[shortest_job].waiting_time = p[shortest_job].turnaround_time - p[shortest_job].temp_burst_time;
        p[shortest_job].burst_time = 0;  // Mark as completed

        // Update total times
        total_waiting_time += p[shortest_job].waiting_time;
        total_turnaround_time += p[shortest_job].turnaround_time;
        completed++;
    }

    // Copy back to original process array
    for (int i = 0; i < n; i++)
    {
        processes[i].waiting_time = p[i].waiting_time;
        processes[i].turnaround_time = p[i].turnaround_time;
        processes[i].completion_time = p[i].completion_time;
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
    
    sjf(Processes, n);
    display(Processes, n);

    return 0;
}
