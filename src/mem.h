#ifndef __MEM_H
#define __MEM_H
#include <stddef.h>

#define ALIGN 8


struct fb{
    size_t size;
    struct fb* next;
};

struct ab {
    size_t size;
};

typedef struct fb* fbp_t;
typedef struct fb fb_t;
typedef struct ab * abp_t;
typedef struct ab ab_t;

void mem_init(char* mem, size_t taille);
void* mem_alloc(size_t);
void mem_free(void*);
size_t mem_get_size(void *);

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free));

/* Choix de la strategie et strategies usuelles */
typedef struct fb* (mem_fit_function_t)(struct fb *, size_t);

void mem_fit(mem_fit_function_t*);
struct fb* mem_fit_first(struct fb*, size_t);
struct fb* mem_fit_best(struct fb*, size_t);
struct fb* mem_fit_worst(struct fb*, size_t);

/* Si vous avez le temps */
struct fb* mem_fit_best(struct fb*, size_t);
struct fb* mem_fit_worst(struct fb*, size_t);

/* Pour les tests */
struct fb * test_utils_get_list();
#endif
