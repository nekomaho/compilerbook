#include <stdio.h>
#include "gen.h"
#include "tokenize.h"

static int gen_lval(Node *node){
  if (node->ty == ND_IDNET) {
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n",('z' - node->name + 1) * 8);
    printf("  push rax\n");

    return GEN_SUCCESS;
  }
  return GEN_ERROR_END;
}

int gen(Node *node) {
  if (node->ty == ND_NUM) {
    printf("  push %d\n", node->val);
    return GEN_SUCCESS;
  }

  if (node->ty == ND_IDNET) {
      if (GEN_ERROR_END == gen_lval(node))
          return GEN_ERROR_END;
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return GEN_SUCCESS;
  }

  if (node->ty == '=') {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return GEN_SUCCESS;
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
  return GEN_SUCCESS;
}
