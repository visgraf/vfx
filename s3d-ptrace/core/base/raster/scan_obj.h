#include "geom.h"
#include "poly.h"

typedef struct Edge {
  int n;
  Vector4 p, i;
  double t, d;
} Edge;

static int first_vertex(Vector4 v[], int n);
static Edge *edge(Vector4 p0, Vector4 p1, char c, Edge *e);
static Edge *increment(Edge *e);
