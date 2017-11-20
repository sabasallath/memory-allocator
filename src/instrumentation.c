#include "mem.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

size_t INSTR_ASK_MEM;
size_t INSTR_SIZE_MAX;
char* INSTR_HEAP_HEAD;
struct timeval INSTR_T_INIT;
struct timeval INSTR_T_CURRENT;
    

long long int to_usec(struct timeval t) {
    return t.tv_sec*1000000LL + t.tv_usec;
}

fbp_t find_last_fb(fbp_t list) {
    fbp_t currentfb = list;

    while (currentfb != NULL && currentfb->next != NULL) {
        currentfb = currentfb->next;
    }

    return currentfb;
}

void instr_init(char* mem, size_t taille) {
	INSTR_ASK_MEM = 0;
	INSTR_SIZE_MAX = 0;
	INSTR_HEAP_HEAD = mem;
	gettimeofday(&INSTR_T_INIT, 0);

}

void instr_ask_alloc_size(size_t size) {
	INSTR_ASK_MEM += size;
	gettimeofday(&INSTR_T_CURRENT, 0);

}

void instr_ask_free_size (size_t size) {
	INSTR_ASK_MEM -= size;
	gettimeofday(&INSTR_T_CURRENT, 0);
}

void instr_ask_list(fbp_t list) {
	long long int t_exec = to_usec(INSTR_T_CURRENT) - to_usec(INSTR_T_INIT);
	fbp_t last_fb = find_last_fb(list);

	int heap_size;
	
	if (last_fb != NULL) {
		heap_size = (int) (((char *) last_fb) - ((char *) INSTR_HEAP_HEAD));
	} else {
		//!\ OU MAX !!! cas ignorer pour l'instant
		heap_size = 0;
	}

	fprintf(stderr, "%lld, %zd, %d,\n", t_exec, INSTR_ASK_MEM, heap_size);
}