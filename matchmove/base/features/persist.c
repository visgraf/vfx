#include "featureslist.h"


FeaturesList *features_list_read( FILE *fin )
{
 int i, nfeatures, frame;
 Real x, y;
 Features *f;
 FeaturesList *fl;

 fscanf( fin, "Features per frame = %i\n", &nfeatures );
 fl = features_list_alloc();

 while( !feof( fin ) ){
   fscanf( fin, "Frame %i\n", &frame );
   f = features_alloc( nfeatures, frame, NULL );
   for( i = 0; i < nfeatures; i++ )
      fscanf( fin, "%lf %lf %i\n", &(f->p[i].x), &(f->p[i].y), &f->status[i] );
   features_list_insert( fl, f );
 }
 return fl;
}

void features_list_write( FILE *fout, FeaturesList *fl )
{
 int i, nfeatures;
 Features *f;

  if( fl->head != NULL ){
    nfeatures = fl->head->nfeatures;
    fprintf( fout, "Features per frame = %i\n", nfeatures  );
    for( f = fl->head; f != NULL; f = f->next ){
        fprintf( fout, "Frame %i\n", f->frame );
        for( i=0; i < nfeatures; i++ )
          fprintf( fout, "%lf %lf %i\n", f->p[i].x, f->p[i].y, f->status[i] );
   }
 }
}
