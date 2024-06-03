#include "s21_cat.h"

int main(int argc, char **argv) {
  flags flag = {0};

  char *short_flags = "+beEnstTv";
  struct option long_flags[] = {{"number-nonblank", 0, NULL, 'b'},
                                {"number", 0, NULL, 'n'},
                                {"squeeze-blank", 0, NULL, 's'},
                                {"show-nonprinting", 0, NULL, 'v'}};

  int code_result = 0;
  int error = 0;
  int pntr = 0;

  while (code_result != -1 && error == 0) {
    code_result = getopt_long(argc, argv, short_flags, long_flags, &pntr);
    error = flag_flags(code_result, &flag);
  }

  if (error == 1) {
    printf("Error. No such option! Try -beEnstTv or GNU format flags!");
    return 1;
  }

  file_execution(&flag, argc, argv);
  return 0;
}

int flag_flags(int code_result, flags *flag) {
  switch (code_result) {
    case 'b':
      flag->b = 1;
      return 0;
    case 'e':
      flag->e = 1;
      flag->v = 1;
      return 0;
    case 'E':
      flag->e = 1;
      return 0;
    case 'n':
      flag->n = 1;
      return 0;
    case 's':
      flag->s = 1;
      return 0;
    case 't':
      flag->t = 1;
      flag->v = 1;
      return 0;
    case 'T':
      flag->t = 1;
      return 0;
    case 'v':
      flag->v = 1;
      return 0;
    case '?':
      return 1;
    default:
      return 2;
  }
}

void file_execution(flags *flag, int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      FILE *file = fopen(argv[i], "r");
      char prev_c = '\n';
      int count = 0;
      int blank_str = 0;
      if (file != NULL) {
        char c = getc(file);
        unsigned char uchar_value = (unsigned char)c;
        while (c != EOF) {
          if (flag->s == 1 && prev_c == '\n') {
            if (c == '\n') {
              if (blank_str) {
                prev_c = c;
                c = getc(file);
                continue;
              }
              blank_str = 1;
            } else {
              blank_str = 0;
            }
          }

          if (flag->b == 1) {
            if (c != '\n' && prev_c == '\n') {
              printf("%6d\t", ++count);
            }
          } else if (flag->n == 1) {
            if (prev_c == '\n') {
              count++;
              printf("%6d\t", count);
            }
          }

          if (flag->t == 1) {
            if (c == '\t') {
              printf("^I");
              prev_c = c;
              c = getc(file);
              continue;
            }
          }

          if (flag->e == 1) {
            if (c == '\n') {
              printf("$");
            }
          }

          if (flag->v == 1) {
            if (c >= 0 && c < 32 && c != 9 && c != 10) {
              printf("^%c", c + 64);
              prev_c = c;
              c = getc(file);
              continue;
            } else if (uchar_value > 127 && uchar_value < 160) {
              printf("M-^%c", c - 64);
              prev_c = c;
              c = getc(file);
              continue;
            } else if (c == 127) {
              printf("^%c", c - 64);
              prev_c = c;
              c = getc(file);
              continue;
            }
          }

          prev_c = c;
          printf("%c", c);
          c = getc(file);
        }

      } else {
        printf("ERROR.FILE NOT FOUND");
      }
      fclose(file);
    }
  }
}