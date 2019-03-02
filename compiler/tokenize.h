#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

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

void allocate_tokens(Token *empty_tokens);
void tokenize(char *p);

#endif