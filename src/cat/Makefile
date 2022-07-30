CC = gcc
CFLAGS = -Wall -Werror -Wextra
SOURCE = s21_cat.c
EXECUTABLE = s21_cat
BASH = t_cat.sh
START = ./
RUILES = chmod 777 
DELETE = rm -rf
LEAKS = leaks -atExit -- $(START)$(EXECUTABLE) -n byte.txt
CPPCHECK = cppcheck --enable=all --suppress=missingIncludeSystem ../../src/
PYTHON = python3 ../../materials/linters/cpplint.py --extensions=c  *.c *.h

all:
	@$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE)

test:
	@$(RUILES)$(BASH)
	@$(START)$(BASH)
	@$(DELETE) log.txt

.PHONY: clean
clean:
	@$(DELETE) $(EXECUTABLE) *.o

.PHONY: rebuild
rebuild:
	@make clean
	@make all
	
.PHONY: check
check:
	@$(LEAKS)
	@$(CPPCHECK)
	@$(PYTHON)

.PHONY: style
style:
	@$(PYTHON)
