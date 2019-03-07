#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "node.h"
#include "tokenize.h"

int get_position(void);
void set_tokens(Vector *analyzed_tokens);
int number_of_ast(void);
Node* ast(int index);
void program();
void new_code(Node *node);
Node* assign(void);
Node* expr(void);
Node* mul(void);
Node* term(void);

#endif