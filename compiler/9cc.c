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

static Vector *tokens;

int main(int argc, char **argv) {
  if(argc !=2) {
    fprintf(stderr, "invalid arguments");
    return 1;
  }

  if(!strcmp(argv[1], "-test")) {
    runtest();
    return 0;
  }

  tokens = tokenize(argv[1]);

  set_tokens(tokens);
  program(tokens);

  if (GEN_ERROR_END == output_asm()) {
    fprintf(stderr, "代入の左辺値が変数ではありません：%s", (char *)((Token *)(tokens->data)[get_position()])->input);
    exit(-1);
  }

  return 0;
}