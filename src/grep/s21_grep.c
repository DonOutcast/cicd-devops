#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(
        stderr,
        "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
        "[-C[num]]\n"
        "        [-e pattern] [-f file] [--binary-files=value] [--color=when]\n"
        "        [--context[=num]] [--directories=action] [--label] "
        "[--line-buffered]\n"
        "        [--null] [pattern] [file ...]\n");
  }
  Spisok *template = NULL;
  Spisok *file_name = NULL;
  Check_options options;
  bring_to_zero(&options);
  int res = parsing_flags(argc, argv, &options);
  if (!res) {
    int flag = 0;
    for (int i = 1; i < argc; i++) {
      if (*argv[i] != '-' && *argv[i] != '\0') {
        if (flag == 0) {
          add_template(&template, argv[i]);
          flag = 1;
        } else {
          add_file_name(&file_name, argv[i]);
        }
      }
    }
  } else {
    for (int i = 1; i < argc; i++) {
      int result_e = search_E(argv[i - 1]);
      int result_f = search_F(argv[i - 1]);
      if (result_e == 1 && !options._f) {
        add_template(&template, argv[i]);
      } else if (result_f == 1 && options._f) {
        open_F(&template, argv[i]);

      } else if (check_e_middle(argv[i], &template)) {
        continue;
      } else if (*argv[i] != '-' && !options._e && !template && !options._f) {
          add_template(&template, argv[i]);
      } else if (*argv[i] != '-') {
        add_file_name(&file_name, argv[i]);
      }
    }
  }
  read_file(template, file_name, &options);
  free_list(template);
  free_list(file_name);
  return 0;
}

int count_file_name(Spisok *file_name) {
  int result = 0;
  while (file_name) {
    result++;
    file_name = file_name->next;
  }
  return result;
}

int use_regex(char *keyword, char *str, int i_flag, regmatch_t *pmatch) {
  regex_t reg;
  int result_regex = 1;
  int result = 1;
  result_regex = regcomp(&reg, keyword, i_flag);
  if (result_regex) {
    printf("Error\n");
  } else {
    result = regexec(&reg, str, 1, pmatch, 0);
  }

  regfree(&reg);
  return result;
}

void open_F(Spisok **template, char *str) {
  // printf("str %s\n", str);
  FILE *pointer_file;
  // char file_name[100] = {0};
  // strcpy(file_name, str);
  //   if (*str == '-' && *(str + 1) == 'f' && *(str + 2) == '\0') {
  //       printf("name %s\n", (str + 1));
  pointer_file = fopen(str, "r");
  if (pointer_file == NULL) {
    printf("I am here %s: %s\n", (*template)->name, strerror(errno));
  } else {
      int flag = 0;
      char fox[256];
    while (fgets(fox, 255, pointer_file)) {
      if (fox[strlen(fox) - 1] == '\n') fox[strlen(fox) - 1] = '\0';

      if (flag == 0) {
        (*template) = create((*template), fox);
        flag = 1;
      } else {
        add((*template), fox);
      }
    }
  }
}

