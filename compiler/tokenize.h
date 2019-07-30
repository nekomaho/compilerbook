#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

#include "vector.h"

// トークンの型を表す値
enum {
  TK_NUM = 256, // 整数トークン
  TK_IDENT,     // 識別子
  TK_EOF,       // 入力の終わりを表すトークン
  TK_RETURN,    // return
};

// トークンの型
typedef struct {
  int ty; //トークンの型
  int val; //tyがTK_NUMの場合、その数値
  char *variabale_name; //変数文字列
  char *input; //トークン文字列
} Token;

Vector *tokenize(char *p);

#endif