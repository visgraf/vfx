#include <stdio.h>
#include "geom.h"


void main( int argc, char **argv )
{
 FILE *f;
 int i, j;
 double m[3][3], x, y , z;
 float a, b, sx, sy, r;
 Vector3 v1, v2, v3, u, v, A, B, C, D, n, origin, south;
 
 f = fopen( "r", "r" );
 fscanf( f, "%lf%lf%lf", &x, &y, &z );
 fscanf( f, "%lf%lf%lf", &x, &y, &z );
 south = v3_make( x, y, z);
 fscanf( f, "%lf%lf%lf", &x, &y, &z );
 origin = v3_make( -x, -y, -z);
 fclose( f );
 
 printf( "orig = { %lf, %lf, %lf },\n", origin.x, origin.y, origin.z );
 printf( "south = { %lf, %lf, %lf },\n\n", south.x, south.y, south.z );
 
 f = fopen( argv[1], "r" );
 for( i = 0; i < 3; i++ )
   for( j = 0; j < 3; j++ )
     fscanf( f, "%lf", &m[i][j] );
 fclose( f );
   
 v1 = v3_make( m[0][0], m[0][1], m[0][2] ); 
 v2 = v3_make( m[1][0], m[1][1], m[1][2] );
 v3 = v3_make( m[2][0], m[2][1], m[2][2] );

 
 u = v3_sub( v2, v1 );
 v = v3_sub( v3, v1 );
 printf( "origin:\n" );
 scanf( "%f%f", &a, &b );
 printf( "sx:\n" );
 scanf( "%f", &sx );
 printf( "sy:\n" );
 scanf( "%f", &sy );
  
 A = v3_add( v1, v3_add( v3_scale( a, u ), v3_scale( b, v ) ) );
 B = v3_add( A, v3_scale( sx, u ) );
 C = v3_add( A, v3_scale( sy, v ) );
 D = v3_add( A, v3_add( v3_scale( sx, u ), v3_scale( sy, v ) ) );
 
 printf( "shape = trilist { {{%lf, %lf, %lf},  {%lf, %lf, %lf}, {%lf, %lf, %lf}}, \
                            {{%lf, %lf, %lf},  {%lf, %lf, %lf}, {%lf, %lf, %lf}}  }\n\n",        
          A.x, A.y, A.z, B.x, B.y, B.z, C.x, C.y, C.z,
          C.x, C.y, C.z, B.x, B.y, B.z, D.x, D.y, D.z );
          
 for( ;; ){
    printf( "Sphere radius:\n" );
    scanf( "%f", &r );
    r *= v3_norm(u);
    printf( "Sphere center:\n" );
    scanf( "%f%f", &a, &b ); 
    n = v3_scale( r, v3_unit( v3_cross( u, v ) ) );
    C = v3_add( v1, v3_add( n, v3_add( v3_scale( a, u ), v3_scale( b, v ) ) ) );
    
    printf( "shape = sphere{ center = {%lf, %lf, %lf}, radius = %lf}\n\n",
            C.x, C.y, C.z, r ); 
 }
}



