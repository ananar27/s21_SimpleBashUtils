CC_FLAGS = -Wall -Wextra -Werror

all: s21_cat

s21_cat: s21_cat.c s21_cat.h
	gcc $(CC_FLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf s21_cat
	rm -f cat/*.o

rebuild: clean all