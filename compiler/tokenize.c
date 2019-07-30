#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenize.h"

static int is_alnum(char c) {
  return ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') ||
         (c == '_');
}

Vector *tokenize(char *p) {
  Vector *tokens;
  Token *token;
  char *variable_str;
  char *head;
  int str_count = 0;

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
        p++;
        vec_push(tokens, token);
        continue;
    }

    if(isdigit(*p)) {
      token->ty = TK_NUM;
      token->input = p;
      token->val = strtol(p, &p, 10);
      vec_push(tokens, token);
      continue;
    }

    // 変数のトークナイズ
    str_count = 0;
    head = p;

    while ('a' <= *p && *p <= 'z')
    {
      str_count++;
      p++;
    }

    if (str_count > 0) {
      variable_str = (char *)calloc(str_count, sizeof(char));
      strncpy(variable_str, head, str_count);

      token->ty = TK_IDENT;
      token->input = head;
      token->variabale_name =  variable_str;
      vec_push(tokens, token);
      str_count = 0;
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