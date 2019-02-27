#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "../vector.h"

int expect(int line, int expected, int actual) {
  if (expected == actual) {
    return 0;
  }

  fprintf(stderr, "%d: %d expected, but got %d\n",line, expected, actual);
  exit(1);
}

void runtest(void) {
  Vector *vector = new_vector();

  expect(__LINE__, 0, vector->len);
  expect(__LINE__, 16, vector->capacity);

  for (int i = 0; i < 100; i++)
    vec_push(vector, (void *)i);

    expect(__LINE__, 100, vector->len);
    expect(__LINE__, 0, (int)vector->data[0]);
    expect(__LINE__, 50, (int)vector->data[50]);
    expect(__LINE__, 99, (int)vector->data[99]);

    printf("OK\n");
}