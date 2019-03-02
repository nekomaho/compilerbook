#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenize.h"

static Token *tokens;

/* FIXME 後で内部で確保出来るように変更する */
void allocate_tokens(Token *empty_tokens) {
    tokens = empty_tokens;
}

void tokenize(char *p) {
  int i = 0;
  while(*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    switch(*p) {
      case '+':
      case '-':
      case '*':
      case '(':
      case ')':
      case '/':
      case '=':
      case ';':
        tokens[i].ty = *p;
        tokens[i].input = p;
        i++;
        p++;
        continue;
    }

    if(isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      tokens[i].ty = TK_IDENT;
      tokens[i].input = p;
      tokens[i].val =  *p;
      i++;
      p++;
      continue;
    }

    fprintf(stderr, "トークナイズ出来ません: %s\n", p);
    exit(1);
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}