#include "mem.h"
#include <stdio.h>
#include <assert.h>
#include "test_utils.h"
#include <stdint.h>

static const size_t AB_T = sizeof(ab_t);
static const size_t AB_SIZE = 128;
static const size_t AB_SIZE_SMALLER = 32;
static const size_t AB_SIZE_TOO_SMALL = sizeof(fb_t) - sizeof(ab_t);
static const size_t AB_SIZE_MIN = sizeof(fb_t);
static const size_t AB_SIZE_NON_ALIGN = sizeof(fb_t) + 1;

static void alloc_test_0_a(char * test) {
    if (td.display) fprintf(stderr, "Test d'une allocation simple de taille suffisante pour contenir un block libre (%lu)\n", AB_SIZE);

    init();
    mem_alloc(AB_SIZE);

    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->size == td.MEM_SIZE - (AB_SIZE + 2 * AB_T));
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void alloc_test_0_b(char * test) {
    if (td.display) fprintf(stderr, "Test d'une allocation simple de taille insuffisante pour contenir un block libre (%lu)\n", AB_SIZE_TOO_SMALL);

    init();
    mem_alloc(AB_SIZE_TOO_SMALL);

    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->size == td.MEM_SIZE - (AB_SIZE_MIN + 2 * AB_T));
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void alloc_test_0_c(char * test) {
    if (td.display) fprintf(stderr, "Test d'une allocation simple de taille non alignée (%lu, alignement %d)\n", AB_SIZE_NON_ALIGN, ALIGN);

    init();
    mem_alloc(AB_SIZE_NON_ALIGN);

    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    uintptr_t add_alignement = ALIGN - (AB_SIZE_NON_ALIGN % ALIGN);

    assert(td.fbtab[0]->size == td.MEM_SIZE - (AB_SIZE_NON_ALIGN + add_alignement + 2 * AB_T));
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void alloc_test_1(char * test) {
    if (td.display) fprintf(stderr, "Test d'allocations multiples et libération du deuxième block alloué\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE);
    mem_free(ab2);
    void* ab4 = mem_alloc(AB_SIZE_SMALLER);

    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(ab2 == ab1 + AB_T + AB_SIZE);
    assert(ab3 == ab2 + AB_T + AB_SIZE);
    assert(ab4 == ab2);

    assert(td.fbtab[0] == (fbp_t) ((char *)((abp_t) ab2 - 1) + AB_SIZE_SMALLER + AB_T));
    assert(td.fbtab[1] ==  (fbp_t) ((char *)((abp_t) ab3 - 1) + AB_SIZE + AB_T));

    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->size == AB_SIZE - AB_SIZE_SMALLER - AB_T);
    assert(td.fbtab[1]->size == td.MEM_SIZE - ((AB_SIZE + AB_T)*3 + AB_T));

    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->next == NULL);

    assert_ok(test);
}

static void alloc_test_2_a(char * test) {
    if (td.display) fprintf(stderr, "Test d'allocations multiples avec insertion en tête sans remplissage du block libre\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    mem_free(ab1);
    void* ab3 = mem_alloc(AB_SIZE_SMALLER);

    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(td.list == td.fbtab[0]);
    assert(ab2 == ab1 + AB_T + AB_SIZE);
    assert(ab3 == ab1);

    assert(td.fbtab[0] == (fbp_t) ((char *)((abp_t) ab1 - 1) + AB_SIZE_SMALLER + AB_T));
    assert(td.fbtab[1] ==  (fbp_t) ((char *)((abp_t) ab2 - 1) + AB_SIZE + AB_T));

    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->size == AB_SIZE - AB_SIZE_SMALLER - AB_T);
    assert(td.fbtab[1]->size == td.MEM_SIZE - (2 * (AB_SIZE + AB_T) + AB_T));

    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->next == NULL);

    assert_ok(test);
}

static void alloc_test_2_b(char * test) {
    if (td.display) fprintf(stderr, "Test d'allocations multiples avec insertion en tête avec remplissage du block libre\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    mem_free(ab1);
    void* ab3 = mem_alloc(AB_SIZE);

    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(ab2 == ab1 + AB_T + AB_SIZE);
    assert(ab3 == ab1);

    assert(td.fbtab[0] == (fbp_t) ((char *)((abp_t) ab2 - 1) + AB_SIZE + AB_T));

    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->size == td.MEM_SIZE - (2 * (AB_SIZE + AB_T) + AB_T));

    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

int main(int argc, char *argv[]) {

    test_utils_set_test_name("Test d'allocation");
    test_utils_set_display(true);

    test_utils_start();
    alloc_test_0_a("alloc_test_0_a");
    alloc_test_0_b("alloc_test_0_b");
    alloc_test_0_c("alloc_test_0_c");
    alloc_test_1("alloc_test_1");
    alloc_test_2_a("alloc_test_2_a");
    alloc_test_2_b("alloc_test_2_b");
    test_utils_end();

    return 0;
}
