CC=g++
FLAGS=-fshort-enums

default: lexer-test

lexer-test: lexer.cpp lexer.h
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG lexer.cpp -o lexer
