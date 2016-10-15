CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --no_abstraction --no_backwards_moves --history_len=2

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

%.generator: %.c ../../global/generator.cpp
	$(CXX) $(OPT) ../../global/generator.cpp -include $< -o $@
	rm -f $*.c

%.dfs_count: %.c ../../global/dfs_count.cpp
	$(CXX) $(OPT) ../../global/dfs_count.cpp -include $< -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary *.generator psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
