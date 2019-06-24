#include <stdio.h>
#include "gen.h"
#include "program.h"
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

static int gen_operator(int operator) {
  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (operator) {
    case ND_PLUS:
      printf("  add rax, rdi\n");
      break;
    case ND_MINUS:
      printf("  sub rax, rdi\n");
      break;
    case ND_MULTI:
      printf("  mul rdi\n");
      break;
    case ND_DIV:
      printf("  mov rdx, 0\n");
      printf("  div rdi\n");
      break;
    case ND_EQ:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzx rax, al\n");
      break;
    case ND_NEQ:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzx rax, al\n");
      break;
    case ND_LT:
    case ND_RT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzx rax, al\n");
      break;
    case ND_LTE:
    case ND_RTE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzx rax, al\n");
      break;
  }

  printf("  push rax\n");
  return GEN_SUCCESS;
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

  if (node->ty == ND_SUB) {
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return GEN_SUCCESS;
  }

  if (node->ty == ND_RETURN) {
    gen(node->lhs);
    return GEN_SUCCESS;
  }

  gen(node->lhs);
  gen(node->rhs);

  gen_operator(node->ty);

  return GEN_SUCCESS;
}


int output_asm(void) {
  printf(".intel_syntax noprefix\n");
  printf(".global _main\n");
  printf("_main:\n");

  // プロローグ
  // 変数26個分の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, %d\n", max_stack_count() * ALLOCATE_UNIT);

  // 先頭の式から順にコード生成
  for (int i = 0; i < number_of_ast(); i++) {
    if (GEN_ERROR_END == gen(ast(i)))
      return GEN_ERROR_END;

    // 式の評価結果としてスタックに一つの値が残っている
    // はずなので、スタックが溢れないようにポップしておく
    printf("  pop rax\n");
  }

  // エピローグ
  // 最後の式の結果がRAXに残っているのでそれが返り値になる
  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");

  return GEN_SUCCESS;
}
