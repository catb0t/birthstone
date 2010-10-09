CC=g++
FLAGS=-fshort-enums

default: birthstone
	chmod +x ../bin/birthstone

debug: birthstone.cpp parser.h lexer.h symbol.h parser.cpp lexer.cpp
	$(CC) -g -DBS_DEBUG lexer.cpp parser.cpp birthstone.cpp -o ../bin/bs-debug

birthstone: birthstone.o parser.o lexer.o 
	$(CC) $(FLAGS) lexer.o parser.o birthstone.o   -o ../bin/birthstone

birthstone.o: birthstone.cpp parser.h lexer.h
	$(CC) $(FLAGS) -c birthstone.cpp -o birthstone.o

parser.o: parser.cpp parser.h lexer.h symbol.h
	$(CC) $(FLAGS) -c parser.cpp -o parser.o

lexer.o: lexer.cpp lexer.h symbol.h
	$(CC) $(FLAGS) -c lexer.cpp -o lexer.o

lexer-test: lexer.cpp lexer.h
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG lexer.cpp -o lexer

clean:
	rm -v ./*.o birthstone bs