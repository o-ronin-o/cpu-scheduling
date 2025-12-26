// lab7.h - Maintain original structure
#include <string>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

class Process
{
public:
    char processName;
    char processState;
    int arrivalTime;
    int serveTime;
    int remainingTime;
    float turnAroundTime;
    int finishTime;
    int waitingTime;
    float responseRatio;
    int FBLevel;
    float NormTurnTime;
    int q;
    int priority;
    int currentPriority;
    int id;
    
    // For HRRN priority queue
    float getResponseRatio(int currentTime) const {
        return (currentTime - arrivalTime + serveTime) / (float)serveTime;
    }
};

class Scheduler
{
public:
    string type;
    int numberOfProcesses;
    int maxSeconds;
    bool processorBusy;
    Process currentProcess;
    vector<Process> processes;
    vector<pair<int, int>> schedulongPoliceis;
    char *processesPrintingArray;
    queue<Process> readyQueue;
    
    // Keep the original priority queue type
    // We'll use it with custom comparison functions for different algorithms
    priority_queue<pair<float, int>> readyPriorityQueue;
    
    vector<queue<Process>> FBQueues;
    
    void printTracing();
    void printStats();
    void readFile();
    void clearTables();
    void splitPrcoessAndTimes(string str, int id);
    void splitPolicyAndParameter(string str);
    void printHeader();
    void printDashes();
    void printDetails();
    void execute();
    void trace(int policy, int argument);
    void stats(int policy, int argument);
    
    // Scheduling algorithms
    void FCFS();
    void RR(int quantum);
    void SPN();
    void SRT();
    void HRRN();
    void FB1();
    void FB2i();
    void AGE(int quantum);
    
    // Helper functions (same as before but adapted)
    void addToReadyQueueIfArrived(int currentTime);
    void updateWaitingTimeForReadyProcesses(int currentTime);
    void calculateStats();
    void markProcessReady(int processIndex, int currentTime);
    void markProcessRunning(int processIndex, int currentTime);
    void markProcessFinished(int processIndex, int currentTime);
    
    // Helper to find process by name
    int findProcessIndex(char processName);
};