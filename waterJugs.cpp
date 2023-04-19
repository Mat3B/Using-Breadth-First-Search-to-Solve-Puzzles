/*
 * Name: Mathew Breland
 * Date Submitted: 4/18/2023
 * Lab Section: 001
 * Assignment Name: Lab 10
 */

#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// Reflects what each node represents...
// First value units of water in A, second units of water in B
typedef pair<int, int> state;

// Each string in edge_label must be one of the following:
const string actions[] = { "Fill A",
                          "Fill B",
                          "Empty A",
                          "Empty B",
                          "Pour A->B",
                          "Pour B->A" };

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state, state>, string> edge_label;

// GENERIC (breadth-first search, outward from source_node)
void search(state source_node)
{
    queue<state> to_visit;
    to_visit.push(source_node);
    visited[source_node] = true;
    dist[source_node] = 0;

    while (!to_visit.empty()) {
        state curnode = to_visit.front();
        to_visit.pop();
        for (state n : nbrs[curnode])
            if (!visited[n]) {
                pred[n] = curnode;
                dist[n] = 1 + dist[curnode];
                visited[n] = true;
                to_visit.push(n);
            }
    }
}

// GENERIC
void print_path(state s, state t)
{
    if (s != t) {
        print_path(s, pred[t]);
        cout << edge_label[make_pair(pred[t], t)] << ": " << "[" << t.first << "," << t.second << "]\n";
    }
    else {
        cout << "Initial state: " << "[" << s.first << "," << s.second << "]\n";
    }
}


vector<state> get_neighbors(state st)
{
    vector<state> neighbors;
    int a = st.first;
    int b = st.second;

    // filling a by pushing back a pair with 3 in the space of a
    neighbors.push_back(make_pair(3, b));

    // filling by pushing back a pair with 4 in b
    neighbors.push_back(make_pair(a, 4));

    // emptying a by putting 0 in a and keeping b
    neighbors.push_back(make_pair(0, b));

    // emptying b by putting 0 in b and keeping a
    neighbors.push_back(make_pair(a, 0));

    // Pour A -> B by keeping the value of a and 4-b the value of b
    int pour_amt = min(a, 4 - b);
    neighbors.push_back(make_pair(a - pour_amt, b + pour_amt));

    // Pour B -> A by keeping the value of b and 3-a the value of a
    pour_amt = min(b, 3 - a);
    neighbors.push_back(make_pair(a + pour_amt, b - pour_amt));

    return neighbors;
}

void build_graph(void)
{
    for (int a = 0; a <= 3; a++) {

        for (int b = 0; b <= 4; b++) {

            // creating st (a, b) which will hold the values of a and b
            state st(a, b);

            // creating a vector called neighbors that calls the neighbor function using st which is (a, b)
            vector<state> neighbors = get_neighbors(st);

            int i = 0;

            // for each iteration through neighbors push back and make a label
            for (state nextSt : neighbors) {

                nbrs[st].push_back(nextSt);
                edge_label[make_pair(st, nextSt)] = actions[i++];

            }
        }
    }
}
