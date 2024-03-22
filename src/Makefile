CC = gcc
TESTFLAGS = -lcheck
CFLAGS = -Wall -Wextra -Werror
GCOVFLAGS = -fprofile-arcs -ftest-coverage

ifeq ($(OS),Linux)
	FLAGS = -lcheck -lm -lpthread -lrt -lsubunit -fprofile-arcs -ftest-coverage
	OPEN_CMD = xdg-open
else
	FLAGS = -lcheck -lm -lpthread -fprofile-arcs -ftest-coverage
	OPEN = open
endif

all: s21_decimal.a

s21_decimal.a : functions/*.c
	$(CC) $(CFLAGS) -g -std=c11 -c functions/*.c 
	ar rcs s21_decimal.a *.o
	rm -rf *.o

test: functions/*.c tests/*.c
	$(CC) $(CFLAGS) functions/*.c tests/*.c -o test $(TESTFLAGS)
	./test

gcov_report:
	$(CC) $(GCOVFLAGS) $(CFLAGS) functions/*.c tests/*.c -o test $(TESTFLAGS)
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	$(OPEN) ./report/index.html
	rm -rf gcovreport gcovreport.info *.gcda *.gcno 

clang:
	clang-format -style=Google -n tests/*.c tests/*.h *.h functions/*.c

toclang:
	clang-format --style=Google -i tests/*.c tests/*.h *.h functions/*.c

rebuild: clean all

clean: 
	rm -rf *.o test *.a a.out *.gcno *.gcda test.dSYM *.info report
