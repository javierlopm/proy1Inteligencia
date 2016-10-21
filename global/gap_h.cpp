#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

unsigned gap_h(state_t state, int numvar){
    int c;

    //Calculo de heuristica
    c=0;
    for(int x = 0; x < numvar-1; x++){
        if (abs(state.vars[x]-state.vars[x+1]) > 1) c++;

    }
    return c;
}

int main(int argc, char **argv) {
    state_t state, child1, child2;
    int ruleid,status;
    ruleid_iterator_t rule_iterator;
    char first_state[511];
    unsigned v0,v1,v2;

    cout << "Introduce un estado> ";
    cin.getline(first_state,511);

    // cout << first_state << flush;

    status = read_state(first_state,&state);

    if( status <= 0 ) {
        cout << "Error: estado inválido.\n";
        return 0; 
    }
    
    init_fwd_iter(&rule_iterator,&state);
    ruleid = next_ruleid(&rule_iterator);
    apply_fwd_rule(ruleid,&state,&child1);
    ruleid = next_ruleid(&rule_iterator);
    apply_fwd_rule(ruleid,&state,&child2);

    // Padre
    cout << "Estado inicial: " << first_state << "\n";
    v0= gap_h(state, 28);
    cout << "Valor heurística_gap inicial: " << v0<<"\n";

    // Hijo 1
    printf("\nThe child1 is: ");
    sprint_state(first_state,511, &child1);
    cout << first_state << "\n";
    v1 = gap_h(child1, 28);
    cout << "Valor heurística_gap hijo1: " << v1<<"\n";

    // Hijo 2
    printf("\nThe child2 is: ");
    print_state(stdout, &child2);
    printf("\n");
    v2 = gap_h(child2, 28);
    cout << "Valor heurística_gap  hijo2: " << v2 <<"\n";

    return 0;
}

