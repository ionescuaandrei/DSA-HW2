#include "headers/adjacencymatrix.cpp.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <algorithm>
#include <climits>
#include <map>
#include <math.h>
using namespace std;

ifstream f("input.txt");

struct TaskInfo {
    int taskID;
    float workload;
    float deadline;
};

struct CoreInfo {
    float speed;
    float powerRate;
};

struct Assignment {
    int taskID;
    int coreID;
    float startTime;
    float endTime;
    float energy;
};

struct ScheduleState {
    vector<Assignment> assignments;
    vector<float> coreAvailableTime;
    vector<bool> taskScheduled;
    float totalEnergy;
    float currentTime;
    
    bool operator>(const ScheduleState& other) const {
        return totalEnergy > other.totalEnergy;
    }
};

bool mutualDependency(Graph<TaskInfo, int>& taskDependency, int node_1, int node_2) {
    return (taskDependency.checkEdge(node_1, node_2) && taskDependency.checkEdge(node_2, node_1));
}

bool isCyclic(int N, Graph<TaskInfo, int>& graph, int maxNodes) {
    char* visited = new char[maxNodes];
    char* recStack = new char[maxNodes];

    for (int i = 0; i < maxNodes; i++) {
        visited[i] = 0;
        recStack[i] = 0;
    }

    function<bool(int, int)> dfsUtil = [&](int node, int depth) -> bool {
        visited[node] = 1;
        recStack[node] = 1;

        for (int i = 0; i <= N; i++) {
            if (graph.checkEdge(node, i)) {
                if (mutualDependency(graph, node, i)) {
                    continue;
                }
                
                if (!visited[i]) {
                    if (dfsUtil(i, depth + 1)) {
                        return true;
                    }
                }
                else if (recStack[i] && depth >= 2) {
                    return true;
                }
            }
        }

        recStack[node] = 0;
        return false;
    };

    bool hasCycle = false;
    for (int i = 0; i <= N; i++) {
        if (!visited[i]) {
            if (dfsUtil(i, 0)) {
                hasCycle = true;
                break;
            }
        }
    }

    delete[] visited;
    delete[] recStack;
    return hasCycle;
}

vector<pair<int, int>> findMutualDependencies(int N, Graph<TaskInfo, int>& graph) {
    vector<pair<int, int>> mutualDeps;
    for (int i = 0; i <= N; i++) {
        for (int j = i + 1; j <= N; j++) {
            if (mutualDependency(graph, i, j)) {
                mutualDeps.push_back({i, j});
            }
        }
    }
    return mutualDeps;
}

bool canScheduleTask(int taskID, int coreID, float startTime, 
                    const TaskInfo& task, const CoreInfo& core) {
    float executionTime = task.workload / core.speed;
    float endTime = startTime + executionTime;
    return endTime <= task.deadline;
}

float calculateEnergy(const TaskInfo& task, const CoreInfo& core) {
    return task.workload / core.speed * core.powerRate;
}

bool dependenciesSatisfied(int taskID, const ScheduleState& state, 
                          Graph<TaskInfo, int>& graph, int N, int maxNodes) {
    for (int i = 0; i <= N; i++) {
        if (graph.checkEdge(i, taskID) && !mutualDependency(graph, i, taskID)) {
            if (!state.taskScheduled[i]) {
                return false;
            }
        }
    }
    return true;
}

