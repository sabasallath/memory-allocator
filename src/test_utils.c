#include "test_utils.h"
#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "mem.h"


void fflush_all() {
    fflush(stdout);
    fflush(stderr);
}

void init() {
    td.MEM_SIZE = get_memory_size();
    mem_init(get_memory_adr(), td.MEM_SIZE);
}

void test_utils_set_test_name(char * string) {
    td.testname = string;
}

void test_utils_set_display(bool boolean) {
    td.display = boolean;
}

void test_utils_start() {
    fflush_all();
    if (td.display) fprintf(stderr, "==========================================\n%s\n==========================================\n\n", td.testname);
}

void test_utils_end() {
    if (td.display) {
        fprintf(stderr, "=== %s ===\n"
                "ALL TEST PASSED\n"
                "==========================================\n", td.testname);
        fflush_all();
    }
}

void test_utils_init() {
    for(int i=0; i <= MAX_FB; i++) {
        td.fbtab[i] = NULL;
    }
}

void test_utils_update_data() {
    test_utils_init();

    struct fb* list = test_utils_get_list();
    td.list = list;

    struct fb* current = list;
    int i;
    for( i = 0; current != NULL; i++) {
        current = current->next;
    }

    td.size = i;

    current = list;
    for( i = 0; current != NULL; i++) {
        td.fbtab[i] = current;
        current = current->next;
    }
}

void test_utils_print_data(char *test) {
    if (td.display) {
        fflush_all();
        fprintf(stderr, "|| Test %s\n", test);
        fprintf(stderr, " | Number of freeblocks = %d\n", td.size);
        fprintf(stderr, " | List head = %lu\n", (uintptr_t) (void *) td.list);
        for (int i = 0; i < td.size; i++) {
            fprintf(stderr, "   | [%d] current * = %lu, size = %lu, next * = %lu\n",
                    i, (uintptr_t) (void *) td.fbtab[i], td.fbtab[i]->size,
                    (uintptr_t) (void *) td.fbtab[i]->next);
        }
        fflush_all();
    }
}

 void assert_ok(char *test) {
     if (td.display) {
         fflush_all();
         fprintf(stderr, "|| Test %s All asserts passed\n\n", test);
         fflush_all();
     }
 }