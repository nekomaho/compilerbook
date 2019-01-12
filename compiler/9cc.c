#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"

static Token tokens[100];
static int pos = 0;

Node* new_node(int ty, Node *lhs, Node *rhs) {
  Node* node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;

  return node;
}

Node* new_node_num(int val) {
  Node* node = malloc(sizeof(Node));

  node->ty = ND_NUM;
  node->val = val;

  return node;
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

void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->ty) {
    case '+':
      printf("  add rax, rdi\n");
      break;
    case '-':
      printf("  sub rax, rdi\n");
      break;
    case '*':
      printf("  mul rdi\n");
      break;
    case '/':
      printf("  mov rdx, 0\n");
      printf("  div rdi\n");
  }

  printf("  push rax\n");
}

void tokenize(char *p) {
  int i = 0;
  while(*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if(*p == '+' || *p == '-' || *p == '*' || *p == '(' || *p == ')' || *p =='/' ) {
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

    fprintf(stderr, "トークナイズ出来ません: %s\n", p);
    exit(1);
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

void error(int i) {
  fprintf(stderr, "予期せぬトークンです: %s\n", tokens[i].input);
  exit(1);
}

int main(int argc, char **argv) {
  if(argc !=2) {
    fprintf(stderr, "invalid arguments");
    return 1;
  }

  tokenize(argv[1]);

  Node* node = expr();

  printf(".intel_syntax noprefix\n");
  printf(".global _main\n");
  printf("_main:\n");

  // 抽象構文木を降りながらコード生成
  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
