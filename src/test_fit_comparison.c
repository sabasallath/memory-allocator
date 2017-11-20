#include "mem.h"
#include <stdio.h>
#include <assert.h>
#include "test_utils.h"
#include <stdint.h>

static const size_t AB_T = sizeof(ab_t);
static const size_t AB_1 = 160;
static const size_t AB_2 = 120;
static const size_t AB_3 = 96;
static const size_t AB_4 = 112;
static const size_t AB_5 = 56;
static const size_t AB_6 = 152;

static void fit_first_vs_best_1(char * test) {
    if (td.display) fprintf(stderr, "Illustration d'un cas avantageux pour la stratégie fit_first par rapport à fit_best\n");

    init();
    mem_fit(mem_fit_first);
    void* ab1 = mem_alloc(AB_1);
    mem_alloc(AB_1);
    void* ab2 = mem_alloc(AB_2);
    mem_alloc(AB_2);
    test_utils_update_data();
    mem_alloc(td.fbtab[td.size -1]->size - AB_T);
    mem_free(ab1);
    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    if (td.display) fprintf(stderr, "Tentative d'allocation de blocks de tailles successives %ld, %ld, %ld\n", AB_3, AB_4, AB_5);
    mem_alloc(AB_3);
    mem_alloc(AB_4);
    void* last_allocation = mem_alloc(AB_5);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(last_allocation != NULL);
    if (td.display) fprintf(stderr, "Tout les blocks ont pu être allouer grâce à stratégie fit_first\n");

    assert_ok(test);
}

static void fit_first_vs_best_2(char * test) {
    if (td.display) fprintf(stderr, "Illustration d'un cas désavantageux pour la stratégie fit_best par rapport à fit_first\n");

    init();
    mem_fit(mem_fit_best);
    void* ab1 = mem_alloc(AB_1);
    mem_alloc(AB_1);
    void* ab2 = mem_alloc(AB_2);
    mem_alloc(AB_2);
    test_utils_update_data();
    mem_alloc(td.fbtab[td.size -1]->size - AB_T);
    mem_free(ab1);
    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    if (td.display) fprintf(stderr, "Tentative d'allocation de blocks de tailles successives %ld, %ld, %ld\n", AB_3, AB_4, AB_5);
    mem_alloc(AB_3);
    mem_alloc(AB_4);
    void* last_allocation = mem_alloc(AB_5);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(last_allocation == NULL);
    if (td.display) fprintf(stderr, "Tout les blocks n'ont pas pu être allouer grâce à stratégie fit_best\n");

    assert_ok(test);
}

static void fit_best_vs_first_1(char * test) {
    if (td.display) fprintf(stderr, "Illustration d'un cas avantageux pour la stratégie fit_best par rapport à fit_first\n");

    init();
    mem_fit(mem_fit_best);
    void* ab1 = mem_alloc(AB_1);
    mem_alloc(AB_1);
    void* ab2 = mem_alloc(AB_2);
    mem_alloc(AB_2);
    test_utils_update_data();
    mem_alloc(td.fbtab[td.size -1]->size - AB_T);
    mem_free(ab1);
    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    if (td.display) fprintf(stderr, "Tentative d'allocation de blocks de tailles successives %ld, %ld\n", AB_3, AB_6);
    mem_alloc(AB_3);
    void* last_allocation = mem_alloc(AB_6);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(last_allocation != NULL);
    if (td.display) fprintf(stderr, "Tout les blocks ont pu être allouer grâce à stratégie fit_best\n");

    assert_ok(test);
}

static void fit_best_vs_first_2(char * test) {
    if (td.display) fprintf(stderr, "Illustration d'un cas désavantageux pour la stratégie fit_first par rapport à fit_best\n");

    init();
    mem_fit(mem_fit_first);
    void* ab1 = mem_alloc(AB_1);
    mem_alloc(AB_1);
    void* ab2 = mem_alloc(AB_2);
    mem_alloc(AB_2);
    test_utils_update_data();
    mem_alloc(td.fbtab[td.size -1]->size - AB_T);
    mem_free(ab1);
    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    if (td.display) fprintf(stderr, "Tentative d'allocation de blocks de tailles successives %ld, %ld\n", AB_3, AB_6);
    mem_alloc(AB_3);
    void* last_allocation = mem_alloc(AB_6);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(last_allocation == NULL);
    if (td.display) fprintf(stderr, "Tout les blocks n'ont pas pu être allouer grâce à stratégie fit_first\n");

    assert_ok(test);
}


int main(int argc, char *argv[]) {

    test_utils_set_test_name("Test d'illustration de cas avantageux d'une politique par rapport à une autre");
    test_utils_set_display(true);

    test_utils_start();
    fit_first_vs_best_1("fit_first_vs_best_1");
    fit_first_vs_best_2("fit_first_vs_best_2");
    fit_best_vs_first_1("fit_best_vs_first_1");
    fit_best_vs_first_2("fit_best_vs_first_2");
    test_utils_end();

    return 0;
}
