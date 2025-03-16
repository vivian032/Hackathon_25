#include <iostream>
#include <iomanip>
using namespace std;
struct TaskNode
{
        int id;
        int arrival_time;
        int burst_time;
        int priority;
        int completion_time;
        int waiting_time;
        int turn_around_time;
        TaskNode* link;
};
TaskNode *start=NULL;
class Scheduler
{
        public:
        static int count;
        void createTask(int burst_time,int priority);
        void insertTask(int burst_time,int priority);
        void FIFO();
        void RR(int time_quantum);
        void display();
};
int Scheduler::count=0;
void Scheduler::createTask(int burst_time,int priority)
{
        TaskNode *N= new TaskNode();
        N->id=++count;
        N->arrival_time=N->id-1;
        N->burst_time=burst_time;
        N->priority=priority;
        N->completion_time=0;
        N->waiting_time=0;
        N->turn_around_time=0;
        N->link=NULL;
        start=N;
}
void Scheduler::insertTask(int burst_time, int priority)
{
        TaskNode* N = new TaskNode();
        N->id = ++count;
        N->arrival_time = N->id - 1;
        N->burst_time = burst_time;
        N->priority = priority;
        N->completion_time = 0;
        N->waiting_time = 0;
        N->turn_around_time = 0;
        N->link = NULL;
        if (start == NULL)
        {
                start = N;
                return;
        }
        TaskNode* ptr = start;
        while (ptr->link != NULL)
        {
                ptr = ptr->link;
        }
        ptr->link = N;
}
void Scheduler::FIFO()
{
        TaskNode* ptr=start;
        int current_time=0,total_tat=0,total_wt=0;
        while(ptr)
        {
                current_time+=ptr->burst_time;
                ptr->completion_time=current_time;
                ptr->turn_around_time=ptr->completion_time -ptr->arrival_time;
                ptr->waiting_time=ptr->turn_around_time -ptr->burst_time;
                total_tat+=ptr->turn_around_time;
                total_wt+=ptr->waiting_time;
                ptr=ptr->link;
        }
        display();
        cout << "Average Turn Around Time: " << fixed << setprecision(2) << (float)total_tat / count << endl;
        cout << "Average Waiting Time     : " << fixed << setprecision(2) << (float)total_wt / count << endl;

}
void Scheduler::RR(int time_quantum)
{
        int total_tat=0,total_wt=0;
        TaskNode *ptr=start;
        int *remainingBurst=new int[count];
        TaskNode* taskArray[count];
        int i=0;
        while(ptr)
        {

                remainingBurst[i]=ptr->burst_time;
                taskArray[i]=ptr;
                ptr=ptr->link;
                i++;

        }
        int current_time = 0;
        bool done;
        while (true)
        {
                done = true;
                for (int j = 0; j < count; j++)
                {
                        if (remainingBurst[j] > 0)
                        {
                                done = false;
                                if (remainingBurst[j] > time_quantum)
                                {
                                        current_time += time_quantum;
                                        remainingBurst[j] -= time_quantum;
                                }
                                else
                                {
                                        current_time += remainingBurst[j];
                                        taskArray[j]->completion_time = current_time;
                                        remainingBurst[j] = 0;
                                }
                        }
                }
                if (done) break;
        }

        for (int j = 0; j < count; j++)
        {
                taskArray[j]->turn_around_time = taskArray[j]->completion_time - taskArray[j]->arrival_time;
                taskArray[j]->waiting_time = taskArray[j]->turn_around_time - taskArray[j]->burst_time;
                total_tat += taskArray[j]->turn_around_time;
                total_wt += taskArray[j]->waiting_time;
        }
        display();
        cout << "Average Turn Around Time: " << fixed << setprecision(2) << (float)total_tat / count << endl;
        cout << "Average Waiting Time     : " << fixed << setprecision(2) << (float)total_wt / count << endl;
        delete[] remainingBurst;
}
void Scheduler::display()
        cout << "\n+----+-----+-----+-----+-----+-----+\n";
        cout << "| T  | AT  | BT  | CT  | TAT | WT  |\n";
        cout << "+----+-----+-----+-----+-----+-----+\n";
        TaskNode* ptr = start;
        while (ptr)
        {
            cout << "| " << setw(2) << ptr->id
                << " | " << setw(3) << ptr->arrival_time
                << " | " << setw(3) << ptr->burst_time
                << " | " << setw(3) << ptr->completion_time
                << " | " << setw(3) << ptr->turn_around_time
                << " | " << setw(3) << ptr->waiting_time << " |\n";
                ptr = ptr->link;
        }
        cout << "+----+-----+-----+-----+-----+-----+\n";
}
int main()
{
        int n, burst_time, priority;
        cout << "Enter Total Number Of Tasks: ";
        cin >> n;
        Scheduler scheduler;
        for (int i = 0; i < n; i++)
        {
                cout << "Enter Information About Task " << i + 1 << ":\n";
                cout << "Enter The Completion Time of the Task: ";
                cin >> burst_time;
                while (true)
                {
                        cout << "Enter The Priority of the task (1-10): ";
                        cin >> priority;
                        if (priority < 1 || priority > 10)
                        {
                                cout << "Invalid Input for Priority!\n";
                        }
                        else
                        {
                                break;
                        }
                }
                if (i == 0)
                         scheduler.createTask(burst_time, priority);
                else
                        scheduler.insertTask(burst_time, priority);
        }
        scheduler.FIFO();
        scheduler.RR(2);
        return 0;
}