ScheduleState scheduleWithEnergyMinimization(int N, Graph<TaskInfo, int>& graph,
                                           vector<CoreInfo>& cores, int maxNodes) {
    vector<pair<int, int>> mutualDeps = findMutualDependencies(N, graph);

    cout << "\nMutual dependencies found: " << mutualDeps.size() << endl;
    for (auto& dep : mutualDeps) {
        cout << "Tasks " << dep.first << " and " << dep.second
             << " (must run in parallel)" << endl;
    }

    ScheduleState result;
    result.coreAvailableTime.resize(cores.size(), 0.0f);
    result.taskScheduled.resize(maxNodes, false);
    result.totalEnergy = 0.0f;
    result.currentTime = 0.0f;

    for (auto& dep : mutualDeps) {
        int task1 = dep.first;
        int task2 = dep.second;
        
        if (result.taskScheduled[task1] || result.taskScheduled[task2]) continue;
        
        TaskInfo t1 = graph.getNodeInfo(task1);
        TaskInfo t2 = graph.getNodeInfo(task2);
        
        int bestCore1 = -1, bestCore2 = -1;
        float minCombinedEnergy = INFINITY;
        
        for (int c1 = 0; c1 < cores.size(); c1++) {
            for (int c2 = 0; c2 < cores.size(); c2++) {
                if (c1 == c2) continue;
                
                float startTime = max(result.coreAvailableTime[c1], result.coreAvailableTime[c2]);
                
                if (canScheduleTask(task1, c1, startTime, t1, cores[c1]) &&
                    canScheduleTask(task2, c2, startTime, t2, cores[c2])) {
                    
                    float energy1 = calculateEnergy(t1, cores[c1]);
                    float energy2 = calculateEnergy(t2, cores[c2]);
                    float combinedEnergy = energy1 + energy2;
                    
                    if (combinedEnergy < minCombinedEnergy) {
                        minCombinedEnergy = combinedEnergy;
                        bestCore1 = c1;
                        bestCore2 = c2;
                    }
                }
            }
        }
        
        if (bestCore1 != -1 && bestCore2 != -1) {
            float startTime = max(result.coreAvailableTime[bestCore1], result.coreAvailableTime[bestCore2]);
            
            float exec1 = t1.workload / cores[bestCore1].speed;
            float energy1 = calculateEnergy(t1, cores[bestCore1]);
            Assignment a1;
            a1.taskID = task1;
            a1.coreID = bestCore1;
            a1.startTime = startTime;
            a1.endTime = startTime + exec1;
            a1.energy = energy1;
            result.assignments.push_back(a1);
            
            float exec2 = t2.workload / cores[bestCore2].speed;
            float energy2 = calculateEnergy(t2, cores[bestCore2]);
            Assignment a2;
            a2.taskID = task2;
            a2.coreID = bestCore2;
            a2.startTime = startTime;
            a2.endTime = startTime + exec2;
            a2.energy = energy2;
            result.assignments.push_back(a2);
            
            result.coreAvailableTime[bestCore1] = startTime + exec1;
            result.coreAvailableTime[bestCore2] = startTime + exec2;
            result.taskScheduled[task1] = true;
            result.taskScheduled[task2] = true;
            result.totalEnergy += energy1 + energy2;
            result.currentTime = max(result.currentTime, max(startTime + exec1, startTime + exec2));
        }
    }
    
    for (int taskID = 1; taskID <= N; taskID++) {
        if (result.taskScheduled[taskID]) continue;
        
        if (!dependenciesSatisfied(taskID, result, graph, N, maxNodes)) continue;
        
        TaskInfo task = graph.getNodeInfo(taskID);
        
        int bestCore = -1;
        float minEnergy = INFINITY;
        
        for (int coreID = 0; coreID < cores.size(); coreID++) {
            float startTime = result.coreAvailableTime[coreID];
            if (canScheduleTask(taskID, coreID, startTime, task, cores[coreID])) {
                float energy = calculateEnergy(task, cores[coreID]);
                if (energy < minEnergy) {
                    minEnergy = energy;
                    bestCore = coreID;
                }
            }
        }
        
        if (bestCore != -1) {
            float startTime = result.coreAvailableTime[bestCore];
            float executionTime = task.workload / cores[bestCore].speed;
            float endTime = startTime + executionTime;
            
            Assignment assignment;
            assignment.taskID = taskID;
            assignment.coreID = bestCore;
            assignment.startTime = startTime;
            assignment.endTime = endTime;
            assignment.energy = minEnergy;
            
            result.assignments.push_back(assignment);
            result.coreAvailableTime[bestCore] = endTime;
            result.taskScheduled[taskID] = true;
            result.totalEnergy += minEnergy;
            result.currentTime = max(result.currentTime, endTime);
        }
    }
    
    bool progress = true;
    while (progress) {
        progress = false;
        
        for (int taskID = 0; taskID <= N; taskID++) {
            if (result.taskScheduled[taskID]) continue;
            
            if (!dependenciesSatisfied(taskID, result, graph, N, maxNodes)) continue;
            
            TaskInfo task = graph.getNodeInfo(taskID);
            
            int bestCore = -1;
            float minEnergy = INFINITY;
            
            for (int coreID = 0; coreID < cores.size(); coreID++) {
                float startTime = result.coreAvailableTime[coreID];
                if (canScheduleTask(taskID, coreID, startTime, task, cores[coreID])) {
                    float energy = calculateEnergy(task, cores[coreID]);
                    if (energy < minEnergy) {
                        minEnergy = energy;
                        bestCore = coreID;
                    }
                }
            }
            
            if (bestCore != -1) {
                float startTime = result.coreAvailableTime[bestCore];
                float executionTime = task.workload / cores[bestCore].speed;
                float endTime = startTime + executionTime;
                
                Assignment assignment;
                assignment.taskID = taskID;
                assignment.coreID = bestCore;
                assignment.startTime = startTime;
                assignment.endTime = endTime;
                assignment.energy = minEnergy;
                
                result.assignments.push_back(assignment);
                result.coreAvailableTime[bestCore] = endTime;
                result.taskScheduled[taskID] = true;
                result.totalEnergy += minEnergy;
                result.currentTime = max(result.currentTime, endTime);
                progress = true;
            }
        }
    }
    
    return result;
}

