CC = gcc
CFLAGS =  -Wall -Werror -Wextra
SOURCE = s21_grep.c
EXECUTABLE = s21_grep
START = ./
RUILES = chmod 777 
BASH = t_grep.sh
DELETE = rm -rf
LEAKS = leaks -atExit -- $(START)$(EXECUTABLE) -n byte.txt
CPPCHECK = cppcheck --enable=all --suppress=missingIncludeSystem ../../src/
PYTHON = python3 ../../materials/linters/cpplint.py --extensions=c  *.c *.h
all:
	@$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE)

test:
	$(RUILES) $(BASH)
	$(START)$(BASH)
	@$(DELETE) *.log

.PHONY: clean
clean:
	@$(DELETE) *.o $(EXECUTABLE)

.PHONY: rebuild
rebuild:
	@clear
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
