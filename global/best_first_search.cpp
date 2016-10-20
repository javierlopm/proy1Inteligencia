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
#include <cmath>  
#include "priority_queue.hpp"

using namespace std;

#define GRAY  0
#define BLACK 1

#define ROWS 4
#define COLS 4

struct node_manhattan {
    int my_h;
    unsigned char blank;
    state_t *child;
};


inline unsigned char get_blank(int rule_id){
    return 5;
}

unsigned char get_first_blank(state_t& actual){
    for (int i = 0; i < NUMVARS; ++i)
        if (actual.vars[i] == static_cast<int8_t>(0)) 
            return static_cast<char>(i);
}

int init_manhattan(state_t& actual){
    int h = 0;


    for (int i = 0; i < NUMVARS; ++i)
        if (actual.vars[i] != static_cast<int8_t>(0)){
            h += abs( static_cast<int>(actual.vars[i]) / ROWS - i / ROWS );
            h += abs( static_cast<int>(actual.vars[i]) % COLS - i % COLS );
        }

    return h;
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

node_manhattan* make_node(int p_h,unsigned int m_b, state_t *c){
    node_manhattan * new_node = new node_manhattan;

    new_node->my_h  = p_h;
    new_node->blank = m_b;
    new_node->child = c;

    return new_node;
}

void destroy_node(node_manhattan *n){
    delete n->child;
    delete n; 
}


/* No tree needed*/
void best_first_search(state_t& root){
    int cp;
    int rule;
    int last_h;
    
    PriorityQueue<node_manhattan*> pq;
    ruleid_iterator_t iterator;

    node_manhattan *last_node,*node_creator;
    state_t *new_state;
    state_t *aux_child;
    state_map_t *map      = new_state_map();
    state_map_t *map_dist = new_state_map();


    state_map_add(map,&root, GRAY); // distance map
    state_map_add(map,&root, 0   ); // distance map

    last_h = init_manhattan(root);

    last_node =  make_node(last_h,get_first_blank(root),&root);

    pq.Add(0,0,last_node);

    while (! pq.Empty() ){
        int *actual_color;

        // cp  = pq.CurrentPriority();

        last_node = pq.Top();
        pq.Pop();
        // state_t *new_state = last_node->child;

        if (is_goal(last_node->child)) return;

        while ((rule = next_ruleid(&iterator)) >= 0) {
            int g = *state_map_get(map_dist,last_node->child) + get_fwd_rule_cost(rule);

            int hx = manhattan_h(last_node->my_h,
                                 last_node->blank,
                                 get_blank(rule));

            if ( hx == numeric_limits<int>::max()) continue; 

            apply_fwd_rule(rule,last_node->child,aux_child);       

            actual_color = state_map_get(map,aux_child);

            // white color
            if (! actual_color) {

                // init new node as gray with actual distance
                node_creator = make_node(last_h,hx,aux_child);

                state_map_add(map,aux_child, GRAY);
                state_map_add(map_dist,aux_child, g);
                
                // add to queue with distance + heuristic cost
                // hx = manhattan_h(last_node.my_h,last_node->blank,get_blank(rule));
                pq.Add(g + hx,g + hx,node_creator);
            }
            else if (g < *state_map_get(map_dist, aux_child)){ // MEMORY LEAK?

                // update path with one cheaper
                state_map_add(map_dist,aux_child, g);

                if (actual_color == GRAY) continue; // pq.Modify(,,i,); // what...
                else {
                    state_map_add(map,aux_child, GRAY);
                    node_creator = make_node(last_h,last_node->my_h,aux_child);
                    pq.Add(g + hx,g + hx,node_creator);
                }
            }
        }
        state_map_add(map,last_node->child, BLACK);
        delete last_node;
    }
}

// unsigned char blank_list[48] = {
//     1,2,3,5,6,7,9,10,11,13,14,15,
//     2,3,4,5,6,7,8,9,10,11,12,13,14,15,

// }



int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int actual_level, ruleid, max_bound,max_bound_backup,status;
    int move_to_make;
    int bwd_move;
    
    char first_state[255];
    ruleid_iterator_t *actual_m_iter;


    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    cout << "Introduce un estado> ";
    // cin.ignore();
    cin.getline(first_state,255);

    // cout << first_state << flush;

    status = read_state(first_state,&state);

    if( status < 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }

    init_manhattan(state);

    return 0;

    best_first_search(state);

    return 0;
}