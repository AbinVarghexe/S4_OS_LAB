#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PROCESSES 100
// Structure to store process information
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
//Function to get the process informations--NB:PID is given by the program itself
void inputProcesses(Process processes[], int n) 
{
    for (int i = 0; i < n; i++) 
    {
        printf("\n");
        printf("Enter details for Process %d:\n", i + 1);
        processes[i].id = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        printf("Priority: ");
        scanf("%d", &processes[i].priority);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}
// Function to sort processes by arrival time
void sortByArrivalTime(Process processes[], int n) 
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
//Gantt chart display function
void displayGanttChart(int gantt[], int times[], int count) 
{
    printf("\n");
    printf("Gantt Chart:\n");
    for (int i = 0; i < count; i++) 
    {
        if (gantt[i] == -1) 
        {
            int idle_start = times[i];
            while (i + 1 < count && gantt[i + 1] == -1) i++;
            printf("| zZ ");
        } 
        else 
        {
            printf("| P%d ", gantt[i]);
        }
    }
    printf("|\n");
    for (int i = 0; i <= count; i++) 
    {
        if (gantt[i - 1] == -1 && gantt[i] == -1) continue;
       if(times[i]<10)
        {
        printf("%d", times[i]);
        printf("    ");
        }
        else
        {
        printf("%d", times[i]);
        printf("   ");
        }
    }
    printf("\n");
}
void displayResults(Process processes[], int n) 
{
    float total_waiting_time = 0, total_turnaround_time = 0;
    printf("\nProcess Details:\n");
    printf("ID\tAT\tBT\tPR\tWT\tTAT\n");

    for (int i = 0; i < n; i++) 
    {
printf("%d\t%d\t%d\t%d\t%d\t%d\n", processes[i].id, processes[i].arrival_time,
            processes[i].burst_time, processes[i].priority, processes[i].waiting_time, processes[i].turnaround_time);
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }
    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
}
//Priority Scheduling 
void priorityScheduling(Process processes[], int n) 
{
    sortByArrivalTime(processes, n); // Sort processes by arrival time
    int current_time = 0, completed = 0;
    int gantt[MAX_PROCESSES * 10], times[MAX_PROCESSES * 10];
    int gantt_index = 0;
    for (int i = 0; i < n; i++) 
    {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
    while (completed < n) 
    {
        int highest_priority = -1;
        for (int i = 0; i < n; i++) 
        {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) 
            {
                if (highest_priority == -1 || processes[i].priority < processes[highest_priority].priority) 
                {
                    highest_priority = i;
                }
            }
        }
        if (highest_priority != -1) 
        {
            gantt[gantt_index] = processes[highest_priority].id;
            times[gantt_index++] = current_time;
            current_time += processes[highest_priority].burst_time;
            processes[highest_priority].remaining_time = 0;
            processes[highest_priority].turnaround_time = current_time - processes[highest_priority].arrival_time;
            processes[highest_priority].waiting_time = processes[highest_priority].turnaround_time - processes[highest_priority].burst_time;
            completed++;
        } 
        else 
        {
            gantt[gantt_index] = -1; // Idle marker
            times[gantt_index++] = current_time;
            current_time++;
        }
    }
    times[gantt_index] = current_time;
    displayGanttChart(gantt, times, gantt_index);
}




int main() 
{
    Process processes[MAX_PROCESSES];
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    inputProcesses(processes, n);

    priorityScheduling(processes, n);
    displayResults(processes, n);

return 0;
}