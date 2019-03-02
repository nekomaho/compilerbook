#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"
#include "vector.h"
#include "test/test.h"

static Token tokens[100];
static int pos = 0;
static Node *code[100];
static int sentence = 0;

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
  Node* lhs = assign();

  new_code(lhs);
  pos++;

  if (tokens[pos].ty != TK_EOF) {
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
void gen_lval(Node *node){
  if (node->ty == ND_IDNET) {
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n",('z' - node->name + 1) * 8);
    printf("  push rax\n");

    return;
  }

  fprintf(stderr, "代入の左辺値が変数ではありません：%s", tokens[pos].input);
  exit(1);

}

void gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  if (node->ty == ND_IDNET) {
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
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

void error(int i) {
  fprintf(stderr, "予期せぬトークンです: %s\n", tokens[i].input);
  exit(1);
}

int main(int argc, char **argv) {
  if(argc !=2) {
    fprintf(stderr, "invalid arguments");
    return 1;
  }

  if(!strcmp(argv[1], "-test")) {
    runtest();
    return 0;
  }

  tokenize(argv[1]);
  program();

  printf(".intel_syntax noprefix\n");
  printf(".global _main\n");
  printf("_main:\n");

  // プロローグ
  // 変数26個分の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  // 先頭の式から順にコード生成
  for (int i = 0; code[i]; i++) {
    gen(code[i]);

    // 式の評価結果としてスタックに一つの値が残っている
    // はずなので、スタックが溢れないようにポップしておく
    printf("  pop rax\n");
  }

  // エピローグ
  // 最後の式の結果がRAXに残っているのでそれが返り値になる
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}