void printSchedule(const ScheduleState& schedule, vector<CoreInfo>& cores) {
    if (schedule.assignments.empty()) {
        cout << "No valid schedule found!" << endl;
        return;
    }

    cout << "\n=== OPTIMAL SCHEDULE ===" << endl;
    cout << "Total Energy Consumption: " << schedule.totalEnergy << " units" << endl;
    cout << "Total Completion Time: " << schedule.currentTime << " units" << endl;

    vector<Assignment> sortedAssignments = schedule.assignments;
    sort(sortedAssignments.begin(), sortedAssignments.end(),
         [](const Assignment& a, const Assignment& b) {
             return a.startTime < b.startTime;
         });

    cout << "\nTask Schedule:" << endl;
    cout << "Task ID | Core ID | Start Time | End Time | Energy |" << endl;
    cout << "--------|---------|------------|----------|--------|" << endl;

    for (const auto& assignment : sortedAssignments) {
        cout << "   " << assignment.taskID
             << "    |    " << assignment.coreID
             << "    |    " << assignment.startTime
             << "     |   " << assignment.endTime
             << "    |  " << assignment.energy << endl;
    }
}

void parseInput(int N_OF_TASKS, int& N_OF_DEPENDENCIES, int& N_OF_CORES,
               Graph<TaskInfo, int>& taskDependency, vector<CoreInfo>& cores) {
    TaskInfo temporaryTaskInfo{};
    for (int i = 0; i < N_OF_TASKS; i++) {
        f >> temporaryTaskInfo.taskID >> temporaryTaskInfo.workload >> temporaryTaskInfo.deadline;
        taskDependency.setNodeInfo(temporaryTaskInfo.taskID, temporaryTaskInfo);
    }

    f >> N_OF_DEPENDENCIES;
    int tempNode1, tempNode2;
    for (int i = 0; i < N_OF_DEPENDENCIES; i++) {
        f >> tempNode1 >> tempNode2;
        taskDependency.addEdge(tempNode1, tempNode2);
    }

    f >> N_OF_CORES;
    CoreInfo tempCore{};
    for (int i = 0; i < N_OF_CORES; i++) {
        f >> tempCore.speed >> tempCore.powerRate;
        cores.push_back(tempCore);
    }
}

int main() {
    int N, M, K;
    f >> N;
    int maxNodes = N + 5;
    Graph<TaskInfo, int> graph(maxNodes);
    vector<CoreInfo> cores;
    parseInput(N, M, K, graph, cores);

    cout << "=== TASK SCHEDULING SYSTEM ===" << endl;

    if (isCyclic(N, graph, maxNodes)) {
        cout << "Cycle with 3 or more nodes detected, cannot schedule!" << endl;
        return 0;
    }

    vector<pair<int, int>> mutualDeps = findMutualDependencies(N, graph);
    if (!mutualDeps.empty()) {
       if (mutualDeps.size() * 2 > cores.size()) {
            cout << "Not enough cores to handle mutual dependencies!" << endl;
            return 0;
        }
    }

    ScheduleState optimalSchedule = scheduleWithEnergyMinimization(N, graph, cores, maxNodes);
    printSchedule(optimalSchedule, cores);

    return 0;
}