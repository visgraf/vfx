#ifndef MMOVE_H
#define MMOVE_H

#include "scene.h"
#include "view.h"
#include "image.h"

typedef struct Matrix3 {
  Vector3 r1, r2, r3;
} Matrix3;


typedef struct CamData{
  Real w, h;
  Real near, far;
  int nframes;
  Matrix3 k;
  Matrix4 *rt;
} CamData;


Matrix3  kmatrix_read( FILE *f );
Matrix4 rtmatrix_read( FILE *f );
void mmove_read( CamData  *cd, char *kfname, char *rtfname );
void mmove_view( View *view, CamData *cd, int frame );
CamData *cam_data_alloc( int ncams, int w, int h, Real near, Real far );
void cam_data_free( CamData *cd );


#endif
