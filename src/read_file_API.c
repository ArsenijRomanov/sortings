#include "../include/file_read.h"

char **read_file(const char *file, size_t *strings_cnt) {
  FILE *fp = fopen(file, "r");

  if (fp == NULL) {
    fprintf(stderr, "can't open file %s\n", file);
    return NULL;
  }

  int exit_code = 0;

  char *buf = calloc(sizeof(char), 1000);

  size_t strings_len = 100;
  char **strings = malloc(sizeof(char *) * strings_len);

  char symbol = (char)getc(fp);
  int index = 0;
  size_t strings_index = 0;

  while (symbol != EOF) {
    if (symbol == '\n' || symbol == '\0') {
      buf[index] = '\0';
      strings[strings_index] = malloc(index + 1);
      strcpy(strings[strings_index], buf);
      index = 0;
      ++strings_index;
      if (strings_index == strings_len) {
        strings_len += 100;
        char **new_strings = malloc(sizeof(char *) * strings_len);
        if (new_strings == NULL) {
          fprintf(stderr, "memory allocation error\n");
          exit_code = 1;
          goto stop_API;
        }
        for (size_t i = 0; i < strings_index; ++i) {
          new_strings[i] = strings[i];
        }
        free(strings);
        strings = new_strings;
      }
    } else {
      buf[index] = symbol;
      ++index;
      if (index == MAX_STR_LEN) {
        fprintf(stderr, "string length limit exceeded\n");
        exit_code = 1;
        goto stop_API;
      }
    }
    symbol = (char)getc(fp);
  }

  if (index != 0) {
    buf[index] = '\0';
    strings[strings_index] = malloc(index + 1);
    strcpy(strings[strings_index], buf);
    ++strings_index;
  }

stop_API:
  free(buf);
  fclose(fp);

  if (exit_code) {
    for (size_t i = 0; i < strings_index; ++i)
      free(strings[i]);
    free(strings);
    return NULL;
  }

  *strings_cnt = strings_index;
  return strings;
}
