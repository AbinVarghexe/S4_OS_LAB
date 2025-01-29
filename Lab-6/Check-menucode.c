#include <stdio.h>
#include <stdlib.h>

//===================================== Defining Structure ========================================

typedef struct
{
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
void print_gantt_chart(int *gantt_chart, int *start_time, int *end_time, int gantt_index);
void sort_by_arrival_time(Process *processes, int n);

// Function to print Gantt chart
void print_gantt_chart(int *gantt_chart, int *start_time, int *end_time, int gantt_index)
{
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_index; i++)
    {
        printf("| P%d ", gantt_chart[i]);
    }
    printf("|\n");
    for (int i = 0; i < gantt_index; i++)
    {
        printf("%d    ", start_time[i]);
    }
    printf("%d\n", end_time[gantt_index - 1]);
}

// Function to sort processes by arrival time
void sort_by_arrival_time(Process *processes, int n)
{
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
}

// Round Robin scheduling algorithm
void round_robin(Process *processes, int n, int time_quantum)
{
    // Sort by arrival time
    sort_by_arrival_time(processes, n);

    int *remaining_time = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        remaining_time[i] = processes[i].burst_time;

    int current_time = 0, completed = 0, i = 0;
    int *gantt_chart = NULL;
    int *start_time = NULL;
    int *end_time = NULL;
    int gantt_size = 0;

    while (completed < n)
    {
        if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time)
        {
            int slice = (remaining_time[i] < time_quantum) ? remaining_time[i] : time_quantum;
            remaining_time[i] -= slice;

            // Add to Gantt chart
            gantt_size++;
            gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
            start_time = realloc(start_time, gantt_size * sizeof(int));
            end_time = realloc(end_time, gantt_size * sizeof(int));
            gantt_chart[gantt_size - 1] = processes[i].id;
            start_time[gantt_size - 1] = current_time;
            end_time[gantt_size - 1] = current_time + slice;

            current_time += slice;

            if (remaining_time[i] == 0)
            {
                completed++;
                processes[i].waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
                if (processes[i].waiting_time < 0)
                    processes[i].waiting_time = 0;
                processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
            }
        }
        else
        {
            current_time++;
        }
        i = (i + 1) % n;
    }

    // Print Gantt chart
    print_gantt_chart(gantt_chart, start_time, end_time, gantt_size);

    // Print process table
    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    free(remaining_time);
    free(gantt_chart);
    free(start_time);
    free(end_time);
}

// FCFS scheduling algorithm
void fcfs(Process *processes, int n)
{
    int *gantt_chart = (int *)malloc(n * sizeof(int));
    int *start_time = (int *)malloc(n * sizeof(int));
    int *end_time = (int *)malloc(n * sizeof(int));
    int gantt_index = 0;
    int time = 0;

    for (int i = 0; i < n; i++)
    {
        start_time[gantt_index] = time;
        gantt_chart[gantt_index] = processes[i].id;
        time += processes[i].burst_time;
        end_time[gantt_index] = time;
        processes[i].waiting_time = start_time[gantt_index] - processes[i].arrival_time;
        if (processes[i].waiting_time < 0)
            processes[i].waiting_time = 0;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        gantt_index++;
    }

    // Print Gantt chart
    print_gantt_chart(gantt_chart, start_time, end_time, gantt_index);

    // Print table of processes
    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    free(gantt_chart);
    free(start_time);
    free(end_time);
}

// SJF scheduling algorithm
void sjf(Process *processes, int n)
{
    sort_by_arrival_time(processes, n);

    int time = 0, completed = 0, shortest = -1;
    int *remaining_time = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        remaining_time[i] = processes[i].burst_time;

    int *gantt_chart = NULL;
    int *start_time = NULL;
    int *end_time = NULL;
    int gantt_size = 0;
    int current_p = -1;
    int current_start = 0;

    while (completed < n)
    {
        shortest = -1;
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrival_time <= time && remaining_time[i] > 0)
            {
                if (shortest == -1 || remaining_time[i] < remaining_time[shortest])
                {
                    shortest = i;
                }
            }
        }

        if (shortest != -1)
        {
            int pid = processes[shortest].id;
            if (current_p != pid)
            {
                if (current_p != -1)
                {
                    gantt_size++;
                    gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
                    start_time = realloc(start_time, gantt_size * sizeof(int));
                    end_time = realloc(end_time, gantt_size * sizeof(int));
                    gantt_chart[gantt_size - 1] = current_p;
                    start_time[gantt_size - 1] = current_start;
                    end_time[gantt_size - 1] = time;
                }
                current_p = pid;
                current_start = time;
            }

            remaining_time[shortest]--;
            time++;

            if (remaining_time[shortest] == 0)
            {
                completed++;
                processes[shortest].waiting_time = time - processes[shortest].arrival_time - processes[shortest].burst_time;
                if (processes[shortest].waiting_time < 0)
                    processes[shortest].waiting_time = 0;
                processes[shortest].turnaround_time = processes[shortest].waiting_time + processes[shortest].burst_time;
            }
        }
        else
        {
            if (current_p != -1)
            {
                gantt_size++;
                gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
                start_time = realloc(start_time, gantt_size * sizeof(int));
                end_time = realloc(end_time, gantt_size * sizeof(int));
                gantt_chart[gantt_size - 1] = current_p;
                start_time[gantt_size - 1] = current_start;
                end_time[gantt_size - 1] = time;
                current_p = -1;
            }
            time++;
        }
    }

    if (current_p != -1)
    {
        gantt_size++;
        gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
        start_time = realloc(start_time, gantt_size * sizeof(int));
        end_time = realloc(end_time, gantt_size * sizeof(int));
        gantt_chart[gantt_size - 1] = current_p;
        start_time[gantt_size - 1] = current_start;
        end_time[gantt_size - 1] = time;
    }

    print_gantt_chart(gantt_chart, start_time, end_time, gantt_size);

    printf("\nProcess\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    free(remaining_time);
    free(gantt_chart);
    free(start_time);
    free(end_time);
}

