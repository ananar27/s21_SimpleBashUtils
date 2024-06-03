#ifndef S21_CAT_H
#define S21_CAT_H
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int v;
  int n;
  int s;
  int t;
} flags;

int flag_flags(int code_result, flags *flag);
void file_execution(flags *flag, int argc, char **argv);

#endif
