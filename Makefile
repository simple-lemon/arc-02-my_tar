CC = gcc
FLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address 
SRC = my_tar* c/my_string.c c/linked_list.c
TARGET = my_tar

$(TARGET): $(SRC)
	$(CC) $(FLAGS) -o $@ $^

.PHONY: fclean

fclean:
	@rm -r $(TARGET)
