#ifndef ZBUF_H
#define ZBUF_H

void zbuf_init(int w, int h);
void zbuf_clear(Real val);
int  zbuf_store(Vector4 p);
Real zbuf_peek(Vector4 p);

#endif
