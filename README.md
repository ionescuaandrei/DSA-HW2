# Data Structures and Algorithms – Homework 2 (DSA-HW2)

**Course:** Data Structures and Algorithms  
**University:** University POLITEHNICA of Bucharest (UPB)  
**Student:** Andrei Ionescu, Stancu Stefanita Ionut - group 1211ED

**Deadline:** May 26, 2025, 08:00 AM  
**Repository:** [DSA-HW2 on GitHub](https://github.com/ionescuaandrei/DSA-HW2)

## 🔧 Project Structure

The project consists of three main exercises, each implemented using specific data structures (graphs or binary trees). Each exercise is placed in its own folder and contains source files (`.cpp` and `.h`) only — no executables or build files.


## 📘 Problem Descriptions

### 🧩 1. Digital Combo Lock (3.5p)

**Data Structure Used:** Graph (De Bruijn Graph)  
**Problem:**  
Generate the shortest possible sequence of digits of length `k` using a digit range [0, k-1] such that every possible combination of length `n` appears at least once as a substring.

**Example:**  
For `n=2`, `k=2`, the output may be `"00110"` which covers: `00`, `01`, `11`, and `10`.

**Goal:**  
Implement an efficient algorithm that uses graph traversal (e.g., Eulerian path on De Bruijn graph) to generate the sequence.

---

### 🏰 2. Castle Defence (3p)

**Data Structure Used:** Binary Tree  
**Problem:**  
Place dragons in a binary tree of cities such that every city is either protected directly or by its parent/child.

**Rules:**
- A dragon protects: the city it's in, its parent, and its children.
- Goal is to minimize the number of dragons placed.

**Approach:**  
Post-order traversal with dynamic programming to determine the minimum number of dragons required for full coverage.

---

### ⚙️ 3. Core Scheduling with Deadlines and Dependency Cycles (3.5p)

**Data Structure Used:** Directed Graph (Task Dependency Graph)  
**Problem:**  
Given a set of tasks with deadlines and dependencies, schedule them on different cores to minimize energy consumption.

**Requirements:**
- Detect and handle cycles (2-node cycles can be scheduled in parallel, longer cycles invalidate the schedule).
- Schedule tasks while respecting:
  - Dependency constraints
  - Deadlines
  - Core capabilities (speed and energy use)

**Input:** `input.txt` containing task specs, dependencies, and core information.  
**Output:** Minimum energy consumption and task schedule or failure message.

---

## ▶️ How to Run the Code

Each exercise can be compiled and run independently.

