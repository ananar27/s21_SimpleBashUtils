CC_FLAGS = -Wall -Wextra -Werror

all: s21_grep

s21_grep: s21_grep.c s21_grep.h functions.c functions.h
	gcc $(CC_FLAGS) s21_grep.c functions.c -o s21_grep

clean:
	rm -rf s21_grep
	rm -f cat/*.o

rebuild: clean all

