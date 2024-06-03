#include "s21_grep.h"

#include "functions.h"

struct option long_flags[] = {
    {"ignore-case", 0, NULL, 'i'}, {"invert-match", 0, NULL, 'v'},
    {"count", 0, NULL, 'c'},       {"files-with-matches", 0, NULL, 'l'},
    {"line-number", 0, NULL, 'n'}, {"no-filename", 0, NULL, 'h'},
    {"no-massage", 0, NULL, 's'},  {"file", 0, NULL, 'f'}};

int parsing_args(int argc, char *argv[], flags *flags, char *patterns[],
                 char *files[], Variables *variables) {
  int flag, error = 0;
  while (!error && (flag = getopt_long(argc, argv, "e:ivclnhsf:", long_flags,
                                       NULL)) != -1) {
    switch (flag) {
      case 'e':
        flags->e = 1;
        if (strlen(optarg) < MAX_SIZE)
          patterns[variables->patterns_cnt++] = optarg;
        else {
          printf("s21_grep: out of memory\n");
          error = 1;
        }
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        files[variables->files_cnt++] = optarg;
        break;
      default:
        error = 1;
        usage();
    }
  }

  if (flags->l || flags->c) flags->n = 0;
  return error;
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    char *patterns[argc], *files[argc];
    flags flags = {0};
    Variables variables = {0};
    int error = parsing_args(argc, argv, &flags, patterns, files, &variables);
    if (!error) {
      reading_from_file(argc, argv, flags, &variables, patterns, files);
    }
  } else
    usage();
  return 0;
}
