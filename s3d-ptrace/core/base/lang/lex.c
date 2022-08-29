/*   lex.c - lexical analizer */

#include <stdio.h>
#include <ctype.h>

#ifdef __APPLE__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "defs.h"
#include "lang.h"
#include "symbol.h"
#include "y.tab.h"

extern int yyparse();

int lineno = 0;

static FILE *fin = NULL;
static int c;


int yylex()
{
  Symbol *s;

  while ((c = getc(fin)) == ' ' || c == '\t')
    ;
  if (c == EOF)
    return 0;
  if (c == '.' || isdigit(c)) { double d;
    ungetc(c, fin);
    if (fscanf(fin, "%lf", &d) > 0)
      yylval.dval = d;
    return NUMBER;
  }
  if (c == '"') { char sbuf[320], *p;
    for (p = sbuf; (c = getc(fin)) != '"'; *p++ = c) {
      if (c == '\\') 
	c = ((c = getc(fin)) == '\n')? ' ' : c;
      if (c == '\n' || c == EOF) {
	fprintf(stderr,"missing quote, sbuf\n");
	break;
      }
      if (p >= sbuf + sizeof(sbuf) - 1) {
	fprintf(stderr,"sbuffer overflow\n");
	break;
      }
    }
    *p = '\0';
    yylval.sval = (char *)emalloc(strlen(sbuf) + 1);
    strcpy(yylval.sval, sbuf);
    return STRING;
  }
  if (isalpha(c)) { char sbuf[1024], *p = sbuf;
    do {
      if (p >= sbuf + sizeof(sbuf) - 1) {
	*p = '\0';
	fprintf(stderr,"name too long %s\n", sbuf);
      }
      *p++ = c;
    } while ((c = getc(fin)) != EOF && (isalnum(c) || c == '_' ));
    ungetc(c, fin);
    *p = '\0';
    if ((s = sym_lookup(sbuf)) == (Symbol *)0)
      s = sym_install(sbuf, NAME, NULL);
    if (s->token == CLASS)
      yylval.fval = s->func;
    else
      yylval.sval = s->name;
    return s->token;
  }
  if (c == '%')
    while ((c = getc(fin)) != '\n')
      ;
  switch (c) {
  case '\\': if ((c = getc(fin)) != '\n')
               return c;
             else
               return yylex();
  case '\n': lineno++; return yylex();
  default:   return c;
  }
}

void lang_file(FILE *fd)
{
  fin = fd;
}

int lang_parse(void)
{
  if (fin == NULL) lang_file(stdin);
  yyparse();
  return (lang_ptree() == NULL);
}

Val include_parse(int pass, Pval *pl)
{
  Val v = {0, {0}};
  if (pass == T_EXEC) { 
    char *fname = NULL;
    FILE *fd = NULL;
    Node *root = NULL;
    Pval *p = pl;
    while (p != NULL) {
      if (p->name && strcmp(p->name,"fname") == 0 && p->val.type == V_STR) {
	fname = p->val.u.v;
      }
      p = p->next;
    }
    if (fname == NULL) 
      error("(lang include - missing filename)");
    fd = dfopen(fname, "r");
    lang_file(fd);
    root = lang_ptree();
    if (lang_parse() != 0)
      error("(lang include - parse)");
    v = lang_eval();
    lang_set_root(root);
    fclose(fd);
    lang_file(NULL);
  }
  return v;
}
