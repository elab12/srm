// Experiment 7: Banker's Algorithm for deadlock avoidance (safety algorithm)
// SRM Institute of Science and Technology - OS Lab (21CSC202J)
//
// Compile: g++ e7.cpp -o e7
// Run:     ./e7

#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, m;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resource types: ";
    cin >> m;

    vector<vector<int>> alloc(n, vector<int>(m)), maxm(n, vector<int>(m)), need(n, vector<int>(m));
    vector<int> avail(m);

    cout << "Enter the available resources: ";
    for (int j = 0; j < m; j++) cin >> avail[j];

    for (int i = 0; i < n; i++)
    {
        cout << "\nProcess P" << i << " details\n";
        cout << "Enter Allocation: ";
        for (int j = 0; j < m; j++) cin >> alloc[i][j];
        cout << "Enter Max: ";
        for (int j = 0; j < m; j++) cin >> maxm[i][j];
        for (int j = 0; j < m; j++) need[i][j] = maxm[i][j] - alloc[i][j];
    }

    cout << "\nProcess\tAllocation\tMax\t\tNeed\n";
    for (int i = 0; i < n; i++)
    {
        cout << "P" << i << "\t";
        for (int j = 0; j < m; j++) cout << alloc[i][j] << " ";
        cout << "\t\t";
        for (int j = 0; j < m; j++) cout << maxm[i][j] << " ";
        cout << "\t\t";
        for (int j = 0; j < m; j++) cout << need[i][j] << " ";
        cout << "\n";
    }

    vector<bool> finished(n, false);
    vector<int> safeSeq;
    vector<int> work = avail;

    for (int count = 0; count < n; count++)
    {
        bool found = false;
        for (int i = 0; i < n; i++)
        {
            if (finished[i]) continue;
            bool canRun = true;
            for (int j = 0; j < m; j++)
                if (need[i][j] > work[j]) { canRun = false; break; }
            if (canRun)
            {
                for (int j = 0; j < m; j++) work[j] += alloc[i][j]; // process releases resources
                finished[i] = true;
                safeSeq.push_back(i);
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "\nSystem is NOT in a safe state (deadlock possible)\n";
            return 0;
        }
    }

    cout << "\nSystem is in a SAFE state.\nSafe sequence: ";
    for (size_t i = 0; i < safeSeq.size(); i++)
        cout << "P" << safeSeq[i] << (i + 1 < safeSeq.size() ? " -> " : "\n");

    return 0;
}
