#ifndef __TEST_H__
#define __TEST_H__

#define TEST_OK 1
#define TEST_NG 0

int expect(char *file_name, int line, int expected, int actual);
void runtest(void);

#endif