NAME=btc
CXX=c++ -std=c++98
CXXFLAGS=-Wall -Wextra -Werror
CPPFILES=${wildcard *.cpp}
OFILES=${CPPFILES:.cpp=.o}

all: $(NAME)

$(NAME): $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o  $(NAME)

clean:
	rm -f $(OFILES)
fclean:clean
	rm -f $(NAME)
re:fclean all