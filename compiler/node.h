#ifndef __NODE_H__
#define __NODE_H__

enum {
  ND_NUM = 256, // 整数ノードの型
  ND_IDNET,
  ND_SUB,   // =
  ND_PLUS,  // +
  ND_MINUS, // - 
  ND_MULTI, // *
  ND_DIV,   // /
  ND_EQ,    // ==
  ND_NEQ,   // !=
  ND_LT,    // <
  ND_LTE,   // <=
  ND_RT,    // >
  ND_RTE,   // >=
  ND_RETURN,// return
  ND_IF,    // if
  ND_ELSE,  // else
};

// Node
typedef struct Node {
  int ty; // 演算子かND_NUM
  struct Node *lhs; // 左辺
  struct Node *rhs; // 右辺
  int val; //tyがND_NUMの場合に数字が入る
  char *name; //tyがND_IDNETの場合に変数名が入る
} Node;

Node* new_node(int ty, Node *lhs, Node *rhs);
Node* new_node_num(int val);
Node* new_node_idnet(char *name);

#endif