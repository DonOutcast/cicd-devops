#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct struct_colors {
  int Color_b;
  int Color_e;
  int Color_n;
  int Color_s;
  int Color_t;
  int Color_v;
} struct_colors;
void no_files();
void bring_to_zero(struct_colors *check_colors);
int parsing_colors(int argc, char *argv[], struct_colors *chek_colors);
// void input_file(char *argv[], struct_colors *check_colors);
int color_S(int last_symbol, int fread, int *empty_str_count,
            struct_colors *check_colors);
void print_color_B(int *count_string);
int color_B(int last_symbol, int fread, struct_colors *check_colors);
int color_T(int fread, struct_colors *check_colors);
void print_color_T(int *fread);
int color_E(int fread, struct_colors *check_colors);
void print_color_E();
int color_V(int *fraed, struct_colors *check_colors);
void read_file(char *argv[], struct_colors *check_colors);
#endif  // SRC_CAT_S21_CAT_H_
