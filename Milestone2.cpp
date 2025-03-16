#include <iostream>
#include <iomanip> // For formatted output
using namespace std;

#define MAX_PRIORITY 10  // Number of priority levels
#define MAX_TASKS 100   // Maximum number of tasks

struct Task {
    int id;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turn_around_time;
    Task *next;
};

struct Queue {
    Task *front, *rear;
    Queue() { front = rear = nullptr; }
    void enqueue(Task* task);
    Task* dequeue();
    bool isEmpty() { return front == nullptr; }
};

void Queue::enqueue(Task* newTask) {
    newTask->next = nullptr;
    if (rear == nullptr) {
        front = rear = newTask;
    } else {
        rear->next = newTask;
        rear = newTask;
    }
}

Task* Queue::dequeue() {
    if (isEmpty()) return nullptr;
    Task* temp = front;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    return temp;
}

class MLFQScheduler {
public:
    Queue queues[MAX_PRIORITY];
    int time_quantum[MAX_PRIORITY] = {4,8,12,16,20,24,28};
    Task* completedTasks[MAX_TASKS];  // Store completed tasks for final output
    int completedCount = 0;

    void addTask(int id, int burst_time, int priority);
    void execute();
    void showResults();
};

void MLFQScheduler::addTask(int id, int burst_time, int priority) {
    if (priority < 0 || priority >= MAX_PRIORITY) {
        cout << "Invalid priority level. Task not added.\n";
        return;
    }
    Task* newTask = new Task{id, burst_time, priority, burst_time, 0, 0, 0, nullptr};
    queues[priority].enqueue(newTask);
}

void MLFQScheduler::execute() {
    int current_time = 0;

    for (int i = 0; i < MAX_PRIORITY; i++) {
        if(!queues[i].isEmpty())
            cout << "\nExecuting tasks from Priority Queue " << i << endl;
        while (!queues[i].isEmpty()) {
            Task* task = queues[i].dequeue();

            if (task->remaining_time > time_quantum[i]) {
                cout << "Task " << task->id << " executed for " << time_quantum[i] << " units. "<<endl;
                current_time += time_quantum[i];
                task->remaining_time -= time_quantum[i];

                int new_priority = min(i + 1, MAX_PRIORITY - 1);
                cout << "Remaining time: " << task->remaining_time << " units. Moving to Priority Queue " << new_priority << ".\n";
                queues[new_priority].enqueue(task);
            } else {
                current_time += task->remaining_time;
                task->completion_time = current_time;
                task->turn_around_time = task->completion_time - (task->id - 1);
                task->waiting_time = task->turn_around_time - task->burst_time;

                cout << "Task " << task->id << " executed for " << task->remaining_time << " units. "<<endl;
                cout << "Task " << task->id << " completed execution.\n";

                completedTasks[completedCount++] = task;
            }
        }
    }
    showResults();
}

void MLFQScheduler::showResults() {
    cout << "\n+----+-----+-----+-----+-----+-----+\n";
    cout << "| T  | AT  | BT  | CT  | TAT | WT  |\n";
    cout << "+----+-----+-----+-----+-----+-----+\n";
    for (int i = 0; i < completedCount; i++) {
        Task* task = completedTasks[i];
        cout << "| " << setw(2) << task->id
             << " | " << setw(3) << (task->id - 1)
             << " | " << setw(3) << task->burst_time
             << " | " << setw(3) << task->completion_time
             << " | " << setw(3) << task->turn_around_time
             << " | " << setw(3) << task->waiting_time << " |\n";
        delete task; // Free memory after displaying
    }
    cout << "+----+-----+-----+-----+-----+-----+\n";
}

int main() {
    MLFQScheduler scheduler;
    int n, burst_time, priority;
    cout << "Enter number of tasks: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        do {
            cout << "Enter burst time and priority (0-" << MAX_PRIORITY - 1 << "): ";
            cin >> burst_time >> priority;
            if (priority < 0 || priority >= MAX_PRIORITY) {
                cout << "Invalid priority! Please enter a value between 0 and " << MAX_PRIORITY - 1 << ".\n";
            }
        } while (priority < 0 || priority >= MAX_PRIORITY);
        scheduler.addTask(i + 1, burst_time, priority);
    }
    scheduler.execute();
    return 0;
}
