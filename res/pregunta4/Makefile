# to make a PDB starting from foo.psvn  type the command:     make pdb ss=foo
# if $(absname).txt exists it will be used as the input (abstraction commands) to build the abstraction,
#   otherwise the user will enter the abstraction commands interactively.

CC = gcc
CXX = g++ -std=gnu++11
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --backward_moves --abstraction --fwd_history_len=0 --bwd_history_len=1

psvn2c_core.c:
	cp ../../src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../../src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../../src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../../bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

%.succ: %.c ../../global/succ.c
	$(CC) $(OPT) ../../global/succ.c -include $< -o $@
	rm -f $*.c

%.dist: %.c ../../global/dist.cpp
	$(CXX) $(OPT) ../../global/dist.cpp -include $< -o $@
	rm -f $*.c

%.distSummary: %.c ../../global/distSummary.cpp
	$(CXX) $(OPT) ../../global/distSummary.cpp -include $< -o $@
	rm -f $*.c

%.best_first_search: %.c ../../global/best_first_search.cpp
	$(CXX) $(OPT) ../../global/best_first_search.cpp -include $< -o $@
	rm -f $*.c

%.a_star_pdb: %.c ../../global/a_star_pdb.cpp
	$(CXX) $(OPT) ../../global/a_star_pdb.cpp -include $< -o $@
	rm -f $*.c

%.id_dfs: %.c ../../global/id_dfs.cpp
	$(CXX) $(OPT) ../../global/id_dfs.cpp -include $< -o $@
	rm -f $*.c

%.ida_gap: %.c ../../global/ida_gap.cpp
	$(CXX) $(OPT) ../../global/ida_gap.cpp -include $< -o $@

%.ida_man: %.c ../../global/ida_man.cpp
	$(CXX) $(OPT) ../../global/ida_man.cpp -include $< -o $@

%.ida_pdb: %.c ../../global/ida_pdb.cpp
	$(CXX) $(OPT) ../../global/ida_pdb.cpp -include $< -o $@


%.wida_pdb: %.c ../../global/wida_pdb.cpp
	$(CXX) $(OPT) ../../global/wida_pdb.cpp -include $< -o $@

abstractor:
	$(CXX) $(OPT) ../../src/abstractor.cpp ../../src/psvn.cpp -o $@

%.pdb: abstractor
	# @rm -f `basename $*`.abst `basename $*`.pdb `basename $*`.psvn
	./abstractor `dirname $*`.psvn `basename $*` < `basename $*`.txt
	make `basename $*`.distSummary
	echo Calculating `basename $*.pdb` ...
	@./`basename $*`.distSummary `basename $*`.pdb

%.dist_pdb: %.c ../../global/dist_pdb.cpp
	$(CXX) $(OPT) ../../global/dist_pdb.cpp -include $< -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary *.dist_pdb psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c abstractor *.dSYM *.o *~
