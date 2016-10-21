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



int per_tile_val[16][16] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                            ,{2,1,0,1,3,2,1,2,4,3,2,3,5,4,3,4}
                            ,{3,2,1,0,4,3,2,1,5,4,3,2,6,5,4,3}
                            ,{1,2,3,4,0,1,2,3,1,2,3,4,2,3,4,5}
                            ,{2,1,2,3,1,0,1,2,2,1,2,3,3,2,3,4}
                            ,{3,2,1,2,2,1,0,1,3,2,1,2,4,3,2,3}
                            ,{4,3,2,1,3,2,1,0,4,3,2,1,5,4,3,2}
                            ,{2,3,4,5,1,2,3,4,0,1,2,3,1,2,3,4}
                            ,{3,2,3,4,2,1,2,3,1,0,1,2,2,1,2,3}
                            ,{4,3,2,3,3,2,1,2,2,1,0,1,3,2,1,2}
                            ,{5,4,3,2,4,3,2,1,3,2,1,0,4,3,2,1}
                            ,{3,4,5,6,2,3,4,5,1,2,3,4,0,1,2,3}
                            ,{4,3,4,5,3,2,3,4,2,1,2,3,1,0,1,2}
                            ,{5,4,3,4,4,3,2,3,3,2,1,2,2,1,0,1}
                            ,{6,5,4,3,5,4,3,2,4,3,2,1,3,2,1,0}};

int manhattan_h2(state_t *actual){
    int h = 0;
    for (int i = 0; i < NUMVARS; ++i)
        h += per_tile_val[actual->vars[i]][i];
    return h;
}


/* No tree needed*/
void best_first_search(state_t& root){
    int cp;
    int rule;
    // int last_h;
    
    PriorityQueue<state_t*> pq;
    ruleid_iterator_t iterator;

    state_t *new_state;
    state_t *child_state;
    state_map_t *map      = new_state_map();
    state_map_t *map_dist = new_state_map();


    state_map_add(map,&root, GRAY); // distance map
    state_map_add(map,&root, 0   ); // distance map


    pq.Add(0,0,&root);

    while (! pq.Empty() ){
        int *actual_color;


        new_state = pq.Top();
        pq.Pop();

        if (is_goal(new_state)) return;

        while ((rule = next_ruleid(&iterator)) >= 0) {
            int g = *state_map_get(map_dist,new_state) + get_fwd_rule_cost(rule);

            apply_fwd_rule(rule,new_state,child_state);       
            
            int hx = manhattan_h2(child_state);

            if ( hx == numeric_limits<int>::max()) continue; 


            actual_color = state_map_get(map,child_state);

            // white color
            if (! actual_color) {

                // init new node as gray with actual distance
                state_map_add(map,child_state, GRAY);
                state_map_add(map_dist,child_state, g);
                
                // add to queue with distance + heuristic cost
                pq.Add(g + hx,g + hx,child_state);
            }
            else if (g < *state_map_get(map_dist, child_state)){ // MEMORY LEAK?

                // update path with one cheaper
                state_map_add(map_dist,child_state, g);

                if (actual_color == GRAY) continue; // pq.Modify(,,i,); // what...
                else {
                    state_map_add(map,child_state, GRAY);
                    pq.Add(g + hx,g + hx,child_state);
                }
            }
        }
        state_map_add(map,new_state, BLACK);
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


    best_first_search(state);

    return 0;
}