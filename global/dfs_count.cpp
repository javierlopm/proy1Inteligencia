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
    int history;
    char first_state[255] = "15 4 3 20 14 5 1 2 6 10 9 11 16 0 8 7 12 13 17 19 18 21";
    ruleid_iterator_t *actual_m_iter;

    vector<long> level_count;
    vector<int> moves_made;
    vector<ruleid_iterator_t*> moves_vector;

    actual_level = 0;


    cout << "Introduce la cantidad máxima de niveles > ";
    cin  >> max_bound;
    max_bound_backup = max_bound;

    FILE *file; // the final state_map is written to this file if it is provided (command line argument)

    // cout << "Introduce un estado> ";
    // cin >> first_state;

    // cout << first_state << flush;

    status = read_state(first_state,&state);
    if( status <= 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }
    history = init_history;
    for (int i = 0; i <= max_bound; ++i)
        level_count.push_back(0);

    for (int i = 0; i <= max_bound; ++i)
        moves_made.push_back(0);

    // Un init más que el requerido
    for (int i = 0; i <= max_bound+1; ++i) {
        actual_m_iter = new ruleid_iterator_t;
        moves_vector.push_back(actual_m_iter);
    }

    init_fwd_iter(moves_vector[0],&state);

    do {
        /* Make a move until we reach bound*/
        while( actual_level <= max_bound ) {
    
            /* Make move */
            move_to_make = next_ruleid(moves_vector[actual_level]);

            if (move_to_make < 0) break;

            if ( fwd_rule_valid_for_history(history,move_to_make) == 0 ) continue;
                moves_made[actual_level] = move_to_make;
                apply_fwd_rule(move_to_make,&state,&child);
                history = next_fwd_history(history,move_to_make);
    
                copy_state(&state,&child);
    
                /* count child */
                level_count[actual_level] = level_count[actual_level] + 1;
                
                /* Initialize next iterator*/
                actual_level++; // deberia almacenar el movimiento que realizó
                init_fwd_iter(moves_vector[actual_level],&state);

            // cout << "arrib ";
            // for (int i = 0; i <= max_bound_backup; ++i)
            //     cout  << level_count[i] << " " ;
            // cout << "\n" << flush;
        }
        
        
        /* Going back one level */
        actual_level--;
        apply_bwd_rule(moves_made[actual_level],&state,&child);
        copy_state(&state,&child);
        history = next_bwd_history(history,moves_made[actual_level]);

    } while ( actual_level > 0);

            

    
    cout << "LEVELS | Children \n";
    for (int i = 0; i <= max_bound_backup; ++i)
        cout << i << " " << level_count[i] << "\n" ;
    
    return 0;
}

