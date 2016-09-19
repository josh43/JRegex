#use make --debug=v
# if you care about the size of the project it is currently
# cat ./*/*h | wc -l ====== 1928 lines
#directories to search for
VPATH = Lexer:Parser:NFAGenerator:TestVSGrep/Test2:Regex:DFAGenerator:Regex/TestPackage
CC = g++
FLAGS = -Wall -std=c++14

main : main.o Utility.o Regex.o
	$(CC) $(FLAGS) main.o Utility.o Regex.o -o main
main.o:  RegularLexer.h EZGraph.h EZTester.h RegexExamples.h DFAGenerator.h
	$(CC) $(FLAGS) -c main.cpp
Utility.o: Utility.h
	$(CC) $(FLAGS) -c Utility.cpp
Regex.o: Regex.h EpsilonNFAMatcher.h RegularParser.h NFAGenerator.h RegularLexer.h DFAGenerator.h
	$(CC) $(FLAGS) -c Regex/Regex.cpp
.PRECIOUS: %.o


clean:
	rm *.o main
