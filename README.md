# Data Structures and Algorithms – Homework 2 (DSA-HW2)

**Course:** Data Structures and Algorithms  
**University:** University POLITEHNICA of Bucharest (UPB)  
**Student:** Andrei Ionescu, Stancu Stefanita Ionut - group 1211ED

**Deadline:** May 26, 2025, 08:00 AM  
**Repository:** [DSA-HW2 on GitHub](https://github.com/ionescuaandrei/DSA-HW2)

## 🔧 Project Structure

The project consists of three main exercises, each implemented using specific data structures (graphs or binary trees). Each exercise is placed in its own folder and contains source files (`.cpp` and `.h`) only — no executables or build files.


## 📘 Problem Descriptions

### 🧩 1. Digital Combo Lock

**Points:** 3.5  
**Data Structure Used:** De Bruijn Graph (Graph Traversal)

#### 📝 Problem:
You must generate the shortest sequence of digits such that every possible combination of `n` digits over an alphabet of `k` digits (from `0` to `k-1`) appears as a substring. The lock checks the last `n` digits continuously.

This is equivalent to generating a **De Bruijn sequence**.

#### 🧪 Example:
- `n = 2, k = 2` → Output: `"00110"` (substrings: `00`, `01`, `11`, `10`)

#### 💡 Solution:
- Model the problem as a De Bruijn graph.
- Use a modified **DFS or Hierholzer’s Algorithm** to find an Eulerian path.
- Construct the final sequence from this traversal.

---

### 🏰 2. Castle Defence

**Points:** 3.0  
**Data Structure Used:** Binary Tree

#### 📝 Problem:
Given a tree of cities where each node represents a city, place dragons in such a way that every city is either:
- Protected directly (a dragon is placed there),
- Protected by its parent, or
- Protected by one of its children.

Your goal is to **minimize** the number of dragons.

#### 💡 Solution:
- Traverse the binary tree using **post-order DFS**.
- Use 3 states at each node:
  1. Node has a dragon.
  2. Node is covered by a dragon.
  3. Node is not yet covered.
- Only place a dragon if its children are not covered.

This is a greedy tree DP problem similar to the **minimum vertex cover**.

---
### ⚙️ 3. Core Scheduling

**Points:** 3.5  
**Data Structure Used:** Directed Graph (Dependency Graph)

#### 📝 Problem:
Given a set of tasks, each with:
- A workload,
- A deadline, and
- Dependencies (A must complete before B),

Assign them to **heterogeneous cores** (different speed/power) so that:
- All dependencies are respected,
- Deadlines are met,
- **Total energy is minimized.**

Tasks with **2-way circular dependencies** must be scheduled to start simultaneously on separate cores. Tasks in **larger cycles** make scheduling impossible.

#### 🔍 Input:
- `input.txt` includes:
  - Number of tasks, their workloads & deadlines
  - Dependencies
  - Core types (speed, power usage)

#### 💡 Solution:
1. **Cycle Detection**
   - Use DFS or topological sort to detect:
     - **Valid DAG**: proceed to scheduling
     - **2-node cycle**: handle with parallel execution
     - **Larger cycle**: no solution possible

2. **Scheduling Algorithm**
   - Use **state-space search** (priority queue) to track:
     - When each task can start
     - What core it uses
     - Total energy consumed

3. **Energy Calculation**
   - Execution time = `workload / speed`
   - Energy = `execution time * power`

If no valid schedule exists, print a failure message.

#### ✅ Output:
- Minimum energy consumed
- Per-task schedule: task ID, core ID, start time, end time, energy used

---

## ▶️ How to Run the Code

Each exercise can be compiled and run independently.
