#include "lab7.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <math.h>

using namespace std;

void Scheduler::execute()
{
    for (int i = 0; i < schedulongPoliceis.size(); i++)
    {
        clearTables();
        if (schedulongPoliceis[i].first == 1)
        {
            FCFS();
            if (type == "trace")
                trace(1, -1);
            else
                stats(1, -1);
        }
        else if (schedulongPoliceis[i].first == 2)
        {
            RR(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(2, schedulongPoliceis[i].second);
            else
                stats(2, schedulongPoliceis[i].second);
        }
        else if (schedulongPoliceis[i].first == 3)
        {
            SPN();
            if (type == "trace")
                trace(3, -1);
            else
                stats(3, -1);
        }
        else if (schedulongPoliceis[i].first == 4)
        {
            SRT();
            if (type == "trace")
                trace(4, -1);
            else
                stats(4, -1);
        }
        else if (schedulongPoliceis[i].first == 5)
        {
            HRRN();
            if (type == "trace")
                trace(5, -1);
            else
                stats(5, -1);
        }
        else if (schedulongPoliceis[i].first == 6)
        {
            FB1();
            if (type == "trace")
                trace(6, -1);
            else
                stats(6, -1);
        }
        else if (schedulongPoliceis[i].first == 7)
        {
            FB2i();
            if (type == "trace")
                trace(7, -1);
            else
                stats(7, -1);
        }
        else if (schedulongPoliceis[i].first == 8)
        {
            AGE(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(8, -1);
        }
    }
}
void Scheduler::trace(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 2)
    {
        if (argument > 10)
            cout << "RR-" << argument << " ";
        else
            cout << "RR-" << argument << "  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 3)
    {
        cout << "SPN   ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 4)
    {
        cout << "SRT   ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 5)
    {
        cout << "HRRN  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 6)
    {
        cout << "FB-1  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 7)
    {
        cout << "FB-2i ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 8)
    {
        cout << "Aging ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
}
void Scheduler::stats(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 2)
    {
        cout << "RR-" << argument << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 3)
    {
        cout << "SPN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 4)
    {
        cout << "SRT" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 5)
    {
        cout << "HRRN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 6)
    {
        cout << "FB-1" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 7)
    {
        cout << "FB-2i" << endl;
        printStats();
        cout << '\n';
    }
}
void Scheduler::FCFS()
{
    // First, sort processes by arrival time
    vector<int> processOrder;
    for (int i = 0; i < numberOfProcesses; i++) {
        processOrder.push_back(i);
    }
    
    sort(processOrder.begin(), processOrder.end(), 
         [this](int a, int b) {
             return processes[a].arrivalTime < processes[b].arrivalTime;
         });
    
    int currentTime = 0;
    
    for (int idx = 0; idx < numberOfProcesses; idx++)
    {
        int pid = processOrder[idx];
        
        
        if (currentTime < processes[pid].arrivalTime) {
            currentTime = processes[pid].arrivalTime;
        }
        
        
        while (processes[pid].remainingTime > 0 && currentTime < maxSeconds)
        {
            
            *(processesPrintingArray + pid * maxSeconds + currentTime) = '*';
            
           
            for (int j = 0; j < numberOfProcesses; j++)
            {
                if (j != pid && 
                    processes[j].arrivalTime <= currentTime && 
                    processes[j].remainingTime > 0)
                {
                    *(processesPrintingArray + j * maxSeconds + currentTime) = '.';
                }
            }
            
            currentTime++;
            processes[pid].remainingTime--;
        }
        
        
        processes[pid].finishTime = currentTime;
        processes[pid].turnAroundTime = 
            processes[pid].finishTime - processes[pid].arrivalTime;
        processes[pid].NormTurnTime = 
            processes[pid].turnAroundTime / (float)processes[pid].serveTime;
    }
}


void Scheduler::RR(int quantum)
{
    int currentTime = 0, completed = 0;
    vector<bool> arrived(numberOfProcesses, false);

    while (completed < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++)
            if (!arrived[i] && processes[i].arrivalTime <= currentTime)
            {
                arrived[i] = true;
                readyQueue.push(processes[i]);
            }

        if (readyQueue.empty())
        {
            currentTime++;
            continue;
        }

        Process p = readyQueue.front();
        readyQueue.pop();
        int pid = p.id;

        int run = min(quantum, processes[pid].remainingTime);

        for (int t = 0; t < run && currentTime < maxSeconds; t++)
        {
            *(processesPrintingArray + pid * maxSeconds + currentTime) = '*';

            for (int j = 0; j < numberOfProcesses; j++)
                if (j != pid && arrived[j] && processes[j].remainingTime > 0)
                    *(processesPrintingArray + j * maxSeconds + currentTime) = '.';

            currentTime++;
            processes[pid].remainingTime--;

            for (int i = 0; i < numberOfProcesses; i++)
                if (!arrived[i] && processes[i].arrivalTime <= currentTime)
                {
                    arrived[i] = true;
                    readyQueue.push(processes[i]);
                }
        }

        if (processes[pid].remainingTime > 0)
            readyQueue.push(processes[pid]);
        else
        {
            completed++;
            processes[pid].finishTime = currentTime;
            processes[pid].turnAroundTime =
                currentTime - processes[pid].arrivalTime;
            processes[pid].NormTurnTime =
                processes[pid].turnAroundTime / processes[pid].serveTime;
        }
    }
}


void Scheduler::SPN()
{
    int currentTime = 0, completed = 0;
    vector<bool> finished(numberOfProcesses, false);

    while (completed < numberOfProcesses && currentTime < maxSeconds)
    {
        int selected = -1, shortest = 1e9;

        for (int i = 0; i < numberOfProcesses; i++)
            if (!finished[i] &&
                processes[i].arrivalTime <= currentTime &&
                processes[i].serveTime < shortest)
            {
                shortest = processes[i].serveTime;
                selected = i;
            }

        if (selected == -1)
        {
            currentTime++;
            continue;
        }

        while (processes[selected].remainingTime > 0 &&
               currentTime < maxSeconds)
        {
            *(processesPrintingArray + selected * maxSeconds + currentTime) = '*';

            for (int j = 0; j < numberOfProcesses; j++)
                if (j != selected &&
                    processes[j].arrivalTime <= currentTime &&
                    !finished[j])
                    *(processesPrintingArray + j * maxSeconds + currentTime) = '.';

            currentTime++;
            processes[selected].remainingTime--;
        }

        finished[selected] = true;
        completed++;
        processes[selected].finishTime = currentTime;
        processes[selected].turnAroundTime =
            currentTime - processes[selected].arrivalTime;
        processes[selected].NormTurnTime =
            processes[selected].turnAroundTime /
            processes[selected].serveTime;
    }
}


void Scheduler::SRT()
{
    int currentTime = 0, completed = 0;

    while (completed < numberOfProcesses && currentTime < maxSeconds)
    {
        int selected = -1, best = 1e9;

        for (int i = 0; i < numberOfProcesses; i++)
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0 &&
                processes[i].remainingTime < best)
            {
                best = processes[i].remainingTime;
                selected = i;
            }

        if (selected == -1)
        {
            currentTime++;
            continue;
        }

        *(processesPrintingArray + selected * maxSeconds + currentTime) = '*';

        for (int j = 0; j < numberOfProcesses; j++)
            if (j != selected &&
                processes[j].arrivalTime <= currentTime &&
                processes[j].remainingTime > 0)
                *(processesPrintingArray + j * maxSeconds + currentTime) = '.';

        processes[selected].remainingTime--;
        currentTime++;

        if (processes[selected].remainingTime == 0)
        {
            completed++;
            processes[selected].finishTime = currentTime;
            processes[selected].turnAroundTime =
                currentTime - processes[selected].arrivalTime;
            processes[selected].NormTurnTime =
                processes[selected].turnAroundTime /
                processes[selected].serveTime;
        }
    }
}


void Scheduler::HRRN()
{
    int currentTime = 0, completed = 0;
    vector<bool> finished(numberOfProcesses, false);

    while (completed < numberOfProcesses && currentTime < maxSeconds)
    {
        int selected = -1;
        float bestRatio = -1;

        for (int i = 0; i < numberOfProcesses; i++)
            if (!finished[i] && processes[i].arrivalTime <= currentTime)
            {
                float wait = currentTime - processes[i].arrivalTime;
                float ratio =
                    (wait + processes[i].serveTime) /
                    (float)processes[i].serveTime;

                if (ratio > bestRatio)
                {
                    bestRatio = ratio;
                    selected = i;
                }
            }

        if (selected == -1)
        {
            currentTime++;
            continue;
        }

        while (processes[selected].remainingTime > 0 &&
               currentTime < maxSeconds)
        {
            *(processesPrintingArray + selected * maxSeconds + currentTime) = '*';

            for (int j = 0; j < numberOfProcesses; j++)
                if (j != selected &&
                    processes[j].arrivalTime <= currentTime &&
                    !finished[j])
                    *(processesPrintingArray + j * maxSeconds + currentTime) = '.';

            currentTime++;
            processes[selected].remainingTime--;
        }

        finished[selected] = true;
        completed++;
        processes[selected].finishTime = currentTime;
        processes[selected].turnAroundTime =
            currentTime - processes[selected].arrivalTime;
        processes[selected].NormTurnTime =
            processes[selected].turnAroundTime /
            processes[selected].serveTime;
    }
}

void Scheduler::FB1()
{
    FBQueues.clear();
    for (int i = 0; i <= 20; i++)
        FBQueues.push_back(queue<Process>());

    int cnt = 0;

    
    for (int time = 0; time < maxSeconds; time++)
    {
        // we start by checking new arrivals
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                processes[i].FBLevel = 0;
                FBQueues[0].push(processes[i]);
            }
        }

      
        if (processorBusy)
        {
            if (cnt == 1) 
            {
                if (currentProcess.remainingTime > 0)
                {
                    processorBusy = false;

                    
                    bool flag = false;
                    for(int i = 0; i < FBQueues.size(); i++) {
                        if(!FBQueues[i].empty()) {
                            flag = true;
                            break;
                        }
                    }

                    // drop level if needed
                    if (flag) {
                        currentProcess.FBLevel++;
                    }
                    
                    FBQueues[currentProcess.FBLevel].push(currentProcess);
                    cnt = 0;
                }
            }
        }

        // choosing the upcoming process 
        if (!processorBusy)
        {
            for (int i = 0; i < FBQueues.size(); i++)
            {
                if (!FBQueues[i].empty())
                {
                    currentProcess = FBQueues[i].front();
                    FBQueues[i].pop();
                    processorBusy = true;
                    cnt = 0;
                    break;
                }
            }
        }

        
        if (processorBusy)
        {
            int index = currentProcess.id * maxSeconds + time;
            *(processesPrintingArray + index) = '*';
            
            currentProcess.remainingTime--;
            cnt++;

            // when the process is terminated
            if (currentProcess.remainingTime == 0)
            {
                processorBusy = false;
                processes[currentProcess.id].finishTime = time + 1;
                processes[currentProcess.id].turnAroundTime = (time + 1) - processes[currentProcess.id].arrivalTime;
                processes[currentProcess.id].NormTurnTime = processes[currentProcess.id].turnAroundTime / processes[currentProcess.id].serveTime;
            }
        }

        // to print the waiting plot
        for (int i = 0; i < FBQueues.size(); i++)
        {
            if (!FBQueues[i].empty())
            {
                queue<Process> q = FBQueues[i];
                while (!q.empty())
                {
                    Process p = q.front(); q.pop();
                    int index = p.id * maxSeconds + time;
                    *(processesPrintingArray + index) = '.';
                }
            }
        }
    }
}
void Scheduler::FB2i()
{
    FBQueues.clear();
    // Use enough queues (at least 4 for 2^0, 2^1, 2^2, 2^3)
    for (int i = 0; i < 10; i++)
        FBQueues.push_back(queue<Process>());

    int cnt = 0;
    processorBusy = false;

    for (int time = 0; time < maxSeconds; time++)
    {
        // 1. Check for new arrivals
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                processes[i].FBLevel = 0;
                FBQueues[0].push(processes[i]);
            }
        }

        // 2. Check Quantum (Preemption/Demotion)
        if (processorBusy)
        {
            // Dynamic quantum: 2^level
            int currentQuantum = pow(2, currentProcess.FBLevel);
            
            if (cnt == currentQuantum) 
            {
                if (currentProcess.remainingTime > 0)
                {
                    processorBusy = false;

                    // Check if others are waiting to decide if we drop level
                    bool higherPriorityWaiting = false;
                    for(int i = 0; i < FBQueues.size(); i++) {
                        if(!FBQueues[i].empty()) {
                            higherPriorityWaiting = true;
                            break;
                        }
                    }

                    if (higherPriorityWaiting) {
                        // Move to next level if someone else is waiting
                        if (currentProcess.FBLevel < 3) currentProcess.FBLevel++; 
                    }
                    
                    FBQueues[currentProcess.FBLevel].push(currentProcess);
                    cnt = 0;
                }
            }
        }

        // 3. Pick next process if idle
        if (!processorBusy)
        {
            for (int i = 0; i < FBQueues.size(); i++)
            {
                if (!FBQueues[i].empty())
                {
                    currentProcess = FBQueues[i].front();
                    FBQueues[i].pop();
                    processorBusy = true;
                    cnt = 0;
                    break;
                }
            }
        }

        // 4. Do the work
        if (processorBusy)
        {
            int index = currentProcess.id * maxSeconds + time;
            *(processesPrintingArray + index) = '*';
            
            currentProcess.remainingTime--;
            cnt++;

            if (currentProcess.remainingTime == 0)
            {
                processorBusy = false;
                processes[currentProcess.id].finishTime = time + 1;
                processes[currentProcess.id].turnAroundTime = (time + 1) - processes[currentProcess.id].arrivalTime;
                processes[currentProcess.id].NormTurnTime = (float)processes[currentProcess.id].turnAroundTime / processes[currentProcess.id].serveTime;
                cnt = 0;
            }
        }

        // 5. Print waiting dots
        for (int i = 0; i < FBQueues.size(); i++)
        {
            if (!FBQueues[i].empty())
            {
                queue<Process> q = FBQueues[i];
                while (!q.empty())
                {
                    Process p = q.front(); q.pop();
                    int index = p.id * maxSeconds + time;
                    *(processesPrintingArray + index) = '.';
                }
            }
        }
    }
}

void Scheduler::AGE(int quantum)
{
    vector<Process> list;
    int cnt = 0;
    // first we loop through time, check for arrivals , check for higher priorities and check for flags and quatnum
    // loop through time
    for (int time = 0; time < maxSeconds; time++)
    {
        // check for new arrivals
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == time)
            {
                processes[i].currentPriority = processes[i].priority;
                list.push_back(processes[i]);
            }
        }

        // check if higher priority exists
        if (processorBusy)
        {
            bool flag = false;
            for(int i=0; i<list.size(); i++) {
                if(list[i].currentPriority > currentProcess.currentPriority) {
                    flag = true; 
                    break;
                }
            }
            
            // preempt if needed
            if(flag) {
                list.push_back(currentProcess);
                processorBusy = false;
                cnt = 0;
            }
        }

        // check quantum
        if (processorBusy)
        {
            if (cnt == quantum)
            {
                currentProcess.currentPriority = currentProcess.priority;
                
                // increase priority for others
                for (int i = 0; i < list.size(); i++)
                    list[i].currentPriority++;

                list.push_back(currentProcess);
                processorBusy = false;
                cnt = 0;
            }
        }

        // pick highest priority
        if (!processorBusy && !list.empty())
        {
            int k = 0;
            for (int i = 1; i < list.size(); i++)
            {
                if (list[i].currentPriority > list[k].currentPriority)
                    k = i;
            }
            currentProcess = list[k];
            list.erase(list.begin() + k);
            processorBusy = true;
            cnt = 0;
        }
        // the crucial part:
        // do the work
        if (processorBusy)
        {
            int index = currentProcess.id * maxSeconds + time;
            *(processesPrintingArray + index) = '*';
            
            cnt++;
        }

        // print waiting dots
        for (int i = 0; i < list.size(); i++)
        {
            int index = list[i].id * maxSeconds + time;
            *(processesPrintingArray + index) = '.';
        }
    }
}
void Scheduler::printTracing()
{
    for (int process = 0; process < numberOfProcesses; process++)
    {
        cout << processes[process].processName << "     |";
        for (int time = 0; time < maxSeconds; time++)
            cout << *(processesPrintingArray + process * maxSeconds + time) << '|';
        cout << " \n";
    }
}
void Scheduler::printStats()
{
    float sum, mean, sum2;
    cout << "Process    |";
    for (int i = 0; i < numberOfProcesses; i++)
        cout << "  " << processes[i].processName << "  |";
    cout << endl;
    cout << "Arrival    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].arrivalTime << "  |";
        else
            cout << " " << processes[i].arrivalTime << "  |";
    }
    cout << endl;
    cout << "Service    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].serveTime << "  |";
        else
            cout << " " << processes[i].serveTime << "  |";
    }
    cout << " Mean|" << endl;
    cout << "Finish     |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].finishTime >= 10)
            cout << " " << processes[i].finishTime << "  |";
        else
            cout << "  " << processes[i].finishTime << "  |";
    }
    cout << "-----|" << endl;
    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcesses; i++)
    {

        if (processes[i].turnAroundTime >= 10)
            cout << " " << (int)processes[i].turnAroundTime << "  |";
        else
            cout << "  " << (int)processes[i].turnAroundTime << "  |";
        sum += processes[i].turnAroundTime;
    }
    cout << fixed;
    cout << setprecision(2);
    mean = (sum * 1.0) / numberOfProcesses;
    if (mean >= 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
    cout << "NormTurn   |";
    sum2 = 0;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].NormTurnTime > 10)
            cout << processes[i].NormTurnTime << "|";
        else
            cout << " " << processes[i].NormTurnTime << "|";
        sum2 += (processes[i].NormTurnTime * 1.0);
    }

    mean = (sum2 * 1.0) / numberOfProcesses;
    if (mean > 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
}
void Scheduler::clearTables()
{

    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < maxSeconds; j++)
            *(processesPrintingArray + i * maxSeconds + j) = ' ';
    }
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processes[i].finishTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].NormTurnTime = 0;
        processes[i].processState = ' ';
        processes[i].remainingTime = processes[i].serveTime;
    }
    processorBusy = false;
    while (!readyQueue.empty())
        readyQueue.pop();
    while (!readyPriorityQueue.empty())
        readyPriorityQueue.pop();
}
void Scheduler::splitPolicyAndParameter(string str)
{
    string w = "";
    pair<int, int> policies;
    bool parameterExists = false;
    policies.second = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            parameterExists = true;
            policies.first = stoi(w);
            w = "";
        }
        else if (str[i] == ',')
        {
            if (parameterExists)
                policies.second = stoi(w);
            else
            {
                policies.first = stoi(w);
                policies.second = -1;
            }
            w = "";
            schedulongPoliceis.push_back(policies);
            parameterExists = false;
        }
        else
            w = w + str[i];
    }
    if (parameterExists)
        policies.second = stoi(w);
    else
        policies.first = stoi(w);
    schedulongPoliceis.push_back(policies);
}
void Scheduler::splitPrcoessAndTimes(string str, int id)
{
    Process process;
    string w = "";
    process.processName = str[0];
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i] == ',')
        {
            process.arrivalTime = stoi(w);
            w = "";
        }
        else
            w = w + str[i];
    }
    processorBusy = false;
    process.processState = ' ';
    if (schedulongPoliceis[0].first == 8)
    {
        process.priority = stoi(w);
        process.currentPriority = stoi(w);
    }
    else
        process.serveTime = stoi(w);
    process.remainingTime = process.serveTime;
    process.waitingTime = 0;
    process.id = id;
    processes.push_back(process);
}
void Scheduler::readFile()
{
    processorBusy = false;
    string temp1, temp2;
    cin >> type;
    cin >> temp1;
    splitPolicyAndParameter(temp1);
    cin >> maxSeconds;
    cin >> numberOfProcesses;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cin >> temp2;
        splitPrcoessAndTimes(temp2, i);
    }
    processesPrintingArray = new char[numberOfProcesses * maxSeconds];
    currentProcess.processName = 0;
    currentProcess.q = 0;
    clearTables();
}
void Scheduler::printHeader()
{
    for (int i = 0; i < maxSeconds + 1; i++)
        cout << i % 10 << ' ';
    cout << "\n";
    printDashes();
}
void Scheduler::printDashes()
{
    cout << "------------------------------------------------";
}
int main(void)
{
    Scheduler scheduler;
    scheduler.readFile();
    scheduler.execute();
    return 0;
}