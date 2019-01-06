// トークンの型を表す値
enum {
  ND_NUM = 256, // 整数トークン
};

// トークンの型
typedef struct {
  int ty; //トークンの型
  int val; //tyがTK_NUMの場合、その数値
} Token;

// Node
typedef struct Node {
  int ty; // 演算子かND_NUM
  struct Node *lhs; // 左辺
  struct Node *rhs; // 右辺
  int val; //tyがND_NUMの場合に数字が入る
} Node;

void tokenize(char *p);
void error(int i);
Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr(void);
Node *mul(void);
Node *term(void);
void gen(Node *node);
