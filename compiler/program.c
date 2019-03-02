#include <stdio.h>
#include <stdlib.h>
#include "program.h"

static Node *code[100];
static int sentence = 0;
static int pos = 0;
static Token *tokens;

int get_potition()  {
    return pos;
}
void set_tokens(Token *empty_tokens) {
    tokens = empty_tokens;
}

Node *ast(int index) {
    return code[index];
}

void new_code(Node *node) {
  code[sentence] = node;
  sentence++;
  code[sentence] = NULL;
}

Node* assign() {
  Node* lhs = expr();

  if (tokens[pos].ty == ';')
    return lhs;

  if (tokens[pos].ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }

  return lhs;
}

void program() {
    Node *lhs = assign();

    new_code(lhs);
    pos++;

    if (tokens[pos].ty != TK_EOF)
    {
        program();
    }
}

Node* expr() {
  Node* lhs = mul();
  if (tokens[pos].ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }

  if (tokens[pos].ty == '-') {
    pos++;
    return new_node('-', lhs, expr());
  }

  return lhs;
}

Node* mul() {
  Node* lhs = term();
  if (tokens[pos].ty == '*') {
    pos++;
    return new_node('*', lhs, mul());
  }

  if (tokens[pos].ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs;
}

Node* term() {
  if (tokens[pos].ty == TK_NUM)
    return new_node_num(tokens[pos++].val);

  if (tokens[pos].ty == TK_IDENT)
    return new_node_idnet((char)tokens[pos++].val);

  if (tokens[pos].ty == '(') {
    pos++;
    Node *node = expr();

    if (tokens[pos].ty != ')') {
      fprintf(stderr, "開きカッコに対する閉じカッコがありません：%s", tokens[pos].input);
      exit(1);
    }

    pos++;
    return node;
  }

  fprintf(stderr, "数値でも開きカッコでもないトークンです：%s", tokens[pos].input);
  exit(1);
}
