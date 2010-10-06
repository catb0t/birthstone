CC=g++
FLAGS=-fshort-enums

default: birthstone
	(cd ..; ln -s birthstone bs)

birthstone: birthstone.o parser.o
	$(CC) $(FLAGS) birthstone.o parser.o -o ../bin/birthstone

birthstone.o: birthstone.cpp parser.o
	$(CC) $(FLAGS) -c birthstone.cpp -o birthstone.o

parser.o: parser.h parser.cpp lexer.h
	$(CC) $(FLAGS) -c parser.cpp -o parser.o

lexer.o: lexer.cpp lexer.h
	$(CC) $(FLAGS) -c lexer.cpp -o lexer.o

lexer-test: lexer.cpp lexer.h
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG lexer.cpp -o lexer
