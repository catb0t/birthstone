CC=g++
FLAGS=-fshort-enums

default: birthstone
	chmod +x ../bin/birthstone

debug: birthstone.cpp parser.h lexer.h symbol.h parser.cpp lexer.cpp
	$(CC) -g -DBS_DEBUG lexer.cpp parser.cpp birthstone.cpp -o ../bin/bs-debug

birthstone: birthstone.cpp parser.h lexer.h symbol.h parser.cpp lexer.cpp
	$(CC) $(FLAGS) lexer.cpp parser.cpp birthstone.cpp -o ../bin/birthstone

lexer-test: lexer.cpp lexer.h
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG lexer.cpp -o lexer

clean:
	rm -v ../bin/*.o birthstone