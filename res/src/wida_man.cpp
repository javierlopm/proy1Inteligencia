//Necesito cost, generated, time
#include <vector>
#include <iostream>
#include <tuple>
#include <time.h>
#include <csignal>
#include "manhattan.cpp"
#include <algorithm>

using namespace std;


void signalHandler( int signum )
{
    cout << "na, na, na, na\n" << flush;
   exit(signum);  

}
unsigned nodos_gen = 0;

float k = 1 + 0; //0 = manhattan

tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  d, int  bound, int history);

int main(int argc, char **argv) {
    clock_t t_ini, t_fin;
    double secs, genxs;

    int bound, status;
    float Epsilon;
    char first_state[511];
    state_t state;
    bool goal;

    signal(SIGALRM,signalHandler);

    // cout << "Introduce un estado> ";
    cin >> Epsilon;
    cin.ignore();
    cin.getline(first_state,511);
    k=1+Epsilon;

    t_ini = clock();
    status = read_state(first_state,&state);

    if( status <= 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }

//-------------------------------------------------------
    cout << "X, ida, manhattan, 15puzzle, " << "\""<< first_state << "\", " << flush ;

    bound=k*manhattan(&state,NUMVARS);
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

            int h0 = k*manhattan(&state,NUMVARS);
            //int costo = static_cast<int>(get<1>(p));
            cout << get<0>(p)<< ", " << h0 << ", "<< nodos_gen << ", " << secs << ", "<< genxs <<"\n" << flush;
            return 0;
        }   
    }

}
//Iterative deepening depth-first search
// <costo, generados, goal?>
tuple<unsigned,unsigned,bool> bounded_id_dfs(state_t state, int  costoActual, int  bound, int history){
        //cout << "d: " << costoActual << ", h=" << manhattan(&state,NUMVARS)<< endl;
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
        f = costoActual + k*manhattan(&state,NUMVARS);
        //cout << "\n" << manhattan(&state,NUMVARS) << "\n";
        if (f > bound) return make_tuple(0, -f, false);
        if (is_goal(&state)){
            return (make_tuple(costoActual, costoActual, true));
        }
        

        //expansion and recursion
        int t = INT_MAX;
        init_fwd_iter(&posible_moves, &state);
        while ( (ruleid = next_ruleid(&posible_moves)) >= 0 ) {
            if ( fwd_rule_valid_for_history(history,ruleid) == 0 ) continue;
            apply_fwd_rule(ruleid, &state, &child);
            history_hijo = next_fwd_history(history,ruleid);
            //++childCount;
            tuple<unsigned,unsigned,bool> px = bounded_id_dfs(child, costoActual+1, bound, history_hijo);
            if ((get<2>(px))  == 1) return make_tuple(get<0>(px), f,true);  
            t=min(t,-static_cast<int>(get<1>(px)));

        }
        return make_tuple(0, -t,false);
}

