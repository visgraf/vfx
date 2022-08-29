#include "featureslist.h"

#define BOTH_TRACKED ((f1->status[i] == FEATURE_TRACKED) && (f2->status[i] == FEATURE_TRACKED))


int features_ncommon( Features *f1, Features *f2 )
{
 int i;
 int ncommon = 0;

 for( i=0; i < f1->nfeatures; i++ )
   if( BOTH_TRACKED )
     ncommon++;
 return ncommon;
}


Real features_distancy( Features *f1, Features *f2 )
{
 int i, ncommon = 0;
 Real d = 0;

 for( i=0; i < f1->nfeatures; i++ ){
    if( BOTH_TRACKED ){
       d += v3_norm( v3_sub( f1->p[i], f2->p[i] ) );
       ncommon++;  
    }   
 }
 
 return d/ncommon;
}


void features_reduce_status( Features *f1, Features *f2 )
{
 int i;
 int ncommon = 0;

 for( i=0; i < f1->nfeatures; i++ )
   if( !BOTH_TRACKED ){
     f1->status[i] = FEATURE_VOID;
     f2->status[i] = FEATURE_VOID;
   }
}


