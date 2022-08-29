#include "featureslist.h"

static Features *fcurrent;


void features_list_begin_frag( FeaturesList *fl )
{
 fcurrent = fl->head;
} 


FeaturesList *features_list_get_next_frag( FeaturesList *fl, int nframes, double distancy )
{
 Features *faux, *fbase;
 FeaturesList *flout;
 
 flout = features_list_alloc();
 fbase = fcurrent;
 while( (fcurrent != NULL) && ((nframes >= 0) ||
    features_distancy(fbase,faux) < distancy) ){
      faux = features_clone( fcurrent );
      features_reduce_status( fbase, faux );
      features_list_insert( flout, faux ); 
      fcurrent = fcurrent->next;
      nframes--;
 } 
 /* it's important to scale adjust in seq_adjust_cameras */
 if( fcurrent != NULL  )
   fcurrent = fcurrent->prev; 
 return flout;
}


int features_list_end_frag( void )
{
 if( fcurrent == NULL  )
   return TRUE;
 else
   return FALSE;  
}
