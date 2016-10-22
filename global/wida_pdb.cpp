//Necesito cost, generated, time
#include <vector>
#include <iostream>
#include <tuple>
#include <time.h>
#include <csignal>
#include <algorithm>

using namespace std;

//VARIABLES GLOBALES
state_map_t *pdb[3];
abstraction_t *abst[3];
unsigned nodos_gen = 0;
float k = 1 + 0; //0 = gap


void signalHandler( int signum )
{
    cout << "na, na, na, na\n" << flush;
   exit(signum);  

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

tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  d, int  bound, int history);

int main(int argc, char **argv) {

    // Compruebo argumentos
    // a_star_pdb prefijo1 prefijo2 prefijo3
    if( argc < 4 ) {
        cout << "Numero incorrecto de argumentos\n";
        exit(-1);
    }

    clock_t t_ini, t_fin;
    double secs, genxs;

	int bound, status;
    float Epsilon;
	char first_state[511];
	state_t state;
    bool goal;
    FILE *file;
    signal(SIGALRM,signalHandler);

    // cout << "Introduce un estado> ";
    cin >> Epsilon;
    cin.ignore();
    cin.getline(first_state,511);
    k=1+Epsilon;
    //for (int i=0; i < 28; i++)
    //    cout << first_state[i]<< " ";
    //cout << "\n";

    t_ini = clock();
    status = read_state(first_state,&state);
    if( status <= 0 ) {
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
        // cout << "abro archivo : " << f_abst<<"\n";
        abst[i-1] = read_abstraction_from_file(f_abst);
        // cout << "abro archivo : " << f_pdb<<"\n";
        file = fopen(f_pdb, "r");
        // cout << "abrí\n";
        pdb[i-1] = read_state_map(file);
        fclose(file);
    }

//-------------------------------------------------------
    cout << "X, wida, pdb5+5+5, 15-puzzle, " << "\""<< first_state << "\", " << flush ;

    bound=k*pdb_h(&state);
    //int costo = 0;
    //Perform depth-bounded searches with increasing depth bounds
    while (true){
        nodos_gen++;
    	tuple<unsigned,unsigned,bool> p = bounded_id_dfs(state, 0, bound, init_history);
        //costo+= get<1>(p);
        bound =  -get<1>(p);
    	if (get<2>(p) == 1){
            t_fin = clock();
            secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
            genxs = nodos_gen / secs;

            int h0 = k*pdb_h(&state);
            //int costo = static_cast<int>(get<1>(p));
            cout << get<0>(p)<< ", " << h0 << ", "<< nodos_gen << ", " << secs << ", "<< genxs <<"\n" << flush;
 			return 0;
    	}	
    }

}
//Iterative deepening depth-first search
// <costo, generados, goal?>
tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  costoPadre, int  bound, int history){
		int ruleid;
	    ruleid_iterator_t posible_moves;
	    state_t child;
        bool goal;
        //unsigned childCount;
        int history_hijo;
        int f;
        //childCount = 0;
        nodos_gen++;
        //Caso Base
        f = costoPadre + k*pdb_h(&state);
        if (f > bound) return make_tuple(0, -f, false);
        if (is_goal(&state)){
            return (make_tuple(costoPadre, costoPadre, true));
        }
	    

        //expansion and recursion
        int t = INT_MAX;
        init_fwd_iter(&posible_moves, &state);
        while ( (ruleid = next_ruleid(&posible_moves)) >= 0 ) {
            if ( fwd_rule_valid_for_history(history,ruleid) == 0 ) continue;
	        apply_fwd_rule(ruleid, &state, &child);
            history_hijo = next_fwd_history(history,ruleid);
	        //++childCount;
	        tuple<unsigned,unsigned,bool> px = bounded_id_dfs(child, costoPadre+1, bound, history_hijo);
            if ((get<2>(px))  == 1) return make_tuple(get<0>(px), f,true);  
            t=min(t,-static_cast<int>(get<1>(px)));

        }
        return make_tuple(0, -t,false);
}