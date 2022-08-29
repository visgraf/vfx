#include "featureslist.h"


/* Constructors */

Features *features_alloc( int nfeatures, int frame, int *status )
{
 int i;
 Features *f;

 f = NEWSTRUCT( Features );
 f->nfeatures = nfeatures;
 f->next = f->prev =  NULL;
 f->frame = frame;
 f->p = NEWTARRAY( nfeatures, Vector3 );
 f->status = NEWTARRAY( nfeatures, int );
 if( status != NULL )
   for( i=0; i < nfeatures; i++ )
      f->status[i] = status[i];
 return f;
}


Features *features_clone( Features *f )
{
 int i;
 Features *fclone;

 fclone = NEWSTRUCT( Features );
 fclone->nfeatures = f->nfeatures;
 fclone->next = fclone->prev = NULL;
 fclone->frame = f->frame;
 fclone->p = NEWTARRAY( f->nfeatures, Vector3 );
 fclone->status = NEWTARRAY( f->nfeatures, int );
 for( i = 0; i < f->nfeatures; i++ ){
    fclone->p[i] = f->p[i];
    fclone->status[i] = f->status[i];
 }
 return fclone;
}


FeaturesList *features_list_alloc( void )
{
 FeaturesList *fl;

 fl = NEWSTRUCT( FeaturesList );
 fl->head = fl->tail = NULL;
 return fl;
}


/* Destructors */

void features_dispose( Features *f )
{
 efree( f->p );
 efree( f->status );
 efree( f );
}


void features_list_dispose( FeaturesList *fl )
{
 Features *aux1, *aux2;

 aux1 = fl->head;
 while( aux1 != NULL ){
   aux2 = aux1;
   aux1 = aux1->next;
   features_dispose(aux2);
 }
 efree( fl );
}
