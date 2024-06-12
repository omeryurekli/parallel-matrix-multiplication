FLAGS = -Wall -Werror -std=gnu99 -pthread
CC = gcc
TARGET = matrix_mult

$(TARGET): matrix_mult.c main.c
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
