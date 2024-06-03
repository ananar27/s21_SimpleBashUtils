#include "functions.h"

#include "s21_grep.h"

void first_pattern(Variables *variables, char *patterns[], int cnt) {
  for (int i = 0; i < cnt; i++) {
    size_t max_length = MAX_SIZE - strlen(variables->reg_pattern);
    if (strlen(patterns[i]) < max_length) {
      if ((strlen(variables->reg_pattern) != 0)) {
        strcat(variables->reg_pattern, "|");
        strcat(variables->reg_pattern, patterns[i]);
      } else
        strcat(variables->reg_pattern, patterns[i]);
    }
  }
}

int check_files(Variables *variables, char *files[]) {
  int error = 0, size_error = 0;
  for (int i = 0; i < variables->files_cnt && !error; i++) {
    FILE *file = fopen(files[i], "r");
    if (file != NULL) {
      int n = 0;
      char *str = NULL;
      char *patterns_from_file[MAX_SIZE];
      size_t len = 0;
      ssize_t size;

      while ((size = getline(&str, &len, file)) != EOF && !size_error) {
        if (size < MAX_SIZE && n < MAX_SIZE) {
          str[size - 1] = '\0';
          patterns_from_file[n++] = str;
          str = NULL;
        } else {
          printf("s21_grep: out of memory\n");
          size_error = 1;
        }
      }

      if (str) free(str);

      if (!size_error && !error) {
        first_pattern(variables, patterns_from_file, n);

        for (int j = 0; j < n; j++) free(patterns_from_file[j]);
      } else
        error = 1;
      closing_file(file);
    } else {
      printf("s21_grep: %s: No such file or directory\n", files[i]);
      error = 1;
    }
  }
  return error;
}

void reading_from_file(int argc, char *argv[], flags flags,
                       Variables *variables, char *patterns[], char *files[]) {
  int error = 0;
  if (flags.f) error = check_files(variables, files);
  if (!error) {
    if (flags.e) first_pattern(variables, patterns, variables->patterns_cnt);
    variables->total_files =
        flags.e || flags.f ? argc - optind : argc - optind - 1;
    for (variables->index = !flags.e && !flags.f ? optind + 1 : optind;
         variables->index < argc; variables->index++) {
      FILE *file = fopen(argv[variables->index], "r");
      if (file != NULL) {
        variables->str_counter = 0;
        file_parcing(argv, file, flags,
                     flags.e || flags.f ? variables->reg_pattern : argv[optind],
                     variables);
        closing_file(file);
      } else if (!flags.s)
        printf("s21_grep: %s: No such file or directory\n",
               argv[variables->index]);
    }
  }
}

void file_parcing(char *argv[], FILE *file, flags flags, char pattern[],
                  Variables *variables) {
  int stop = 0;
  int regex_flag = REG_EXTENDED;
  if (flags.i) regex_flag = REG_EXTENDED | REG_ICASE;
  regex_t reg;
  int reg_expr =
      regcomp(&reg, !flags.e && !flags.f ? pattern : variables->reg_pattern,
              regex_flag);
  if (reg_expr) {
    printf("s21_grep: Invalid regular expression\n");
  } else {
    int str_number = 0;
    char *str = NULL;
    size_t len = 0;
    while (getline(&str, &len, file) != EOF && !stop) {
      str_number++;

      int match = regexec(&reg, str, 0, NULL, 0);

      printing_data(argv, match, flags, str, str_number, variables, &stop);
    }

    if (flags.c) {
      if (variables->total_files == 1 || flags.h)
        printf("%d\n", variables->str_counter);
      else
        printf("%s:%d\n", argv[variables->index], variables->str_counter);
    }

    if (flags.l && stop) printf("%s\n", argv[variables->index]);

    if (str) free(str);
    regfree(&reg);
  }
}

void printing_data(char *argv[], int match, flags flags, char str[],
                   int str_number, Variables *variables, int *stop) {
  if ((!flags.v && match == 0) || (flags.v && match == REG_NOMATCH)) {
    if (flags.l) *stop = 1;
    if (flags.c) variables->str_counter++;

    if ((flags.n || flags.h) && !flags.l && !flags.c) {
      if (flags.h && !flags.n)
        printf("%s", str);
      else if (variables->total_files == 1 || flags.h)
        printf("%d:%s", str_number, str);
      else if (variables->total_files > 1)
        printf("%s:%d:%s", argv[variables->index], str_number, str);
    }

    else if (!flags.c && !flags.n && !flags.l && !flags.h) {
      if (variables->total_files == 1)
        printf("%s", str);
      else if (variables->total_files > 1)
        printf("%s:%s", argv[variables->index], str);
    }
  }
}

void closing_file(FILE *file) {
  if (fclose(file) == EOF) printf("Error closing file\n");
}

void usage() { printf("Usage: s21_grep [-enlcoshf] [file]\n"); }
