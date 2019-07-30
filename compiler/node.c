#include <stdio.h>
#include <stdlib.h>
#include "node.h"

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

Node* new_node_idnet(char *name) {
  Node* node = malloc(sizeof(Node));

  node->ty = ND_IDNET;
  node->name = name;

  return node;
}