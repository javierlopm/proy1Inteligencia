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

struct node_manhattan {
    int distance;
    int parent_h;
    unsigned char parent_blank;
    unsigned char my_blank;
    state_t *child;
};

node_manhattan* make_manhattan_node(int d, int p_h,unsigned int p_bl,unsigned int m_b, state_t *c){
    node_manhattan * new_node = new node_manhattan;

    new_node->distance     = d;
    new_node->parent_h     = p_h;
    new_node->parent_blank = p_bl;
    new_node->my_blank     = m_b;
    new_node->child        = c;

    return node_manhattan;
}

int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int actual_level, ruleid, max_bound,max_bound_backup,status;
    int move_to_make;
    int bwd_move;
    
    char first_state[255];
    ruleid_iterator_t *actual_m_iter;


    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    cout << "Introduce un estado> ";
    cin.ignore();
    cin.getline(first_state,255);

    // cout << first_state << flush;

    status = read_state(first_state,&state);

    if( status <= 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }

    return 0;
}

/* No tree needed*/
void best_first_search(int (*heuristic)(state_t),state_t& root){
    int cp;
    int rule;
    
    PriorityQueue<node> pq;
    ruleid_iterator_t iterator;

    node    *new_node;
    state_t *new_state;
    state_t *aux_child;
    // state_map_t *map = new_state_map();

    // state_map_add(map, root, 0); // distance map


    while (! pq.Empty() ){
        cp  = pq.CurrentPriority();

        new_node = pq.Top();
        pq.Pop();
        // state_t *new_state = new_node->child;

        if (cp < new_node->distance) {
            set_distance(new_node,cp);
            new_node->distance = cp;

            if (is_goal(new_state)) return;

            init_fwd_iter(&iterator,new_state);

            while ((rule = next_ruleid(&iterator)) >= 0) {
                int hx;
                int new_val;
                // state_t * child = new state_t;
                apply_fwd_rule(rule,new_state,aux_child);

                hx = heuristic(*aux_child);

                if ( hx == numeric_limits<int>::max()) continue;
                
                // marcar nuevo como gris
                new_val = cp + hx + get_fwd_rule_cost(rule);
                pq.Add(new_val, new_val, *make_node(new_node,aux_child));

            // marcar como negro

                
            }
        }
    }
}

// unsigned char blank_list[48] = {
//     1,2,3,5,6,7,9,10,11,13,14,15,
//     2,3,4,5,6,7,8,9,10,11,12,13,14,15,

// }

inline unsigned char get_blank(int rule_id){
    return 5;
}

int init_manhattan(state_t& actual){
    return 1;
}

int manhattan_h(int old_manhattan,
                unsigned char old_blank,
                unsigned char new_blank){
    /*
        0  1  2  3
        4  5  6  7
        8  9  10 11
        12 13 14 15 
    */

    return 1;
}


