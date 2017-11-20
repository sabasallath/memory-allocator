#include "common.h"
#include "mem.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "instrumentation.h"

// CONSTANTES
static const size_t AB_MIN_SIZE = sizeof(fb_t);
static const size_t AB_T = sizeof(ab_t);

//FONCTIONS UTILES
size_t minimum_value_or_align(size_t size);

//VARIABLE GLOBALES
static const char *MEM;
static size_t TOTAL_SIZE;
static fbp_t list = NULL;
static mem_fit_function_t* strategie = NULL;
static fbp_t PREVIOUS_FB = NULL;

fbp_t mem_fit_first(fbp_t list, size_t size) {

    PREVIOUS_FB = NULL;
    fbp_t currentfb = list;

    while (currentfb != NULL && (currentfb->size < size && currentfb->next != NULL)) {
        PREVIOUS_FB = currentfb;
        currentfb = currentfb->next;
    }

    if (currentfb != NULL && currentfb->size >= size) {
        return currentfb;
    } else {
        return NULL;
    }

}

struct fb* mem_fit_worst(struct fb* list, size_t size) {

    PREVIOUS_FB = NULL;
    fbp_t currentfb = list;
    size_t max_size = 0;
    fbp_t max_fb = NULL;

    if (currentfb != NULL) {
        if (currentfb->size >= size) {
            max_size = currentfb->size;
            max_fb = currentfb;
        }

        while (currentfb->next != NULL) {
            if (currentfb->next->size >= size && currentfb->next->size > max_size) {
                PREVIOUS_FB = currentfb;
                max_fb = currentfb->next;
                max_size = currentfb->next->size;
            }
            currentfb = currentfb->next;
        }
    }

    return max_fb;
}

struct fb* mem_fit_best(struct fb* list, size_t size) {

    PREVIOUS_FB = NULL;
    fbp_t currentfb = list;
    size_t best_size = SIZE_MAX;
    fbp_t best_fb = NULL;

    if (currentfb != NULL) {
        if (currentfb->size == size) {
            return currentfb;
        } else if (currentfb->size > size) {
            best_size = currentfb->size;
            best_fb = currentfb;
        }

        while (currentfb->next != NULL) {
            if (currentfb->next->size == size) {
                PREVIOUS_FB = currentfb;
                return currentfb->next;
            } else if (currentfb->next->size > size && currentfb->next->size < best_size) {
                PREVIOUS_FB = currentfb;
                best_fb = currentfb->next;
                best_size = currentfb->next->size;
            }
            currentfb = currentfb->next;
        }
    }

    return best_fb;
}

void mem_fit(mem_fit_function_t* function) {
    strategie = function;
}


void mem_init(char* mem, size_t taille) {
    fbp_t first = (fbp_t) mem;
    MEM = mem;
    TOTAL_SIZE = taille;
    first->size = taille - AB_T;
    first->next = NULL;
    mem_fit(mem_fit_first);
    list = first;

    instr_init(mem, taille);
}

void* mem_alloc(size_t size) {


    instr_ask_alloc_size(size);
    size = minimum_value_or_align(size);

    fbp_t fb;
    if ((fb = (*strategie) (list, size)) == NULL) {
        return NULL;
    } else {
        size_t freeSpaceRemaining = fb->size - size;

        if (freeSpaceRemaining <= AB_MIN_SIZE) {
            size = fb->size;
            if (PREVIOUS_FB != NULL) {
                PREVIOUS_FB->next = fb->next;
            } else {
                list = fb->next;
            }
        } else {
            fbp_t newfb = (fbp_t) ((abp_t) (((char *) fb) + size) + 1);
            newfb->size = freeSpaceRemaining - AB_T;
            if (PREVIOUS_FB != NULL) {
                PREVIOUS_FB->next = newfb;
                newfb->next = fb->next;
            } else {
                list = newfb;
                newfb->next = fb->next;
            }
        }

        abp_t ab = (abp_t) fb;
        ab->size = size;
        ab = ab + 1;

        instr_ask_list(list);

        return ab;
    }
}


size_t minimum_value_or_align(size_t size) {
    if (size < AB_MIN_SIZE) {
        size = AB_MIN_SIZE;
    } else if (size % ALIGN != 0){
        size = size + (ALIGN - (size % ALIGN));
    }
    return size;
}


void mem_free(void* zone){

    abp_t currentAb = ((abp_t) zone) - 1;
    instr_ask_free_size(currentAb->size);

    fbp_t newfb = (fbp_t) currentAb;
    fbp_t previous = NULL;
    fbp_t next = list;

    bool left_merge = false;
    bool right_merge = false;
    bool has_previous = false;

    if (list != NULL) {
        if (newfb < list) {  // libération en tête
            if ((fbp_t)((char *) newfb + (currentAb->size + AB_T)) == next) right_merge = true;

        } else {
            // libération après la tête
            has_previous = true;
            while (next != NULL && next < newfb) {
                // S'éxécute au moins une fois, post condition : previous != NULL
                previous = next;
                next = next->next;
            }
            if (previous != NULL && ((fbp_t) ((char *) previous + (previous->size + AB_T)) == newfb)) {
                left_merge = true;
            }
            if (next == NULL) {
            } else if ((fbp_t) ((char *) newfb + (currentAb->size + AB_T)) == next) {
                 right_merge = true;
            }
        }
    }

    if (left_merge & right_merge) {
        newfb = previous;
        newfb->size = next->size + currentAb->size + previous->size + 2 * AB_T;
        newfb->next = next->next;
    } else if (left_merge) {
        newfb = previous;
        newfb->next = next;
        newfb->size = previous->size + currentAb->size + AB_T;
    } else if (right_merge) {
        if (has_previous) {
            previous->next = newfb;
        } else {
            list = newfb;
        }
        newfb->size = next->size + currentAb->size + AB_T;
        newfb->next = next->next;
    } else {
        if (has_previous) {
            previous->next = newfb;
        } else {
            list = newfb;
        }
        newfb->next = next;
        newfb->size = currentAb->size;
    }

    instr_ask_list(list);
}

size_t mem_get_size(void* zone) {
    fbp_t currentfb = list;
    abp_t currentab;
    char * mem = (char *) MEM;

    while (mem < (char *) zone) {
        if ((char *) currentfb == mem) {
            currentfb =  (fbp_t) mem;
            mem = mem + currentfb->size + AB_T;
            currentfb = currentfb->next;
        } else {
            currentab = (abp_t) mem;
            mem = mem + currentab->size + AB_T;
        }
    }

    if (mem == (char *) zone) {
        if ((char *) currentfb != mem) {
            currentab = (abp_t) mem;
            return currentab->size;
        }
    }

    return 0;
}

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free)){
    fbp_t currentfb = list;
    abp_t currentab;
    char * mem = (char *) MEM;

    while (mem < (MEM + TOTAL_SIZE)) {
        if ((char *) currentfb == mem) {
            currentfb =  (fbp_t) mem;
            print(mem, currentfb->size, 1);
            mem = mem + currentfb->size + AB_T;
            currentfb = currentfb->next;
        } else {
            currentab = (abp_t) mem;
            print(mem, currentab->size, 0);
            mem = mem + currentab->size + AB_T;
        }
    }
}

/* Debug function */
fbp_t test_utils_get_list() {
    return list;
}