#include <stdio.h>
#include <stdlib.h>
#include "tokenize.h"
#include "vector.h"
#include "program.h"

static Vector *code;
static int sentence = 0;
static int stack_count = 0;
static int pos = 0;
static Vector *tokens;

int get_position()  {
    return pos;
}
void set_tokens(Vector *analyzed_tokens) {
  code = new_vector();
  tokens = analyzed_tokens;
}

int max_stack_count() {
  return stack_count;
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

/*
  expr = equality
*/
Node* expr() {
  Node* lhs = equality();

  return lhs;
}

/*
  equality = relational ("==" relational | "!=" relational)*
*/
Node* equality() {
  Token *token;

  Node* lhs = relational();

  token = get_token_from_tokens(pos);
  if (token->ty == '=') {
    pos++;
    token = get_token_from_tokens(pos);
    if (token->ty == '=') {
      pos++;
      return new_node(ND_EQ, lhs, relational());
    } else {
      pos--;
    }
  }

  if (token->ty == '!') {
    pos++;
    token = get_token_from_tokens(pos);
    if (token->ty == '=') {
      pos++;
      return new_node(ND_NEQ, lhs, relational());
    }
  }

  return lhs;
}

/*
  relational = add ("<" add | "<=" add | ">" add | ">=" add)*
*/
Node* relational() {
  Token *token;

  Node* lhs = add();

  token = get_token_from_tokens(pos);
  if (token->ty == '<') {
    pos++;
    token = get_token_from_tokens(pos);
    if (token->ty == '=') {
      pos++;
      return new_node(ND_LTE, lhs, add());
    } else {
      return new_node(ND_LT, lhs, add());
    }
  }

  if (token->ty == '>') {
    pos++;
    token = get_token_from_tokens(pos);
    if (token->ty == '=') {
      pos++;
      return new_node(ND_RTE, lhs, add());
    } else {
      return new_node(ND_RT, lhs, add());
    }
  }

  return lhs;
}

/*
  add = mul ("+" expr | "-" expr)*
*/
Node* add() {
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

/*
  mul = unary ("*" unary | "/" unary)*
*/
Node* mul() {
  Token *token;

  Node* lhs = unary();

  token = get_token_from_tokens(pos);
  if (token->ty == '*') {
    pos++;
    return new_node('*', lhs, unary());
  }

  if (token->ty == '/') {
    pos++;
    return new_node('/', lhs, unary());
  }

  return lhs;
}

/*
  unary = ("+" | "-")? term
*/
Node* unary(void) {
  Token *token;
  Node *lhs;

  token = get_token_from_tokens(pos);

  if(token->ty == '+'){
    pos++;
    lhs = term();
    return lhs;
  }

  if(token->ty == '-') {
    pos++;
    return new_node('-', new_node_num(0), term());
  }

  lhs = term();
  return lhs;
}

/*
  term = num | "(" expr ")"
*/
Node* term() {
  Token *token;
  token = get_token_from_tokens(pos);

  if (token->ty == TK_NUM) {
    token = get_token_from_tokens(pos);
    pos++;
    stack_count++;
    return new_node_num(token->val);
  }

  if (token->ty == TK_IDENT) {
    token = get_token_from_tokens(pos);
    pos++;
    stack_count++;
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
