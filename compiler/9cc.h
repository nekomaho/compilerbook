#ifndef __9CC_H__
#define __9CC_H__

#include "node.h"

void error(int i);
void new_code(Node *node);
Node* assign(void);
Node *expr(void);
Node *mul(void);
Node *term(void);
void gen(Node *node);

#endif