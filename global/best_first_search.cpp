/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout each state and its distance (distance first, then the state) and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/


#include <limits>
#include <vector>
#include <iostream>
#include <tuple>
#include "priority_queue.hpp"

using namespace std;

#define WHITE 0
#define GRAY  1
#define BLACK 2

struct node {
    node    *parent;
    state_t *child;
    int distance;
};

inline int  get_distance(node& n)        { return n->distance;}
inline void set_distance(node& n,int nd) { n->distance = nd;  }

node *make_node(node& parent,state_t& child){
    node *new_node = new node;

    new_node->parent = parent;
    new_node->child  = child;
    return new_node;
}

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int actual_level, ruleid, max_bound,max_bound_backup,status;
    int move_to_make;
    int bwd_move;
    
    char first_state[511];
    ruleid_iterator_t *actual_m_iter;


    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    cout << "Introduce un estado> ";
    cin.ignore();
    cin.getline(first_state,511);

    // cout << first_state << flush;

    status = read_state(first_state,&state);

    if( status <= 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }

    return 0;
}


node* best_first_search(int (*heuristic)(state_t),state_t& root){
    int cp;
    int rule;
    
    PriorityQueue<node> pq;
    ruleid_iterator_t iterator;

    node    *new_node;
    state_t *new_state;
    state_t *aux_child;
    // state_map_t *map = new_state_map();

    // state_map_add(map, root, 0); // distance map


    while (! pq.empty() ){
        cp  = pq.CurrentPriority();

        new_node = pq.pop();
        state_t *new_state = new_node->child;

        if (cp < get_distance(new_node)) {
            set_distance(new_node,cp);
            if (is_goal(new_state)) return new_node;

            init_fwd_iter(&iterator,new_state);

            while ((rule = next_ruleid(&iterator)) >= 0) {
                int hx;
                int new_val;
                // state_t * child = new state_t;
                apply_fwd_rule(rule,new_state,&aux_child);

                hx = heuristic(aux_child);

                if ( hx == numeric_limits<int>::max()) continue;
                // marcar nuevo como gris
                new_val = cp + hx + get_fwd_rule_cost(rule);
                pq.Add(new_val, new_val, make_node(new_node,aux_child));

            // marcar como negro

                
            }
        }
    }

    return NULL;
}

int manhattan_h(state_t& actual){
    /*
        0  1  2  3
        4  5  6  7
        8  9  10 11
        12 13 14 15 
    */

    return 1;
}


