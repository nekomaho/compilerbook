#include <stdio.h>
#include "test.h"
#include "../vector.h"

void vector_tests() {
  Vector *vector = new_vector();

  expect(__FILE__, __LINE__, 0, vector->len);
  expect(__FILE__, __LINE__, 16, vector->capacity);

  for (int i = 0; i < 100; i++)
    vec_push(vector, (void *)i);

  expect(__FILE__, __LINE__, 100, vector->len);
  expect(__FILE__, __LINE__, 0, (int)vector->data[0]);
  expect(__FILE__, __LINE__, 50, (int)vector->data[50]);
  expect(__FILE__, __LINE__, 99, (int)vector->data[99]);
}