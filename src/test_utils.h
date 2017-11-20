#ifndef SRC_TEST_UTILS_H
#define SRC_TEST_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#define MAX_FB 50

typedef struct {
    char * testname;
    bool display;
    size_t MEM_SIZE;
    struct fb* fbtab[MAX_FB];
    struct fb* list;
    int size;
} Testdata_t;

Testdata_t td;

void init();
void test_utils_set_test_name(char * string);
void test_utils_set_display(bool boolean);
void test_utils_start();
void test_utils_end();
extern struct fb * test_utils_get_list();
void test_utils_update_data();
void test_utils_print_data(char *test);
void assert_ok(char *test);

#endif //SRC_TEST_UTILS_H
