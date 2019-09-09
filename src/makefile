CC=g++
CFLAGS=-I.
DEPS = Alias.h BinaryExecution.h DestroyTerminal.h History.h InitialData.h Signal.h Trie.h

%.out: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rule: OSAssignment1.cpp
	$(CC) -o shell OSAssignment1.cpp
