parser: lex.yy.c parser.tab.c parser.tab.h
	g++ -std=c++11 parser.tab.c lex.yy.c -lfl -o parser

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexical.l parser.tab.h
	flex lexical.l
