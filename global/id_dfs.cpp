//Necesito cost, generated, time
#include <vector>
#include <iostream>
#include <tuple>

using namespace std;

tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  d, int  bound);

int main(int argc, char **argv) {

	int bound, status;
	char first_state[511];
	state_t state;
    bool goal;
    unsigned nodos_gen;

    cout << "Introduce un estado> ";
    //cin.ignore();
    cin.getline(first_state,511);

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
    	tuple<unsigned,unsigned,bool> p = bounded_id_dfs(state, 0, bound);
        nodos_gen+= get<1>(p);
    	if (get<2>(p) == 1){
            cout << "Nodos generados: " << nodos_gen << "\n";
            cout << "Costo: " << get<0>(p) << "\n";
 			return 0;
    	}	
    	bound =  bound +1;

        //cout << "\nBOUND: " << bound << "\n";
    }

}
//Iterative deepening depth-first search
// <costo, numnodos generados>
tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  d, int  bound){
		int ruleid;
	    ruleid_iterator_t posible_moves;
	    state_t child;
        bool goal;
        unsigned childCount;

        childCount = 0;
	    init_fwd_iter(&posible_moves, &state);

	    //Caso Base
    	if (d > bound) return make_tuple(0, childCount, false);
    	if (is_goal(&state)){
            return (make_tuple(d, childCount, true));
        }

    	//expansion and recursion
        while ( (ruleid = next_ruleid(&posible_moves)) >= 0 ) {
	        apply_fwd_rule(ruleid, &state, &child);
            //cout << "Regla aplicada: " << ruleid<< ". ";
            //printf("Child: ");
            //print_state(stdout, &child);
            //printf("\n");
	        ++childCount;
	        tuple<unsigned,unsigned,bool> px = bounded_id_dfs(child, d+1, bound);
            childCount= childCount + (get<1>(px));
	        if ((get<2>(px))  == 1) return make_tuple(get<0>(px), childCount,true);
	        //t = min(t, get<2>(p));
	        //childCount+=get<2>(p);	

        }
        return make_tuple(0, childCount,false);
}