// Priority Scheduling algorithm
void priority_scheduling(Process *processes, int n)
{
    sort_by_arrival_time(processes, n);

    int time = 0, completed = 0, highest_priority = -1;
    int *remaining_time = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        remaining_time[i] = processes[i].burst_time;

    int *gantt_chart = NULL;
    int *start_time = NULL;
    int *end_time = NULL;
    int gantt_size = 0;
    int current_p = -1;
    int current_start = 0;

    while (completed < n)
    {
        highest_priority = -1;
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrival_time <= time && remaining_time[i] > 0)
            {
                if (highest_priority == -1 || processes[i].priority < processes[highest_priority].priority)
                {
                    highest_priority = i;
                }
            }
        }

        if (highest_priority != -1)
        {
            int pid = processes[highest_priority].id;
            if (current_p != pid)
            {
                if (current_p != -1)
                {
                    gantt_size++;
                    gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
                    start_time = realloc(start_time, gantt_size * sizeof(int));
                    end_time = realloc(end_time, gantt_size * sizeof(int));
                    gantt_chart[gantt_size - 1] = current_p;
                    start_time[gantt_size - 1] = current_start;
                    end_time[gantt_size - 1] = time;
                }
                current_p = pid;
                current_start = time;
            }

            remaining_time[highest_priority]--;
            time++;

            if (remaining_time[highest_priority] == 0)
            {
                completed++;
                processes[highest_priority].waiting_time = time - processes[highest_priority].arrival_time - processes[highest_priority].burst_time;
                if (processes[highest_priority].waiting_time < 0)
                    processes[highest_priority].waiting_time = 0;
                processes[highest_priority].turnaround_time = processes[highest_priority].waiting_time + processes[highest_priority].burst_time;
            }
        }
        else
        {
            if (current_p != -1)
            {
                gantt_size++;
                gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
                start_time = realloc(start_time, gantt_size * sizeof(int));
                end_time = realloc(end_time, gantt_size * sizeof(int));
                gantt_chart[gantt_size - 1] = current_p;
                start_time[gantt_size - 1] = current_start;
                end_time[gantt_size - 1] = time;
                current_p = -1;
            }
            time++;
        }
    }

    if (current_p != -1)
    {
        gantt_size++;
        gantt_chart = realloc(gantt_chart, gantt_size * sizeof(int));
        start_time = realloc(start_time, gantt_size * sizeof(int));
        end_time = realloc(end_time, gantt_size * sizeof(int));
        gantt_chart[gantt_size - 1] = current_p;
        start_time[gantt_size - 1] = current_start;
        end_time[gantt_size - 1] = time;
    }

    print_gantt_chart(gantt_chart, start_time, end_time, gantt_size);

    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].priority, processes[i].waiting_time, processes[i].turnaround_time);
    }

    free(remaining_time);
    free(gantt_chart);
    free(start_time);
    free(end_time);
}

int main()
{
    int n, choice, time_quantum;
    Process *processes, *processes_copy;

    printf("Enter Total Number of processes : ");
    scanf("%d", &n);

    processes = (Process *)malloc(n * sizeof(Process));
    processes_copy = (Process *)malloc(n * sizeof(Process));

    for (int i = 0; i < n; i++)
    {
        printf("\nEnter arrival time for P%d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);

        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &processes[i].burst_time);

        processes[i].id = i + 1;
        processes[i].priority = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }

    sort_by_arrival_time(processes, n);

    for (int i = 0; i < n; i++)
    {
        processes_copy[i] = processes[i];
    }

    while (1)
    {
        printf("\n1. FCFS\n2. SJF\n3. Priority Scheduling\n4. Round Robin\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            fcfs(processes_copy, n);
            break;
        case 2:
            sjf(processes_copy, n);
            break;
        case 3:
            for (int i = 0; i < n; i++)
            {
                printf("Enter priority for P%d: ", processes_copy[i].id);
                scanf("%d", &processes_copy[i].priority);
            }
            priority_scheduling(processes_copy, n);
            break;
        case 4:
            printf("Enter Time Quantum: ");
            scanf("%d", &time_quantum);
            round_robin(processes_copy, n, time_quantum);
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice! Please enter again.\n");
        }

        for (int i = 0; i < n; i++)
        {
            processes_copy[i] = processes[i];
        }
    }

    free(processes);
    free(processes_copy);

    return 0;
}
