NAME=btc
CXX=c++ -std=c++98
CXXFLAGS=-Wall -Wextra -Werror
CPPFILES=${wildcard *.cpp}
OFILES=${CPPFILES:.cpp=.o}

all: $(NAME)

$(NAME): $(OFILES)
	$(CXX) $(CXXFLAGS) $(O_FILES) -o  $(NAME)

clean:
	rm -f $(O_FILES)
fclean:clean
	rm -f $(NAME)
re:fclean all