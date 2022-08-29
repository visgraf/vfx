#include "ray.h"

static int n_inode = 0;

int inode_size(Inode *l) 
{
  Inode *i; int n = 0;
  for (i = l; i != NULL; i = i->next) {
    n++;
  }
  return n;
}

Inode *inode_alloc(Real t, Vector3 n, int enter)
{
  Inode *i = (Inode *) malloc(sizeof(Inode));
  i->t = t;
  i->n = n;
  i->enter = enter;
  i->next = (Inode *)0;
  n_inode++;
  return i;
}

void inode_free(Inode *l)
{
  Inode *i, *p;

  i = l;
  do {
    p = i; i = i->next;
    free(p);
    n_inode--;
  } while (i != NULL);
}

int inode_count()
{
  return n_inode;
}

