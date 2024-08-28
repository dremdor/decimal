LIBRARY 			= s21_decimal.a
FUNC_FILES 			= decimal_functions/s21_*.c
OBJ					= s21_*.o
FUNC_TEST			= tests/*.check
OBJ_TEST			= test_s21_*.o
OUT_TEST			= decimal_test
REPORT_FILE 		= report/index.html

UNAME_S := $(shell uname -s)

CC			= gcc
CFLAGS		= -c -std=c11 -Wall -Werror -Wextra
L_FLAGS		= -std=c11 -Wall -Werror -Wextra
COVER_FLAG  = --coverage
LIB_FLAGS 	= ar rcs

ifeq ($(UNAME_S), Linux)
	OS_SPECIFIC_FLAGS 				= -lcheck -lsubunit -lm
	OS_SPECIFIC_GCOV_REPORT 		= lcov_branch_coverage=1
	OS_SPECIFIC_GCOV_REPORT_HTML 	= genhtml_branch_coverage=1
else ifeq ($(UNAME_S), Darwin)
	OS_SPECIFIC_FLAGS 				= -lcheck
    OS_SPECIFIC_GCOV_REPORT 		= branch_coverage=1
    OS_SPECIFIC_GCOV_REPORT_HTML 	= branch_coverage=1
endif

ifeq ($(shell grep -i microsoft /proc/version),)
	OPEN_REPORT:= open
else
    OPEN_REPORT:= wslview
endif

all: s21_decimal.a

s21_decimal.a: build.o
	$(LIB_FLAGS) $(LIBRARY) $(OBJ)
	rm -fr $(OBJ)

test: clean test_file
	./$(OUT_TEST)
	$(MAKE) gcov_report
	-rm -rf *.gc* *.info $(OUT_TEST) $(OBJ) $(LIBRARY)
	$(OPEN_REPORT) $(REPORT_FILE)

leaks_test: test_file
	leaks -atExit -- ./$(OUT_TEST)
	-rm -rf *.gc* *.info $(OUT_TEST) $(OBJ_TEST) $(OBJ) $(LIBRARY)

valgrind_test: test_file
	valgrind --leak-check=yes ./$(OUT_TEST)
	-rm -rf *.gc* *.info $(OUT_TEST) $(OBJ_TEST) $(OBJ) $(LIBRARY)

test_file: s21_decimal.a_for_test
	checkmk clean_mode=1 $(FUNC_TEST) > test.c
	$(CC) $(L_FLAGS) $(COVER_FLAG) test.c -L. $(LIBRARY) $(OS_SPECIFIC_FLAGS) -o $(OUT_TEST)

s21_decimal.a_for_test: build_object_for_test.o
	$(LIB_FLAGS) $(LIBRARY) $(OBJ)
	rm -fr $(OBJ)

build.o:
	$(CC) $(CFLAGS) $(FUNC_FILES)

build_object_for_test.o:
	$(CC) $(CFLAGS) $(COVER_FLAG) $(FUNC_FILES)

gcov_report:
	lcov -t "test" -o decimal.info -c -d . --rc $(OS_SPECIFIC_GCOV_REPORT)
	genhtml -o report decimal.info --rc $(OS_SPECIFIC_GCOV_REPORT_HTML)

build_o: $(FUNC_FILES)
	$(CC) $(CFLAGS) $(FUNC_FILES)

clang_format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(FUNC_FILES) $(FUNC_TEST) s21_decimal.h
	rm .clang-format

rebuild: clean all

clean:
	rm -rf $(OBJ) $(OUT_TEST) $(OBJ_TEST) $(LIBRARY) $(OUT_TEST) *.gc* report/ *.info *.out test.c
