#include <stdio.h>
#include <stdlib.h>
#include "tokenize.h"
#include "vector.h"
#include "program.h"

static Vector *code;
static int sentence = 0;
static int pos = 0;
static Vector *tokens;

int get_position()  {
    return pos;
}
void set_tokens(Vector *analyzed_tokens) {
  code = new_vector();
  tokens = analyzed_tokens;
}

int number_of_ast(void) {
  return code->len;
}

Node *ast(int index) {
    return code->data[index];
}

void new_code(Node *node) {
  vec_push(code, node);
}

static Token *get_token_from_tokens(int pos) {
  return (Token *)(tokens->data)[pos];
}

Node* assign() {
  Token *token;

  Node* lhs = expr();

  token = get_token_from_tokens(pos);
  if (token->ty == ';')
    return lhs;

  if (token->ty == '=') {
    pos++;
    return new_node('=', lhs, assign());
  }

  return lhs;
}

void program() {
  Token *token;

  Node *lhs = assign();

  new_code(lhs);
  pos++;

  token = get_token_from_tokens(pos);
  if (token->ty != TK_EOF) {
    program();
  }
}

Node* expr() {
  Token *token;

  Node* lhs = mul();

  token = get_token_from_tokens(pos);
  if (token->ty == '+') {
    pos++;
    return new_node('+', lhs, expr());
  }

  if (token->ty == '-') {
    pos++;
    return new_node('-', lhs, expr());
  }

  return lhs;
}

Node* mul() {
  Token *token;

  Node* lhs = term();

  token = get_token_from_tokens(pos);
  if (token->ty == '*') {
    pos++;
    return new_node('*', lhs, mul());
  }

  if (token->ty == '/') {
    pos++;
    return new_node('/', lhs, mul());
  }

  return lhs;
}

Node* term() {
  Token *token;
  token = get_token_from_tokens(pos);

  if (token->ty == TK_NUM) {
    token = get_token_from_tokens(pos);
    pos++;
    return new_node_num(token->val);
  }

  if (token->ty == TK_IDENT) {
    token = get_token_from_tokens(pos);
    pos++;
    return new_node_idnet((char)token->val);
  }

  if (token->ty == '(') {
    pos++;
    Node *node = expr();

    token = get_token_from_tokens(pos);
    if (token->ty != ')') {
      fprintf(stderr, "開きカッコに対する閉じカッコがありません：%s", token->input);
      exit(1);
    }

    pos++;
    return node;
  }

  fprintf(stderr, "数値でも開きカッコでもないトークンです：%s", (char *)token->input);
  exit(1);
}
