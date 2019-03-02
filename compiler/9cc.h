#ifndef __9CC_H__
#define __9CC_H__

#include "node.h"

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

void tokenize(char *p);
void error(int i);
void new_code(Node *node);
Node* assign(void);
Node *expr(void);
Node *mul(void);
Node *term(void);
void gen(Node *node);

#endif