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
// int with each bit == 0 left of river, bit == 1 right of river
typedef int state;

// Bit position representation for wolf/goat/cabbage/me
bool bit(int x, int i) { return (x >> i) & 1; }
const int wolf = 0, goat = 1, cabbage = 2, me = 3;

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state, state>, string> edge_label;

// GENERIC (breadth-first search, outward from curnode)
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

string state_string(state s)
{
    string items[4] = { "wolf", "goat", "cabbage", "you" }, result = "";
    for (int i = 0; i < 4; i++)
        if (!bit(s, i)) result += items[i] + " ";
    result += " |river| ";
    for (int i = 0; i < 4; i++)
        if (bit(s, i)) result += items[i] + " ";
    return result;
}

// GENERIC
void print_path(state s, state t)
{
    if (s != t) {
        print_path(s, pred[t]);
        cout << edge_label[make_pair(pred[t], t)] << ": " << state_string(t) << "\n";
    }
    else {
        cout << "Initial state: " << state_string(s) << "\n";
    }
}

string neighbor_label(int s, int t)
{
    string items[3] = { "wolf", "goat", "cabbage" }, which_cross;
    if (bit(s, me) == bit(t, me)) return "";  // must cross river
    int cross_with = 0;
    for (int i = 0; i < 3; i++) {
        if (bit(s, i) != bit(t, i) && bit(s, i) == bit(s, me)) { cross_with++; which_cross = items[i]; }
        if (bit(s, i) != bit(t, i) && bit(s, i) != bit(s, me)) return "";
    }
    if (cross_with > 1) return "";
    if (cross_with == 0) return "Cross alone";
    else return "Cross with " + which_cross;
}

    // need to fill in the nbrs and edge_label maps 
    // loop through each possible state and then generate each neighboring state 
    // as you generate the neighboring states you should add one to nbrs and edge_label maps

    // 16 different states 
    // each bit in a state is left or right side (0 = left and 1 = right)
    //bitset could be useful

    // 4 different actions 
    // person crosses river with wolf 
    // person crosses river with goat 
    // person crosses river with cabbage
    // person crosses river alone 
    // person can only cross river with something if that thing is on the same side of the river as the person
    // if a bad state (goat and wolf on one side without the person or goat and cabbage on one side, we want to ignore this state)

void build_graph(void)
{
    // Generate all possible states with 16
    for (int i = 0; i < 16; i++) {

        if (bit(i, wolf) == bit(i, goat) && bit(i, me) != bit(i, goat)) continue;
        if (bit(i, goat) == bit(i, cabbage) && bit(i, me) != bit(i, goat)) continue;

        vector<int> v;

        for (int j = 0; j < 16; j++) {  // for loop that runs through all possible states 

            string n = neighbor_label(i, j);    // setting string N that is equal to the neighbor_label(1, 2)

            if (n != "") {  // if N is valid (not empty)

                if (j != 3 && j != 6 && j != 7 && j != 8 && j != 9 && j != 12) {

                    // pushing back the value of J
                    nbrs[i].push_back(j);

                    // making a label with (i, j) and setting it = to N
                    edge_label[make_pair(i, j)] = n;
                }
            }
        }
    }
}
