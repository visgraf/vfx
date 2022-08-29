#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "shade.h"
#include "glshaders.h"


Shader make_shader(char *name, const char *vert, const char *frag)
{
  Shader s;
  char *vert_prog, *frag_prog;
  asprintf(&vert_prog, "%s\n%s", render_context, vert);
  asprintf(&frag_prog, "%s\n%s\n%s\n%s\n%s", render_context, light_support, light_sources, illum_models, frag);
  s.name = name;
  s.vertp = vert_prog;
  s.fragp = frag_prog;
  s.id = -1;
  return s;
}

Shader lookup_shader(char *name)
{
  if (strcmp(name,"phong_plastic") == 0) {
    return make_shader("phong_plastic", phong_vertp, phong_matte_fragp);
  } else if (strcmp(name,"phong_map") == 0) {
    return make_shader("phong_map", phong_vertp, phong_map_fragp);
  } else if (strcmp(name,"phong_proc") == 0) {
    return make_shader("phong_proc", phong_vertp, phong_proc_fragp);
  } else { 
    fprintf(stderr, "(Shader) %s not defined\n", name);
    return make_shader("phong_matte", phong_vertp, phong_matte_fragp);
  }
}

Val shaderprog_parse(int pass, Pval *pl)
{
  Val v = {0, {0}};
  if (pass == T_EXEC) { 
    Pval *p = pl;
    char *s;
    while (p != NULL) {
      if (p->name && strcmp(p->name,"file") == 0 && p->val.type == V_STR) {
	s = p->val.u.v; 
	v.type = V_SHADER_FILE;
	v.u.v = s;
	return v;
      } else if (p->name && strcmp(p->name,"name") == 0 && p->val.type == V_STR) {
	s = p->val.u.v;
	v.type = V_SHADER_DEF;
	v.u.v = s;
	return v;
      }
      p = p->next;
    }
  }
  return v;
}

char *readfile(const char *name, const char *suffix) 
{
  char *fname = emalloc(strlen(name) + strlen(suffix) + 1);
  strcpy(fname, name);
  strcat(fname, suffix);
  FILE *fp = dfopen(fname, "r");
  efree(fname);
  fseek(fp, 0, SEEK_END);
  size_t n = ftell(fp);
  char *buf = (char *) emalloc(n+1);
  if (!buf) 
    return NULL;
  fseek(fp, 0, SEEK_SET);
  fread(buf, n, 1, fp);
  fclose(fp);
  buf[n] = '\0';
  return buf;
}

Shader read_shaderfiles(char *name)
{
  const char *vertp, *fragp;
  vertp = readfile(name, ".vert");
  if (vertp == NULL) 
    vertp = phong_vertp;
  fragp = readfile(name, ".frag");
  if (fragp ==NULL)
    fragp = phong_matte_fragp;
  return make_shader(name, vertp, fragp);
}
