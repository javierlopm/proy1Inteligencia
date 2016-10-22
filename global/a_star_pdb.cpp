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
#include <sstream>
#include "priority_queue.hpp"

using namespace std;

//VARIABLES GLOBALES
state_map_t *pdb[3];
abstraction_t *abst[3];

void signalHandler( int signum )
{
    cout << "na, na, na, na\n" << flush;

   exit(signum);  
}



#define GRAY  0
#define BLACK 1

#define ROWS 4
#define COLS 4



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
    int h = 0;
    for (int i = 0; i < NUMVARS; ++i)
        h += per_tile_val[static_cast<int>(actual->vars[i])][i];
    return h;
}

unsigned gap_h(state_t *state){
    int c;

    //Calculo de heuristica
    c=0;
    for(int x = 0; x < NUMVARS-1; x++)
        if (abs(state->vars[x]-state->vars[x+1]) > 1) c++;

    if (abs(state->vars[NUMVARS-1]-NUMVARS) > 1) c++;

    return c;
}

static int *generic_get(vector<state_map_t*>all_maps,state_t *found){
    // for(vector<state_map_t*>::iterator it = all_maps.begin(); it != all_maps.end(); ++it) {
    for(vector<state_map_t*>::iterator it = all_maps.begin(); it != all_maps.end(); ++it) {
        int *res = state_map_get(*it,found);
        if (res != NULL) return res;
    }
    return NULL;
}
void generic_add(vector<state_map_t*>all_maps,state_t *found,int new_elem,unsigned int *actual){
    int *res;
    for(vector<state_map_t*>::iterator it = all_maps.begin(); it != all_maps.end(); ++it) {
        // if element found in one map, insert there
        if ((res = state_map_get(*it,found)) != NULL){
            state_map_add(*it,found,new_elem);
            return;
        }
    }
    unsigned int counter = *actual;
    // If we didn't find it already we add it
    if (counter < (numeric_limits<unsigned int>::max()/3) -2){
        state_map_add(all_maps.back(),found,new_elem); // if there is space
        *actual = counter+1;
    }
    else { // even if there isnt
        state_map_t *new_map = new_state_map();
        all_maps.push_back(new_map);
        state_map_add(new_map,found,new_elem);
        *actual = 0; // we reset the node counter
    }

}

int pdb_h(state_t *actual){
    state_t abst_st;
    int h =0;

    for(int i = 0; i < 3; ++i){
        abstract_state(abst[i], actual, &abst_st);
        h += *(state_map_get(pdb[i], &abst_st));
    }
    return h;
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

    pq.Add(0,1,&root);
    
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
            int hx = pdb_h(child_state);

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

    // Compruebo argumentos
    // a_star_pdb prefijo1 prefijo2 prefijo3
    if( argc < 4 ) {
        cout << "Numero incorrecto de argumentos\n";
        exit(-1);
    }

    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int actual_level, ruleid, max_bound,max_bound_backup,status;
    int move_to_make;
    int bwd_move;
    clock_t t_ini, t_fin;

    char first_state[255];
    ruleid_iterator_t *actual_m_iter;

    FILE *file;


    // cin.ignore();
    cin.getline(first_state,255);

    status = read_state(first_state,&state);

    if( status < 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }
    
    char suffix[500], f_abst[500], f_pdb[500];

    for(int i = 1; i< 4; i++){
        strcpy(suffix,argv[i]);
        strcpy(f_abst, suffix);
        strcpy(f_pdb, suffix);

        strcat(f_abst, ".abst");
        strcat(f_pdb, ".pdb");

        //Leer
        cout << "abro archivo : " << f_abst<<"\n";
        abst[i-1] = read_abstraction_from_file(f_abst);
        cout << "abro archivo : " << f_pdb<<"\n";
        file = fopen(f_pdb, "r");
        cout << "abrí\n";
        pdb[i-1] = read_state_map(file);
        fclose(file);
    }

    //------------------------------------------------------------------------

    int h0 = pdb_h(&state);

    cout << "X, a*,pdb_h,15puzzle, " << "\""<< first_state << "\", " << flush ;

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