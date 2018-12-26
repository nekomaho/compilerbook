// トークンの型を表す値
enum {
  TK_NUM = 256, // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
};

// トークンの型
typedef struct {
  int ty; //トークンの型
  int val; //tyがTK_NUMの場合、その数値
  char *input; //トークン文字列(エラーメッセージ用)
} Token;
