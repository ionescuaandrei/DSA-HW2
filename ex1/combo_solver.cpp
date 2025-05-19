#include <vector>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

void dfs(int node, int k, vector<vector<bool>> &visited, string &result, int n) {
    for (int d = 0; d < k; ++d) {
        int next = (node * k + d) % (int)pow(k,n-1);
        if (!visited[node][d]) {
            visited[node][d] = true;
            dfs(next, k, visited, result, n);
            result += to_string(d);
        }
    }
}

// De Bruijn: https://media.geeksforgeeks.org/wp-content/uploads/de-bruijn-graph-300x174.png

string generate_sequence(int n, int k) {
    int num_nodes = pow(k,n);
    vector<vector<bool>> visited(num_nodes, vector<bool>(k, false));
    string result;

    dfs(0, k, visited, result, n);

    result += result.substr(0, n - 1);
    return result;
}

int main() {
    int n, k;
    cin >> n >> k;

    string sequence = generate_sequence(n, k);
    cout << sequence << endl;

    return 0;
}