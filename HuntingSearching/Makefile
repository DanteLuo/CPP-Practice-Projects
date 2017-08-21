CXX = g++
FLAGS =-std=c++11 -O3 -ggdb -Wall -Wextra

all:main.o utils.o HuntingMap.o
	$(CXX) $(FLAGS) -o Hunting main.o utils.o HuntingMap.o
%.o:%.cpp
	$(CXX) $(FLAGS) -o $@ -c $<
clean:
	rm *.o Hunting
