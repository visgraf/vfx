#include "ibl.h"


static Color hdrmap_value( HDRDome *d, Vector3 v );
static Vector3 change_ref( Vector3 v, HDRDome *d );

HDRDome *hdr_dome;


void hdrdome_init( void )
{
 hdr_dome = NULL;
}


HDRDome *hdrdome_alloc( Vector3 orig, Vector3 south, HDRImage *img )
{
 HDRDome *d = NEWSTRUCT( HDRDome );

 d->n = v3_unit( south );
 d->u = v3_unit( v3_sub( orig,
                 v3_scale( v3_dot( orig, d->n ), d->n )) );
 d->v = v3_cross( d->n, d->u );                    
 d->img = img;
 
 return d;
}


void hdrdome_free( HDRDome *d )
{
 hdrimg_free( d->img );
 free( d );
}

/*
Color hdrdome_value( HDRDome *d, Vector3 v )
{
 Real theta, phi;
 
 v = change_ref( v, d );
 theta = acos(v.z);
 phi = atan2(v.y,v.x); 
                             
 return hdrimg_getc( d->img, d->img->w - ROUND(((phi/PI) + 1)*d->img->w/2 ), 
                             ROUND((theta/PI)*d->img->h) );                                   
}*/


Color hdrdome_value( HDRDome *d, Vector3 v )
{
 Real theta, phi;
 
 v = change_ref( v, d );
 theta = acos(v.z);
 phi = atan2(v.y,v.x); 
                             
 return hdrimg_getc( d->img, ROUND((1 - (phi/PI))*d->img->w/2 ), 
                             ROUND((theta/PI)*d->img->h) );                                   
}


Vector3 change_ref( Vector3 v, HDRDome *d )
{
 Matrix4 m;
 
 m = m4_ident();
 m.r1.x = d->u.x;  m.r2.x = d->v.x;  m.r3.x = d->n.x; 
 m.r1.y = d->u.y;  m.r2.y = d->v.y;  m.r3.y = d->n.y; 
 m.r1.z = d->u.z;  m.r2.z = d->v.z;  m.r3.z = d->n.z; 
 
 return v3_m3mult( v, m );
} 




