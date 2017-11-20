#include "mem.h"
#include <stdio.h>
#include <assert.h>
#include "test_utils.h"

static const size_t AB_T = sizeof(ab_t);
static const size_t AB_SIZE = 128;

static void free_test_0_a(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple\n");

    init();
    mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    mem_alloc(AB_SIZE);
    void* ab4 = mem_alloc(AB_SIZE);
    mem_alloc(AB_SIZE);
    mem_free(ab4);
    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 3);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->next == td.fbtab[2]);
    assert(td.fbtab[2]->next == NULL);
    assert(td.fbtab[0]->size == AB_SIZE);
    assert(td.fbtab[1]->size == AB_SIZE);
    assert(td.fbtab[2]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 5 - AB_T);

    assert_ok(test);
}

static void free_test_0_b(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple\n");

    init();
    mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    mem_alloc(AB_SIZE);
    void* ab4 = mem_alloc(AB_SIZE);
    mem_alloc(AB_SIZE);
    mem_free(ab2);
    mem_free(ab4);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 3);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->next == td.fbtab[2]);
    assert(td.fbtab[2]->next == NULL);
    assert(td.fbtab[0]->size == AB_SIZE);
    assert(td.fbtab[1]->size == AB_SIZE);
    assert(td.fbtab[2]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 5 - AB_T);

    assert_ok(test);
}


static void free_test_1(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple avec libération en tête\n"
        "Merge à gauche, puis merge à gauche et à droite simultanée, Soit la séquence :\n"
        "[A][A][A][-]\n"
        "[A][-][A][-]\n"
        "[----][A][-]\n"
        "[----------]\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE);
    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab2 - AB_T);
    assert(td.fbtab[0]->size == AB_SIZE);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 3 - AB_T);
    assert(td.fbtab[1]->next == NULL);

    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == AB_SIZE * 2 + AB_T);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 3 - AB_T);
    assert(td.fbtab[1]->next == NULL);

    mem_free(ab3);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void free_test_2(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple\n"
                "Merge à droite, puis merge à gauche et à droite simultanée, Soit la séquence :\n"
                "[A][A][A][-]\n"
                "[-][A][A][-]\n"
                "[-][A][----]\n"
                "[----------]\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE);
    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == AB_SIZE);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 3 - AB_T);
    assert(td.fbtab[1]->next == NULL);

    mem_free(ab3);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == AB_SIZE);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1] == ab3 - AB_T);
    assert(td.fbtab[1]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 2 - AB_T);
    assert(td.fbtab[1]->next == NULL);

    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void free_test_3(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple, vérification de la cohérence des adresses des blocks\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE);
    void* ab4 = mem_alloc(AB_SIZE);
    void* ab5 = mem_alloc(AB_SIZE);
    mem_free(ab3);
    mem_free(ab4);
    void* ab6 = mem_alloc(AB_SIZE);
    void* ab7 = mem_alloc(AB_SIZE * 10);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 2);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab4 - AB_T);
    assert(td.fbtab[0]->size == AB_SIZE);
    assert(td.fbtab[0]->next == td.fbtab[1]);
    assert(td.fbtab[1] == ab7 + AB_SIZE * 10);
    assert(td.fbtab[1]->size == td.MEM_SIZE - (AB_SIZE + AB_T) * 5 - (AB_SIZE * 10 + AB_T) - AB_T);
    assert(td.fbtab[1]->next == NULL);

    mem_free(ab7);
    mem_free(ab5);
    test_utils_update_data();
    test_utils_print_data(test);
    assert(td.size == 1);

    mem_free(ab6);
    mem_free(ab2);
    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);

    assert_ok(test);
}

static void free_test_4_a(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple de gauche à droite\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE);
    void* ab4 = mem_alloc(AB_SIZE);
    mem_free(ab1);
    mem_free(ab2);
    mem_free(ab3);
    mem_free(ab4);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void free_test_4_b(char * test) {
    if (td.display) fprintf(stderr, "Test de libération multiple de droite à gauche\n");

    init();
    void* ab1 = mem_alloc(AB_SIZE);
    void* ab2 = mem_alloc(AB_SIZE);
    void* ab3 = mem_alloc(AB_SIZE);
    void* ab4 = mem_alloc(AB_SIZE);
    mem_free(ab4);
    mem_free(ab3);
    mem_free(ab2);
    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void free_test_5(char * test) {
    if (td.display) fprintf(stderr, "Test de libération suite à un remplissage intégral du segment mémoire\n");

    init();
    void* ab1 = mem_alloc(td.MEM_SIZE - AB_T);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 0);
    assert(td.list == NULL);

    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}

static void free_test_5_a(char * test) {
    if (td.display) fprintf(stderr, "Test de libération suite à un remplissage intégral du segment mémoire par deux allocations\n"
                "Libération en queue puis en tête\n");

    init();
    void* ab1 = mem_alloc(td.MEM_SIZE/2 - AB_T);
    void* ab2 = mem_alloc(td.MEM_SIZE/2 - AB_T);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 0);
    assert(td.list == NULL);

    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab2 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE/2 - AB_T);
    assert(td.fbtab[0]->next == NULL);

    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);


}

static void free_test_5_b(char * test) {
    if (td.display) fprintf(stderr, "Test de libération suite à un remplissage intégral du segment mémoire par deux allocations\n"
        "Libération en tête puis en queue\n");

    init();
    void* ab1 = mem_alloc(td.MEM_SIZE/2 - AB_T);
    void* ab2 = mem_alloc(td.MEM_SIZE/2 - AB_T);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 0);
    assert(td.list == NULL);

    mem_free(ab1);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE/2 - AB_T);
    assert(td.fbtab[0]->next == NULL);

    mem_free(ab2);
    test_utils_update_data();
    test_utils_print_data(test);

    assert(td.size == 1);
    assert(td.list == td.fbtab[0]);
    assert(td.fbtab[0] == ab1 - AB_T);
    assert(td.fbtab[0]->size == td.MEM_SIZE - AB_T);
    assert(td.fbtab[0]->next == NULL);

    assert_ok(test);
}


int main(int argc, char *argv[]) {

    test_utils_set_test_name("Test de libération");
    test_utils_set_display(true);

    test_utils_start();

    free_test_0_a("free_test_0_a");
    free_test_0_b("free_test_0_b");
    free_test_1("free_test_1");
    free_test_2("free_test_2");
    free_test_3("free_test_3");
    free_test_4_a("free_test_4_a");
    free_test_4_b("free_test_4_b");
    free_test_5("free_test_5");
    free_test_5_a("free_test_5_a");
    free_test_5_b("free_test_5_b");

    test_utils_end();

    return 0;
}