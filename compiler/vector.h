#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

Vector *new_vector(void);
void vec_push(Vector *vec, void *elem);

#endif

