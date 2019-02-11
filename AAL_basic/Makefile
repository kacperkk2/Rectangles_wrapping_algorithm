CXX = g++
CXXFLAGS  = -g -Wall -c
RM = rm -f
NAME = basic

$(NAME): main.o
	$(CXX) -o $(NAME) main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

clean:
	$(RM) *.o $(NAME)
