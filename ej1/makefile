WARNINGS = -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -Wshadow -Wnull-dereference -Wfloat-equal -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wformat=2 -Wcast-align -Wstrict-overflow=5 -Wunsafe-loop-optimizations -Wuseless-cast -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wzero-as-null-pointer-constant -Wmissing-declarations -Weffc++ -Wstack-protector

VALGRIND = valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes -s
all:
	@g++ -o run src/* example/main.cpp
	@./run
	@rm run