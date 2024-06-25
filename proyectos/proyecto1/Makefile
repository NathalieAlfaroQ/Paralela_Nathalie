# NOTE: REMEMBER INSTALL GCC VALGRIND CLANG
all: compile_general execute_general
valgrind: compile_valgrind execute_valgrind
# NOTE: Do not use ThreadSanitizer in big programs or in production
ThreadSanitizer: compile_ThreadSanitizer execute_general
ThreadSanitizer-clang: compile_ThreadSanitizer_clang execute_general


# COMPILERS INSTANCES pthread
compile_general:
	g++ -o executeme main.cpp

compile_valgrind:
	gcc -pthread -g -o executeme calcPi_pthread04.c -lm

compile_ThreadSanitizer:
	gcc -pthread -fsanitize=thread -o executeme calcPi_pthread03.c -lm

compile_ThreadSanitizer_clang:
	clang -pthread -fsanitize=thread -o executeme pthread02.c


# EXECUTERS INSTANCES
execute_general:
	./executeme < prueba.txt

execute_valgrind:
	valgrind --tool=helgrind ./executeme $(ARGS)