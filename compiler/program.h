#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "node.h"
#include "tokenize.h"

int get_potition(void);
void set_tokens(Token *empty_tokens);
Node* ast(int index);
void program();
void new_code(Node *node);
Node* assign(void);
Node* expr(void);
Node* mul(void);
Node* term(void);

#endif