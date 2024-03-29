/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout each state and its distance (distance first, then the state) and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <time.h>
#include <limits>
#include <vector>
#include <iostream>
#include <csignal>
#include <tuple>
#include <cmath>  
#include "priority_queue.hpp"
#include "manhattan.cpp"

using namespace std;


void signalHandler( int signum )
{
    cout << "na, na, na, na\n" << flush;

   exit(signum);  
}



#define GRAY  0
#define BLACK 1

#define ROWS 4
#define COLS 4


// manhattan distance for every tile, dest
int per_tile_val[16][16] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                           ,{1,0,1,2,2,1,2,3,3,2,3,4,4,3,4,5}
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
    // manhattan with per tile distances precalculated
    int h = 0;
    for (int i = 0; i < NUMVARS; ++i)
        h += per_tile_val[static_cast<int>(actual->vars[i])][i];
    return h;
}

unsigned gap_h(state_t *state){
    //Calculo de heuristica gap
    int c;
    c=0;
    for(int x = 0; x < NUMVARS-1; x++)
        if (abs(state->vars[x]-state->vars[x+1]) > 1) c++;

    if (abs(state->vars[NUMVARS-1]-NUMVARS) > 1) c++;

    return c;
}

/* No tree needed*/
tuple<unsigned,unsigned> best_first_search(state_t& root){
    int cp;
    int rule;
    unsigned long num_nodes = 0;
    
    PriorityQueue<state_t*> pq;
    ruleid_iterator_t iterator;

    state_t *new_state;
    state_t aux_child;
    state_t *child_state;
    state_map_t *map      = new_state_map();
    state_map_t *map_dist = new_state_map();
    

    state_map_add(map     ,&root, GRAY); // color map
    state_map_add(map_dist,&root, 0   ); // distance map

    pq.Add(0,1,&root)
    
    while (! pq.Empty() ){
        int *actual_color;

        new_state = pq.Top();
        pq.Pop();

        if (is_goal(new_state)){ 
            return make_tuple(*state_map_get(map_dist,new_state),num_nodes);
        }

        init_fwd_iter(&iterator,new_state);
        num_nodes++;


        while ((rule = next_ruleid(&iterator)) >= 0) {

            int g = *state_map_get(map_dist,new_state) + get_fwd_rule_cost(rule);
            child_state = new state_t;
            apply_fwd_rule(rule,new_state,child_state);       

            // int hx = manhattan_h2(child_state);
            int hx = gap_h(child_state);

            if ( hx == numeric_limits<int>::max()) continue; 


            actual_color = state_map_get(map,child_state);

            // white color
            if (! actual_color) {

                // init new node as gray with actual distance
                state_map_add(map,child_state, GRAY);
                state_map_add(map_dist,child_state, g);
                pq.Add(g + hx,1,child_state);

                
            }
            else if (g < *state_map_get(map_dist, child_state)){ 

                // update path with one cheaper
                state_map_add(map_dist,child_state, g);

                if (*actual_color == GRAY) 
                    pq.Add(g + hx,1,child_state);
                else{
                    state_map_add(map,child_state, GRAY);
                    pq.Add(g + hx,1,child_state);
                }
                
            }
        }
        state_map_add(map,new_state, BLACK);
        if (new_state != &root)
            delete new_state;
    }

    return make_tuple(0,0);
}


int main(int argc, char **argv) {
    int status;
    char first_state[255];
    clock_t t_ini, t_fin;

    ruleid_iterator_t *actual_m_iter;


    // cin.ignore();
    cin.getline(first_state,255);

    status = read_state(first_state,&state);

    if( status < 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }

    // int h0 = gap_h(&state);
    int h0 = manhattan_h2(&state);

    cout << "X, a*,manhattan_h2,15puzzle, " << "\""<< first_state << "\", " << flush ;

    // Catch timeout
    signal(SIGALRM,signalHandler);

    t_ini = clock();
    tuple<unsigned,unsigned> dis_num = best_first_search(state);
    t_fin = clock();

    double secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

/*    costo h0  generated time gen_per_sec*/
    cout << 
    get<0>(dis_num) <<","  << 
    h0              << "," << 
    get<1>(dis_num) << "," <<
    secs            << "," <<
    get<1>(dis_num) / secs << "\n" << flush;;

    return 0;
}