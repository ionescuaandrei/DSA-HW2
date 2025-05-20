#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include "bst.h"
#include "tree_solver.h"

using namespace std;

BinarySearchTree<int>* buildTreeFromInput(vector<string>& input) { // Match signature from header
    if (input.empty() || input[0] == "null") return nullptr;

    BinarySearchTree<int>* root = new BinarySearchTree<int>();
    root->setInfo(0);

    queue<BinarySearchTree<int>*> q;
    q.push(root);

    int i = 1;
    while (!q.empty() && i < input.size()) {
        BinarySearchTree<int>* node = q.front();
        q.pop();

        if (i < input.size() && input[i] != "null") {
            node->left_son = new BinarySearchTree<int>();
            node->left_son->setInfo(0);
            q.push(node->left_son);
        }
        i++;

        if (i < input.size() && input[i] != "null") {
            node->right_son = new BinarySearchTree<int>();
            node->right_son->setInfo(0);
            q.push(node->right_son);
        }
        i++;
    }

    return root;
}

int dfs(BinarySearchTree<int>* node, int& dragons) {
    if (!node) return 1; // Null nodes are considered covered

    int left = dfs(node->left_son, dragons);
    int right = dfs(node->right_son, dragons);

    if (left == 0 || right == 0) {
        dragons++;
        return 2; // Place dragon here
    }

    if (left == 2 || right == 2) {
        return 1; // Covered by child
    }

    return 0; // Not covered
}


int minDragons(BinarySearchTree<int>* root) {
    int dragons = 0;
    if (dfs(root, dragons) == 0) {
        dragons++; // Place dragon at root if uncovered
    }
    return dragons;
}



int main() {
    string line;
    getline(cin, line);

    stringstream ss(line);
    string token;
    vector<string> input;

    while (ss >> token) {
        input.push_back(token);
    }

    BinarySearchTree<int>* root = buildTreeFromInput(input);
    int result = minDragons(root);

    cout << result << endl;
    return 0;
}