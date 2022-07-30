#include "s21_cat.h"
int main(int argc, char *argv[]) {
  if (argc < 2) {
    no_files();
  }
  struct_colors check_colors;
  bring_to_zero(&check_colors);
  int x = parsing_colors(argc, argv, &check_colors);
  if (check_colors.Color_b == 1) {
    check_colors.Color_n = 0;
  }
  while (optind < argc) {
    if (x == 1) {
      read_file(argv, &check_colors);
    }
    optind++;
  }
  return 0;
}
void no_files() {
  int fread = 0;
  while ((fread = getchar()) != EOF) {
    printf("%c", fread);
  }
}
int parsing_colors(int argc, char *argv[], struct_colors *check_colors) {
  int stop = 1, value = 0, index = 0;
  opterr = 0;  // for disable invalid option
  static struct option funcOptions[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0},
  };
  while (-1 !=
         (value = getopt_long(argc, argv, "beEnstTv", funcOptions, &index))) {
    switch (value) {
      case 'b':
        check_colors->Color_b = 1;
        break;
      case 'e':
        check_colors->Color_e = 1;
        check_colors->Color_v = 1;
        break;
      case 'E':
        check_colors->Color_e = 1;
        break;
      case 'n':
        check_colors->Color_n = 1;
        break;
      case 's':
        check_colors->Color_s = 1;
        break;
      case 't':
        check_colors->Color_t = 1;
        check_colors->Color_v = 1;
        break;
      case 'T':
        check_colors->Color_t = 1;
        break;
      case 'v':
        check_colors->Color_v = 1;
        break;
      default:
        fprintf(stderr,
                "s21_cat: illegal option -- %c\nusage: s21_cat [-venstuv] "
                "[file ...]\n",
                argv[1][1]);
        stop = 0;
    }
  }
  return stop;
}
void bring_to_zero(struct_colors *check_colors) {
  check_colors->Color_b = 0;
  check_colors->Color_e = 0;
  check_colors->Color_n = 0;
  check_colors->Color_s = 0;
  check_colors->Color_t = 0;
  check_colors->Color_v = 0;
}
int color_S(int last_symbol, int fread, int *empty_str_count,
            struct_colors *check_colors) {
  int ret_val = 0;
  if (last_symbol == '\n' && check_colors->Color_s && fread == '\n') {
    ret_val = 1;
    (*empty_str_count)++;
  }
  return ret_val;
}
int color_B(int last_symbol, int fread, struct_colors *check_colors) {
  int start = 0;
  if (last_symbol == '\n' &&
      ((check_colors->Color_b && fread != '\n') || check_colors->Color_n)) {
    start = 1;
  }
  return start;
}
void print_color_B(int *count_string) { printf("%6d\t", (*count_string)++); }
int color_T(int fread, struct_colors *check_colors) {
  int start = 0;
  if (check_colors->Color_t && fread == '\t') {
    start = 1;
  }
  return start;
}
void print_color_T(int *fread) {
  printf("^");
  (*fread) = 'I';
}
int color_E(int fread, struct_colors *check_colors) {
  int start = 0;
  if (check_colors->Color_e && fread == '\n') {
    start = 1;
  }
  return start;
}
void print_color_E() { printf("$"); }
int color_V(int *fread, struct_colors *check_colors) {
  int start = 0;
  if (check_colors->Color_v && (*fread) != '\n') {
    if (((*fread) >= 0 && (*fread < 9)) || ((*fread) > 10 && (*fread) < 32)) {
      (*fread) += 64;
      printf("^");
    } else if ((*fread) > 127 && (*fread) <= 159) {
      (*fread) -= 64;
      printf("M-^");
    } else if ((*fread) == 127) {
      (*fread) -= 64;
      printf("^");
    }
  }
  return start;
}

void read_file(char *argv[], struct_colors *check_colors) {
  FILE *file;
  file = fopen(argv[optind], "r");
  if (file != NULL) {
    int str_count = 1, empty_str_count = 0, last_sym = '\n';
    int cur_c;
    while ((cur_c = fgetc(file)) != EOF) {
      if (color_S(last_sym, cur_c, &empty_str_count, check_colors)) {
        if (empty_str_count > 1) {
          continue;
        }
      } else {
        empty_str_count = 0;
      }
      if (color_B(last_sym, cur_c, check_colors)) {
        print_color_B(&str_count);
      }
      if (color_T(cur_c, check_colors)) {
        print_color_T(&cur_c);
      }
      if (color_E(cur_c, check_colors)) {
        print_color_E();
      }
      color_V(&cur_c, check_colors);
      printf("%c", cur_c);
      last_sym = cur_c;
    }
    fclose(file);
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or direcory\n", argv[optind]);
  }
}
