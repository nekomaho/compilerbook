#include <stdio.h>
#include <string.h>
#include "test.h"
#include "vector_test.h"

int expect(char *file_name, int line, int expected, int actual) {
  if (expected == actual) {
    fprintf(stdout, "%s: line %d: OK\n", file_name, line);
    return TEST_OK;
  }

  fprintf(stderr, "%s: line %d: %d expected, but got %d\n",file_name, line, expected, actual);
  return TEST_NG;
}

void runtest(void) {
  vector_tests();
}