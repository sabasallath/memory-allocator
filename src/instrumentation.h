#ifndef __INSTRUMENTATION_H__
#define __INSTRUMENTATION_H__

void instr_init(char* mem, size_t taille);

void instr_ask_alloc_size(size_t size);

void instr_ask_free_size (size_t size);

void instr_ask_list (fbp_t list);



#endif