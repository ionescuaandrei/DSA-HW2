#include "headers/adjacencymatrix.cpp.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace  std;

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

void parseInput(int N_OF_TASKS, int N_OF_DEPENDENCIES, int N_OF_CORES, Graph<TaskInfo, int>&taskDependency, vector<CoreInfo>&cores) {
    TaskInfo temporaryTaskInfo{};
    for (int i = 0; i < N_OF_TASKS; i++) {
        f >> temporaryTaskInfo.taskID >> temporaryTaskInfo.workload >> temporaryTaskInfo.deadline;
       taskDependency.setNodeInfo(temporaryTaskInfo.taskID-1, temporaryTaskInfo);
    }
    f>> N_OF_DEPENDENCIES;
    int tempNode1, tempNode2;
    for (int i = 0; i < N_OF_DEPENDENCIES; i++) {
        f >> tempNode1 >> tempNode2;
        taskDependency.addEdge(tempNode1-1, tempNode2-1);
    }
    f>> N_OF_CORES;
    CoreInfo tempCore{};
    for (int i = 0; i < N_OF_CORES; i++) {
        f >> tempCore.speed>> tempCore.powerRate;
        cores.push_back(tempCore);
    }
}

void testOutput(int n, int m, int k, Graph<TaskInfo, int>&taskDependency, vector<CoreInfo>&cores) {
    cout <<"Number of tasks: " << n << endl << endl;
    for (int i = 0; i < n; i++) {
        TaskInfo temp = taskDependency.getNodeInfo(i);
        cout << "Task ID: " << temp.taskID << endl;
        cout << "Workload: " << temp.workload << endl;
        cout << "Deadline: " << temp.deadline << endl;
        cout << endl;
    }
    cout << endl <<  "Task dependencies:" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (taskDependency.checkEdge(i, j) == 1) cout << i << " " << j << endl;
        }
    }
}

int main() {

    int N, M, K;
    f>>N;
    Graph<TaskInfo, int> graph(N);
    vector<CoreInfo> cores;
    parseInput(N, M, K, graph, cores);

    testOutput(N,M,K, graph, cores);



    return 0;
}
