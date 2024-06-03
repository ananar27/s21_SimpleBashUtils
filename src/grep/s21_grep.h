#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE
#define MAX_SIZE 2000
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
} flags;

typedef struct variables {
  int str_counter;
  int total_files;
  int patterns_cnt;
  int files_cnt;
  int index;
  int str_number;
  int regex_error;
  char reg_pattern[MAX_SIZE];
} Variables;

#endif
