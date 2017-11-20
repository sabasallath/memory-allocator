#include "mem.h"
#include <stdio.h>
#include <assert.h>
#include "test_utils.h"
#include <stdint.h>

static const size_t AB_T = sizeof(ab_t);
static const size_t AB_SIZE = 128;
static const size_t AB_SIZE_SMALLER = 32;
static const size_t AB_SIZE_MIN = sizeof(fb_t);

static void fit_worst_test_0(char * test) {
    if (td.display) fprintf(stderr, "Test de la stratégie mem_fit_worst avec allocation du bon freeblock\n");

    init();
    mem_fit(mem_fit_worst);
    void* ab1 = mem_alloc(AB_SIZE);
    mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE * 2);
    mem_alloc(AB_SIZE * 2);
    void* ab5 = mem_alloc(AB_SIZE / 2);
    mem_alloc(AB_SIZE / 2);
    void* ab7 = mem_alloc(AB_SIZE_SMALLER);
    mem_alloc(AB_SIZE_SMALLER);
    void* ab9 = mem_alloc(AB_SIZE);

    mem_free(ab1);
    mem_free(ab3);
    mem_free(ab5);
    mem_free(ab7);
    mem_free(ab9);
    void* ab10 = mem_alloc(AB_SIZE);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 5);
    assert(ab10 == ab9);

    mem_alloc(td.fbtab[td.size -1]->size - AB_T);
    void* ab12 = mem_alloc(AB_SIZE);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 4);
    assert(ab12 == ab3);

    assert_ok(test);
}

static void fit_best_test_0(char * test) {
    if (td.display) fprintf(stderr, "Test de la stratégie mem_fit_best avec allocation du bon freeblock\n");

    init();
    mem_fit(mem_fit_best);
    void* ab1 = mem_alloc(AB_SIZE * 2);
    mem_alloc(AB_SIZE * 2);
    void* ab3 = mem_alloc(AB_SIZE);
    mem_alloc(AB_SIZE);
    void* ab5 = mem_alloc(AB_SIZE / 2);
    mem_alloc(AB_SIZE / 2);
    void* ab7 = mem_alloc(AB_SIZE_SMALLER);
    void* ab8 = mem_alloc(AB_SIZE_SMALLER);

    mem_free(ab1);
    mem_free(ab3);
    mem_free(ab5);
    mem_free(ab7);
    void* ab9 = mem_alloc(AB_SIZE);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 4);
    assert(ab9 == ab3);

    void* ab10 = mem_alloc(AB_SIZE_SMALLER);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 3);
    assert(ab10 == ab7);

    void* ab11 = mem_alloc(AB_SIZE_SMALLER);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 3);
    assert(ab11 == ab5);

    void* ab12 = mem_alloc(AB_SIZE_MIN);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(ab12 == ab5 + AB_SIZE_SMALLER + AB_T);

    void* ab13 = mem_alloc(AB_SIZE * 4);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(ab13 == ab8 + AB_SIZE_SMALLER + AB_T);

    assert_ok(test);
}

int main(int argc, char *argv[]) {

    test_utils_set_test_name("Test des différentes politiques");
    test_utils_set_display(true);

    test_utils_start();
    fit_worst_test_0("fit_worst_test_0");
    fit_best_test_0("fit_best_test_0");
    test_utils_end();

    return 0;
}