void read_file(Spisok *template, Spisok *file_name, Check_options *options) {
  int i_flag = (options->_i) ? REG_ICASE | REG_EXTENDED
                             : REG_EXTENDED;  // for upper vs lower case;
  char bag_file[SIZE];
  int result_useR;
  regmatch_t pmatch[1];
  int count_files = count_file_name(file_name);
  int line_matches = 0;
  while (file_name) {
    FILE *pointer_file = fopen(file_name->name, "r");
    if (pointer_file == NULL) {
      if (!options->_s) {  // FLAG S is a READY!!!
        fprintf(stderr, "grep: %s: No such file or directory\n",
                file_name->name);  // stderr
      }
    } else {
      int number_line = 1;
      while (fgets(bag_file, SIZE - 1, pointer_file)) {
        int match = 0;
        Spisok *tmp = template;
        while (tmp) {
          result_useR = use_regex(tmp->name, bag_file, i_flag, pmatch);
          if (result_useR == 0 && !options->_v) match = 1;
          if (result_useR == REG_NOMATCH && options->_v) match = 1;
          if (match && !options->_l && !options->_c && options->_n) {
            if (count_files > 1) {
              printf("%s:%d:%s", file_name->name, number_line, bag_file);
              break;
            } else {
              printf("%d:%s", number_line, bag_file);
              break;
            }
          }
          if (options->_o && match) {
              print_if_flag_o(tmp->name, bag_file, options);
            // for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
            //   printf("%c", bag_file[i]);
            // }
            // printf("\n");
        }

          if (match && options->_h) {  // Flag H is a READY!!!
            printf("%s", bag_file);
            break;
          }
          if (match && !options->_l && !options->_c && !options->_o) {
            if (count_files > 1) {
                printf("%s:%s", file_name->name, bag_file);
                break;
            } else {
                printf("%s", bag_file);
                break;
            }
          }
          // if (result_useR == 0 ) {
          //   if (!options->_v) {
          //     match = 1;
          //   }
          //   if (options->_n && !options->_l &&
          //       !options->_c) {  // Flag N is a ready!!!
          //     if (count_files > 1) {
          //       printf("%s:%d:%s", file_name->name, number_line, bag_file);
          //     } else {
          //       printf("%d:%s", number_line, bag_file);
          //     }
          //   }
          //   if (!options->_l && !options->_c && !options->_n &&
          //       !options->_v) {  // finalaze for over flags
          //     if (count_files > 1) {
          //       printf("%s:%s", file_name->name, bag_file);
          //     } else {
          //       printf("%s", bag_file);
          //     }
          //   }
          // } else if (result_useR == REG_NOMATCH &&
          //            options->_v) {  // FLag V is a ready!!!!
          //   if (count_files > 1) {
          //     printf("%s:%s", file_name->name, bag_file);
          //   } else {
          //     printf("%s", bag_file);
          //   }
          // }
          // printf("%d", options->_o);
          // printf("Match : %d\n", match);

          number_line++;
          line_matches += match;
          tmp = tmp->next;
        }
      }
    }
    if (options->_c) {  // Flag C is a resdy!!!
      if (options->_l) {
          line_matches = 1;
        }
      if (count_files > 1) {
        printf("%s:%d\n", file_name->name, line_matches);
      } else {
        printf("%d\n", line_matches);
      }
    }

    if (options->_l && line_matches > 0) {  // Flag L is a ready !!!
      printf("%s\n", file_name->name);
    }

    file_name = file_name->next;
    fclose(pointer_file);  // I am need regfree();
  }
}

int check_e_middle(char *str, Spisok **template) {
  int result = 0;
  if (*str == '-') {
    while (*str != '\0') {
      if (*str == 'e' && *(str + 1) != '\0') {
        if (!(*template)) {
          (*template) = create((*template), str + 1);
        } else {
          add((*template), str + 1);
        }
        result = 1;
        break;
      } else if (*str == 'f' && *(str + 1) != '\0') {
        open_F(template, str + 1);
      }
      str++;
    }
  }
  return result;
}

int search_F(char *str) {
  int result = 0;
  int lenstr = 0;
  lenstr = strlen(str);
  if (*str == '-' && *(str + 1) == 'f' && *(str + 2) == '\0') {
    result = 1;
  } else if (*str == '-' && *(str + 1) != 'f' && str[lenstr - 1] == 'f') {
    result = 1;
  }
  return result;
}

int search_E(char *str) {  // argv[i - 1]
  int result = 0;
  int lenstr = 0;
  lenstr = strlen(str);
  if (*str == '-' && *(str + 1) == 'e' && *(str + 2) == '\0') {
    result = 1;
  } else if (*str == '-' && *(str + 1) != 'e' && str[lenstr - 1] == 'e') {
    result = 1;
  }
  return result;
}

