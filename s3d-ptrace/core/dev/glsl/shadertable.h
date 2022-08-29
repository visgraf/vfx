#ifndef SYMBOL_H
#define SYMBOL_H
#include "string.h"
#include "shade.h"

/*	shadertable.h - symbol table defs	*/

typedef struct Item {
  struct Item *next; 
  struct Shader s;
} Item;


unsigned int st_lookup(char *name);
unsigned int st_install(Shader s, unsigned int id);
int st_apply(void (*f)(Item *sp));
int st_cleanup();

#endif
