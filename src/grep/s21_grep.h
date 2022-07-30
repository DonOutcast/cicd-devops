#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define SIZE 1024
typedef struct template {
  char name[256];
  struct template *next;
}
Spisok;

typedef struct options {
  int _e;
  int _i;
  int _v;
  int _c;
  int _l;
  int _n;
  int _h;
  int _s;
  int _f;
  int _o;
} Check_options;

int parsing_flags(int argc, char *argv[], Check_options *options);
void bring_to_zero(Check_options *options);
Spisok *free_list(Spisok *list);
void add(Spisok *list, char *str);
Spisok *create(Spisok *list, char *str);
void add_template(Spisok **template, char *str);
void add_file_name(Spisok **file_name, char *str);
int my_strlen(const char *x);
int search_E(char *str);
int check_e_middle(char *str, Spisok **template);
void read_file(Spisok *template, Spisok *file_name, Check_options *options);
int use_regex(char *keyword, char *str, int i_flag, regmatch_t *pmatch);
void filework(Check_options *options, FILE *pointer_file, regex_t reg,
              Spisok *file_name);

int count_file_name(Spisok *file_name);
void output(Spisok *list);
void open_F(Spisok **template, char *str);
int search_F(char *str);
void print_if_flag_o(char *keyword, char *str, Check_options *options);
#endif  //  SRC_GREP_S21_GREP_H_
