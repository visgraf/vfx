/*	shadertable.c - symbol lookup	*/

#include <stdio.h>
#include "defs.h"
#include "shadertable.h"

static Item *stlist = (Item *)0;

/*:
**	lookup symbol
*/
unsigned int st_lookup(char *s)
{
  Item *sp;
  for (sp = stlist; sp != (Item *)0; sp = sp->next) {
    if (strcmp(sp->s.name, s) == 0)
      return sp->s.id;
  }
  return 0;
}

/*:
**	install symbol
*/
unsigned int st_install(Shader s, unsigned int id)
{
  Item *sp = (Item *) emalloc(sizeof(Item));
  s.id = id;
  sp->s = s;
  sp->next = stlist;
  stlist = sp;
  return stlist->s.id;
}

/*:
**	lookup symbol
*/
int st_apply(void (*f)(Item *sp))
{
  int k; Item *sp;
  for (sp = stlist, k=0; sp != (Item *)0; sp = sp->next, k++) 
    (*f)(sp);
  return k;
}

/*:
**      cleanup symbol
*/
int st_cleanup()
{
  Item *q, *p = stlist;
  int k = 0;
  while (p != NULL) {
    q = p;  p = p->next;
    efree(q); k++;
  }
  stlist = NULL;
  return k;
}

