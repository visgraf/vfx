#include "featureslist.h"

void features_list_insert( FeaturesList *fl, Features *f )
{
 fl->nframes++;
 if( fl->head != NULL ){
   fl->tail->next = f;
   f->prev = fl->tail;
   fl->tail = f;
 }
 else
   fl->head = fl->tail = f;
}
