#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int waiting;
    int turnaround;
} Process;

typedef struct {
    int* items;
    int size;
    int capacity;
} DynamicArray;

// ========================== Core Functions ==========================
void init_array(DynamicArray* da, int capacity) {
    da->items = malloc(capacity * sizeof(int));
    da->size = 0;
    da->capacity = capacity;
}

void push_array(DynamicArray* da, int value) {
    if (da->size >= da->capacity) {
        da->capacity *= 2;
        da->items = realloc(da->items, da->capacity * sizeof(int));
    }
    da->items[da->size++] = value;
}

void free_array(DynamicArray* da) {
    free(da->items);
    da->size = da->capacity = 0;
}

void print_gantt(DynamicArray* pids, DynamicArray* starts, DynamicArray* ends) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < pids->size; i++) {
        printf("| P%-2d ", pids->items[i]);
    }
    printf("|\n");
    
    for (int i = 0; i < starts->size; i++) {
        printf("%-5d", starts->items[i]);
    }
    printf("%d\n", ends->items[ends->size-1]);
}

void reset_processes(Process* dest, Process* src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
        dest[i].remaining = src[i].burst;
        dest[i].waiting = 0;
        dest[i].turnaround = 0;
    }
}

// ======================= Scheduling Algorithms =======================
void fcfs(Process* procs, int n) {
    DynamicArray pids, starts, ends;
    init_array(&pids, n);
    init_array(&starts, n);
    init_array(&ends, n);

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < procs[i].arrival) time = procs[i].arrival;
        
        push_array(&pids, procs[i].id);
        push_array(&starts, time);
        
        time += procs[i].burst;
        push_array(&ends, time);
        
        procs[i].turnaround = time - procs[i].arrival;
        procs[i].waiting = procs[i].turnaround - procs[i].burst;
    }

    print_gantt(&pids, &starts, &ends);
    free_array(&pids); free_array(&starts); free_array(&ends);
}

void round_robin(Process* procs, int n, int quantum) {
    DynamicArray pids, starts, ends;
    init_array(&pids, n*2);
    init_array(&starts, n*2);
    init_array(&ends, n*2);

    int* remaining = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) remaining[i] = procs[i].burst;

    int time = 0, completed = 0, ptr = 0;
    int prev_pid = -1, start_time = 0;

    while (completed < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            int idx = (ptr + i) % n;
            if (procs[idx].arrival > time || remaining[idx] == 0) continue;

            found = 1;
            ptr = (idx + 1) % n;
            int slice = (remaining[idx] > quantum) ? quantum : remaining[idx];
            
            if (prev_pid != procs[idx].id) {
                if (prev_pid != -1) push_array(&ends, time);
                push_array(&pids, procs[idx].id);
                push_array(&starts, time);
                prev_pid = procs[idx].id;
                start_time = time;
            }

            time += slice;
            remaining[idx] -= slice;

            if (remaining[idx] == 0) {
                completed++;
                procs[idx].turnaround = time - procs[idx].arrival;
                procs[idx].waiting = procs[idx].turnaround - procs[idx].burst;
            }
            break;
        }

        if (!found) {
            if (prev_pid != -1) {
                push_array(&ends, time);
                prev_pid = -1;
            }
            time++;
        }
    }

    if (prev_pid != -1) push_array(&ends, time);
    print_gantt(&pids, &starts, &ends);
    
    free(remaining);
    free_array(&pids); free_array(&starts); free_array(&ends);
}

void preemptive_scheduler(Process* procs, int n, int use_priority) {
    DynamicArray pids, starts, ends;
    init_array(&pids, n*2);
    init_array(&starts, n*2);
    init_array(&ends, n*2);

    int time = 0, completed = 0;
    int prev_pid = -1, start_time = 0;

    while (completed < n) {
        int idx = -1, min_val = 1e9;
        
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival > time || procs[i].remaining == 0) continue;
            
            int criteria = use_priority ? procs[i].priority : procs[i].remaining;
            if (criteria < min_val) {
                min_val = criteria;
                idx = i;
            }
        }

        if (idx != -1) {
            if (prev_pid != procs[idx].id) {
                if (prev_pid != -1) push_array(&ends, time);
                push_array(&pids, procs[idx].id);
                push_array(&starts, time);
                prev_pid = procs[idx].id;
                start_time = time;
            }

            time++;
            procs[idx].remaining--;

            if (procs[idx].remaining == 0) {
                completed++;
                procs[idx].turnaround = time - procs[idx].arrival;
                procs[idx].waiting = procs[idx].turnaround - procs[idx].burst;
                push_array(&ends, time);
                prev_pid = -1;
            }
        } else {
            if (prev_pid != -1) {
                push_array(&ends, time);
                prev_pid = -1;
            }
            time++;
        }
    }

    if (prev_pid != -1) push_array(&ends, time);
    print_gantt(&pids, &starts, &ends);
    free_array(&pids); free_array(&starts); free_array(&ends);
}

// ========================== Main Program ==========================
int compare_arrival(const void* a, const void* b) {
    return ((Process*)a)->arrival - ((Process*)b)->arrival;
}

void print_results(Process* procs, int n, int show_priority) {
    float avg_wait = 0, avg_tat = 0;
    
    printf("\nProcess\tArrival\tBurst\t");
    if (show_priority) printf("Priority\t");
    printf("Waiting\tTurnaround\n");
    
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t", procs[i].id, procs[i].arrival, procs[i].burst);
        if (show_priority) printf("%d\t", procs[i].priority);
        printf("%d\t%d\n", procs[i].waiting, procs[i].turnaround);
        
        avg_wait += procs[i].waiting;
        avg_tat += procs[i].turnaround;
    }
    
    printf("\nAverages: Waiting=%.2f Turnaround=%.2f\n", avg_wait/n, avg_tat/n);
}

int main() {
    int n, choice, quantum;
    Process *procs, *working_copy;

    printf("Enter total processes: ");
    scanf("%d", &n);

    procs = malloc(n * sizeof(Process));
    working_copy = malloc(n * sizeof(Process));

    for (int i = 0; i < n; i++) {
        printf("\nProcess P%d:\n", i+1);
        printf("Arrival time: ");
        scanf("%d", &procs[i].arrival);
        printf("Burst time: ");
        scanf("%d", &procs[i].burst);
        procs[i].id = i+1;
        procs[i].priority = 0;
        procs[i].remaining = procs[i].burst;
    }

    qsort(procs, n, sizeof(Process), compare_arrival);

    while (1) {
        printf("\n1. FCFS\n2. SJF\n3. Priority\n4. Round Robin\n5. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 5) break;

        reset_processes(working_copy, procs, n);

        switch (choice) {
            case 1:
                fcfs(working_copy, n);
                print_results(working_copy, n, 0);
                break;
                
            case 2:
                preemptive_scheduler(working_copy, n, 0);
                print_results(working_copy, n, 0);
                break;
                
            case 3:
                for (int i = 0; i < n; i++) {
                    printf("Priority for P%d: ", procs[i].id);
                    scanf("%d", &procs[i].priority);
                }
                reset_processes(working_copy, procs, n);
                preemptive_scheduler(working_copy, n, 1);
                print_results(working_copy, n, 1);
                break;
                
            case 4:
                printf("Time quantum: ");
                scanf("%d", &quantum);
                round_robin(working_copy, n, quantum);
                print_results(working_copy, n, 0);
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    }

    free(procs);
    free(working_copy);
    return 0;
}