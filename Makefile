NAME =		AbstractVM

OBJ_DIR =	./bin/
SRC_DIR =	./src/
INC_DIR =	./inc/

SRC =	main.cpp\
		TypesFabric.cpp

SRC_ENDING = .cpp

OBJ = $(addprefix $(OBJ_DIR), $(SRC:$(SRC_ENDING)=.o))

FLAG = -Wextra -Werror -Wall --std=c++17

COMPILER = g++


.PHONY : all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_DIR) $(INC_DIR) $(OBJ)
	$(COMPILER) $(FLAG) -I$(INC_DIR) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(INC_DIR):
	mkdir $(INC_DIR)

$(OBJ_DIR)%.o: %$(SRC_ENDING)
	$(COMPILER) $(FLAG) -c $< -o $@ -I $(INC_DIR)

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)

re: fclean all

vpath %$(SRC_ENDING) $(SRC_DIR)