CC=g++
FLAGS=-fshort-enums

default: birthstone
	chmod +x ../bin/birthstone

debug: birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) -g -DBS_DEBUG token.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/bs-debug

birthstone: birthstone.cpp parser.h lexer.h symbol.h token_stream.h token.h token.cpp parser.cpp lexer.cpp saved_token_stream.cpp
	$(CC) $(FLAGS) token.cpp saved_token_stream.cpp lexer.cpp parser.cpp birthstone.cpp -o ../bin/birthstone

lexer-test: lexer.cpp lexer.h token.h token.cpp
	$(CC) $(FLAGS) -DBS_LEXER_DEBUG token.cpp lexer.cpp -o lexer

clean:
	rm -v ../bin/*.o birthstone