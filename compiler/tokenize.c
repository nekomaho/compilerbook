#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenize.h"

static int is_alnum(char c) {
  return ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') ||
         (c == '_');
}

Vector *tokenize(char *p) {
  int i = 0;
  Vector *tokens;
  Token *token;

  tokens = new_vector();

  while(*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    token = malloc(sizeof(Token));

    if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6]))
    {
      token->ty = TK_RETURN;
      token->input = p;
      i++;
      p += 6;
      continue;
    }

    switch(*p) {
      case '!':
      case '>':
      case '<':
      case '+':
      case '-':
      case '*':
      case '(':
      case ')':
      case '/':
      case '=':
      case ';':
        token->ty = *p;
        token->input = p;
        i++;
        p++;
        vec_push(tokens, token);
        continue;
    }

    if(isdigit(*p)) {
      token->ty = TK_NUM;
      token->input = p;
      token->val = strtol(p, &p, 10);
      i++;
      vec_push(tokens, token);
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      token->ty = TK_IDENT;
      token->input = p;
      token->val =  *p;
      i++;
      p++;
      vec_push(tokens, token);
      continue;
    }

    fprintf(stderr, "トークナイズ出来ません: %s\n", p);
    exit(1);
  }

  token = malloc(sizeof(Token));
  token->ty = TK_EOF;
  token->input = p;
  vec_push(tokens, token);

  return tokens;
}