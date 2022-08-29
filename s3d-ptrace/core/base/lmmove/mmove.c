#include "mmove.h"


Matrix3  kmatrix_read( FILE *f )
{
 Matrix3 k;
 
 fscanf( f, "%lf", &k.r1.x ); 
 fscanf( f, "%lf", &k.r1.y );
 fscanf( f, "%lf", &k.r1.z );
 fscanf( f, "%lf", &k.r2.x );
 fscanf( f, "%lf", &k.r2.y );
 fscanf( f, "%lf", &k.r2.z );
 fscanf( f, "%lf", &k.r3.x );
 fscanf( f, "%lf", &k.r3.y );
 fscanf( f, "%lf", &k.r3.z );

 return k;
}


Matrix4 rtmatrix_read( FILE *f )
{
 Matrix4 m = m4_ident();
 
 fscanf( f, "%lf", &m.r1.x ); 
 fscanf( f, "%lf", &m.r1.y );
 fscanf( f, "%lf", &m.r1.z );
 fscanf( f, "%lf", &m.r2.x );
 fscanf( f, "%lf", &m.r2.y );
 fscanf( f, "%lf", &m.r2.z );
 fscanf( f, "%lf", &m.r3.x );
 fscanf( f, "%lf", &m.r3.y );
 fscanf( f, "%lf", &m.r3.z );
 fscanf( f, "\n" );
 fscanf( f, "%lf", &m.r1.w );
 fscanf( f, "%lf", &m.r2.w );
 fscanf( f, "%lf", &m.r3.w );
 fscanf( f, "\n" );
 
 return m;
}


void mmove_read( CamData  *cd, char *kfname, char *rtfname )
{
 int i;
 FILE *kfile, *rtfile;
 
 kfile = fopen( kfname, "r" );
 rtfile = fopen( rtfname, "r" );
 cd->k = kmatrix_read( kfile );
 while( !feof(rtfile) ){
   fscanf( rtfile, "Frame %d\n", &i );
   cd->rt[i] = rtmatrix_read(rtfile);
 }
 cd->nframes = i+1;
 
 fclose( kfile );
 fclose( rtfile );
}


void mmove_view( View *view, CamData *cd, int frame )
{ 
 Real mx, my, l, r, b, t;
 Matrix4 vi, v;
 Vector3 p, v1, v2, v3; 
 v = view->V = cd->rt[frame];
 
 vi = m4_ident();
 vi.r1.x = v.r1.x ; vi.r1.y = v.r2.x ; vi.r1.z = v.r3.x ; 
 vi.r2.x = v.r1.y ; vi.r2.y = v.r2.y ; vi.r2.z = v.r3.y ; 
 vi.r3.x = v.r1.z ; vi.r3.y = v.r2.z ; vi.r3.z = v.r3.z ;
 p = v3_make( v.r1.w, v.r2.w, v.r3.w );
 v1 = v3_make( vi.r1.x, vi.r1.y, vi.r1.z );
 v2 = v3_make( vi.r2.x, vi.r2.y, vi.r2.z );
 v3 = v3_make( vi.r3.x, vi.r3.y, vi.r3.z );
 vi.r1.w = -v3_dot( v1 , p ); 
 vi.r2.w = -v3_dot( v2 , p ); 
 vi.r3.w = -v3_dot( v3 , p ); 
 view->Vinv = vi;
 
 mx =  cd->k.r1.x / cd->near;
 my =  cd->k.r2.y / cd->near; 
 l =  -cd->k.r1.z/mx;
 r =  (cd->w - cd->k.r1.z)/mx;
 b =  -cd->k.r2.z/my;
 t =  (cd->h - cd->k.r2.z )/my; 
 frustrum( view, l, b, r, t, cd->near, cd->far );
}


