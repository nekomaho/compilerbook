#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"
#include "vector.h"
#include "program.h"
#include "tokenize.h"
#include "node.h"
#include "gen.h"
#include "test/test.h"

static Token tokens[100];

int main(int argc, char **argv) {
  if(argc !=2) {
    fprintf(stderr, "invalid arguments");
    return 1;
  }

  if(!strcmp(argv[1], "-test")) {
    runtest();
    return 0;
  }

  allocate_tokens(tokens);
  tokenize(argv[1]);

  set_tokens(tokens);
  program(tokens);

  printf(".intel_syntax noprefix\n");
  printf(".global _main\n");
  printf("_main:\n");

  // プロローグ
  // 変数26個分の領域を確保する
  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  // 先頭の式から順にコード生成
  for (int i = 0; ast(i); i++) {
    if (GEN_ERROR_END == gen(ast(i))) {
      fprintf(stderr, "代入の左辺値が変数ではありません：%s", tokens[get_potition()].input);
      exit(-1);
    }

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