int my_strlen(const char *x) {
  int i = 0;
  for (; x[i] != '\0'; i++) {
  }

  return i;
}

int parsing_flags(int argc, char *argv[], Check_options *options) {
  int e = 0, f = 0, result = 0;
  for (int i = 1; i < argc; i++) {
    for (int j = 0; j < my_strlen(argv[i]); j++) {
      if (*argv[i] == '-') {
        if (argv[i][j] == 'e') {
          options->_e = 1;
          e = 1;
          result = 1;
          break;
        } else if (argv[i][j] == 'i' && e == 0 && f == 0) {
          options->_i = 1;
        } else if (argv[i][j] == 'c' && e == 0 && f == 0) {
          options->_c = 1;
        } else if (argv[i][j] == 'l' && e == 0 && f == 0) {
          options->_l = 1;
        } else if (argv[i][j] == 'n' && e == 0 && f == 0) {
          options->_n = 1;
        } else if (argv[i][j] == 'h' && e == 0 && f == 0) {
          options->_h = 1;
        } else if (argv[i][j] == 's' && e == 0 && f == 0) {
          options->_s = 1;
        } else if (argv[i][j] == 'o' && e == 0 && f == 0) {
          options->_o = 1;
        } else if (argv[i][j] == 'v' && e == 0 && f == 0) {
          options->_v = 1;
        } else if (argv[i][j] == 'f' && e == 0 && f == 0) {
          options->_f = 1;
          f = 1;
          result = 1;
          break;
        }
      }
    }
  }
  return result;
}

void bring_to_zero(Check_options *options) {
  options->_e = 0;
  options->_i = 0;
  options->_v = 0;
  options->_c = 0;
  options->_l = 0;
  options->_n = 0;
  options->_h = 0;
  options->_s = 0;
  options->_f = 0;
  options->_o = 0;
}

Spisok *free_list(Spisok *list) {
  while (list != NULL) {
    Spisok *temporary = list;
    list = list->next;
    free(temporary);
  }
  return NULL;
}

void add(Spisok *list, char *str) {
  Spisok *new = (Spisok *)malloc(sizeof(Spisok));
  strcpy(new->name, str);
  new->next = NULL;
  Spisok *tmp = list;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new;
}

Spisok *create(Spisok *list, char *str) {
  if (!list) {
    Spisok *new = (Spisok *)malloc(sizeof(Spisok));
    strcpy(new->name, str);
    new->next = NULL;
    list = new;
  }
  return (list);
}

void add_template(Spisok **template, char *str) {
  if (!(*template)) {
    (*template) = create((*template), str);
  } else {
    add((*template), str);
  }
}

void add_file_name(Spisok **file_name, char *str) {
  if (!(*file_name)) {
    (*file_name) = create((*file_name), str);
  } else {
    add((*file_name), str);
  }
}

// void output(Spisok *list) {
//   Spisok *ptr = list;
//   while (ptr != NULL) {
//     printf("%s\n", ptr->name);
//     ptr = ptr->next;
//   }
// }


void print_if_flag_o(char *keyword, char *str, Check_options *options) {
  regex_t regex;
  regmatch_t matchptr;
  size_t len = strlen(str);
  int marker = 0;
  if (options->_i) marker = REG_EXTENDED | REG_ICASE;
  int return_value = regcomp(&regex, keyword, marker);
  if (!return_value) {
    size_t nmatch = 1, offset = 0;
    while (regexec(&regex, str + offset, nmatch, &matchptr, 0) == 0) {
      matchptr.rm_so += offset;
      matchptr.rm_eo += offset;
      if (matchptr.rm_eo > (regoff_t)len) break;
      if (matchptr.rm_eo <= (regoff_t)len) {
        for (int i = matchptr.rm_so; i < matchptr.rm_eo; i++) {
          printf("%c", str[i]);
        }
        printf("\n");
      }
      offset = matchptr.rm_eo;
    }
  }
  regfree(&regex);
}
