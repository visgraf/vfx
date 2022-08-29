#include "mmove.h"


CamData *cam_data_alloc( int ncams, int w, int h, Real near, Real far )
{
 CamData *cd;
 
 cd = NEWSTRUCT( CamData ); 
 cd->w = w;
 cd->h = h;
 cd->near = near;
 cd->far = far;
 cd->rt = NEWTARRAY( ncams, Matrix4 );
 return cd;
}


void cam_data_free( CamData *cd )
{
 free(cd->rt);
 free(cd);
}
