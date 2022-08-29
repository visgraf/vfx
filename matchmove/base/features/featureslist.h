#ifndef FEATURES_H
#define FEATURES_H

#include "geom.h"

#define FEATURE_TRACKED   0
#define FEATURE_VOID      -1

typedef struct Features{
   struct Features *next, *prev;
   int nfeatures;
   int frame;
   int *status;
   Vector3 *p;
}Features;

typedef struct FeaturesList{
   int nframes;
   Features *head, *tail;
}FeaturesList;


/* constructors & destructors */

Features *features_alloc( int nfeatures, int frame, int *status );
Features *features_clone( Features *f );
FeaturesList *features_list_alloc( void );

void features_dispose( Features *f );
void features_list_dispose( FeaturesList *fl );

/* insert */

void features_list_insert( FeaturesList *fl, Features *f );

/* persistence */

FeaturesList *features_list_read( FILE *fin );
void features_list_write( FILE *fout, FeaturesList *fl );

/* operations */

void features_reduce_status( Features *f1, Features *f2 );
Real features_distancy( Features *f1, Features *f2 );
//void features_list_write_homolog( FILE *fout, Features *fl, Features *f2 );
//FeaturesList *features_list_sel_sequence( Real d, int ncommon, Features *base );

/* fragment */

void features_list_begin_frag( FeaturesList *fl );
FeaturesList *features_list_get_next_frag( FeaturesList *fl, int nframes, double distancy );
/*int features_list_end_frag( FeaturesList *fl );*/
int features_list_end_frag( void );

#endif

