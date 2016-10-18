//Necesito cost, generated, time
#include <vector>
#include <iostream>
#include <tuple>
#include <time.h>

using namespace std;

tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  d, int  bound, int history);

int main(int argc, char **argv) {
    clock_t t_ini, t_fin;
    double secs, genxs;

	int bound, status;
	char first_state[511];
	state_t state;
    bool goal;
    unsigned nodos_gen;

    cout << "Introduce un estado> ";
    cin.getline(first_state,511);

    t_ini = clock();
    status = read_state(first_state,&state);

    if( status <= 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }

//-------------------------------------------------------
    bound=0;
    nodos_gen = 0;
    //Perform depth-bounded searches with increasing depth bounds
    while (true){
        nodos_gen++;
    	tuple<unsigned,unsigned,bool> p = bounded_id_dfs(state, 0, bound, init_history);
        nodos_gen+= get<1>(p);
    	if (get<2>(p) == 1){
            t_fin = clock();
            secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
            genxs = nodos_gen / secs;
            cout << "\nX, id_dfs, tower14_4, " << "\""<< first_state << "\", " << get<0>(p)<< ", "<< nodos_gen << ", "<< secs << ", "<< genxs <<"\n";
 			return 0;
    	}	
    	bound =  bound +1;
    }

}
//Iterative deepening depth-first search
// <costo, generados, goal?>
tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  d, int  bound, int history){
		int ruleid;
	    ruleid_iterator_t posible_moves;
	    state_t child;
        bool goal;
        unsigned childCount;
        int history_hijo;

        childCount = 0;
	    init_fwd_iter(&posible_moves, &state);

	    //Caso Base
    	if (d > bound) return make_tuple(0, childCount, false);
    	if (is_goal(&state)){
            return (make_tuple(d, childCount, true));
        }

    	//expansion and recursion
        while ( (ruleid = next_ruleid(&posible_moves)) >= 0 ) {
            if ( fwd_rule_valid_for_history(history,ruleid) == 0 ) continue;
	        apply_fwd_rule(ruleid, &state, &child);
            history_hijo = next_fwd_history(history,ruleid);
	        ++childCount;
	        tuple<unsigned,unsigned,bool> px = bounded_id_dfs(child, d+1, bound, history_hijo);
            childCount+= (get<1>(px));
	        if ((get<2>(px))  == 1) return make_tuple(get<0>(px), childCount,true);	

        }
        return make_tuple(0, childCount,false);
}