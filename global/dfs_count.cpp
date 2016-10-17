/*
This program computes the distance to goal (i.e. the cost of the least-cost path to the goal)
of every state from which the goal can be reached.
It does this by executing Dijkstra's algorithm backwards from the goal.
It prints on stdout each state and its distance (distance first, then the state) and, if a filename is
provided as a command line argument, it prints the state_map it builds to that file.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <vector>
#include <iostream>

using namespace std;


int main(int argc, char **argv) {
    state_t state, child;   // NOTE: "child" will be a predecessor of state, not a successor
    int actual_level, ruleid, max_bound,max_bound_backup,status;
    int move_to_make;
    int bwd_move;
    
    char first_state[511];
    ruleid_iterator_t *actual_m_iter;

    vector<long> level_count; // per level count of nodes
    vector<int>  moves_made;  // moves made (needed to go back)
    vector<int>  histories;   // history (for pruning)

    vector<ruleid_iterator_t*> moves_vector; // iterators per level

    actual_level = 0;


    cout << "Introduce la cantidad máxima de niveles > ";
    cin  >> max_bound;
    max_bound_backup = max_bound;

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

    for (int i = 0; i <= max_bound; ++i)
        level_count.push_back(0);

    for (int i = 0; i <= max_bound; ++i)
        histories.push_back(init_history);

    for (int i = 0; i <= max_bound; ++i)
        moves_made.push_back(0);

    // Un init más que el requerido
    for (int i = 0; i <= max_bound+1; ++i) {
        actual_m_iter = new ruleid_iterator_t;
        moves_vector.push_back(actual_m_iter);
    }

    level_count[0] = 1;
    
    init_fwd_iter(moves_vector[0],&state);

    do {
        /* Make a move until we reach bound*/
        while( actual_level <= max_bound ) {
    
            /* Make move */
            move_to_make = next_ruleid(moves_vector[actual_level]);

            if (move_to_make < 0) break;

            if ( fwd_rule_valid_for_history(histories[actual_level],move_to_make) == 0 ) continue;

            moves_made[actual_level] = move_to_make;
            apply_fwd_rule(move_to_make,&state,&child);
            histories[actual_level+1] = next_fwd_history(histories[actual_level],move_to_make);

            copy_state(&state,&child);

            /* count child */
            actual_level++;
            level_count[actual_level]++;
            
            /* Initialize next iterator*/
            init_fwd_iter(moves_vector[actual_level],&state);

        }
        
        
        /* Going back one level */
        actual_level--;
        apply_bwd_rule(moves_made[actual_level],&state,&child);
        copy_state(&state,&child);

    } while ( actual_level >= 0);

            

    
    cout << "LEVELS | Children \n";
    for (int i = 0; i <= max_bound_backup; ++i)
        cout << i << " " << level_count[i] << "\n" ;
    
    return 0;
}

