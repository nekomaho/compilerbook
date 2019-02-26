enum {
  ND_NUM = 256, // 整数ノードの型
  ND_IDNET,
};

// トークンの型を表す値
enum {
  TK_NUM = 256, // 整数トークン
  TK_IDENT,     // 識別子
  TK_EOF,       // 入力の終わりを表すトークン
};

// トークンの型
typedef struct {
  int ty; //トークンの型
  int val; //tyがTK_NUMの場合、その数値
  char *input; //トークン文字列
} Token;

// Node
typedef struct Node {
  int ty; // 演算子かND_NUM
  struct Node *lhs; // 左辺
  struct Node *rhs; // 右辺
  int val; //tyがND_NUMの場合に数字が入る
  char name; //tyがND_IDNETの場合に変数名が入る
} Node;

typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

Vector *new_vector(void);
void vec_push(Vector *vec, void *elem);

int expect(int line, int expected, int actual);
void runtest(void);

void tokenize(char *p);
void error(int i);
void new_code(Node *node);
Node* new_node(int ty, Node *lhs, Node *rhs);
Node* new_node_num(int val);
Node* new_node_idnet(char name);
Node* assign(void);
Node *expr(void);
Node *mul(void);
Node *term(void);
void gen(Node *node);
