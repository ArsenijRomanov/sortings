#include "../include/file_read.h"

char **read_file(const char *file, size_t *strings_cnt) {
  int fd = open(file, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "can't open file %s\n", file);
    return NULL;
  }

  struct stat file_info;
  if (fstat(fd, &file_info) == -1) {
    fprintf(stderr, "read file error\n");
    close(fd);
    return NULL;
  }
  size_t file_size = file_info.st_size;

  char *data_ptr = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_ptr == MAP_FAILED) {
    fprintf(stderr, "read file error\n");
    close(fd);
    return NULL;
  }

  close(fd);

  int exit_code = 0;

  char *buf = calloc(sizeof(char), 1000);

  size_t strings_len = 100;
  char **strings = malloc(sizeof(char *) * strings_len);

  size_t index = 0;
  size_t strings_index = 0;

  for (size_t i = 0; i < file_size; ++i) {
    char symbol = data_ptr[i];
    if (symbol == '\n' || symbol == '\0') {
      buf[index] = '\0';
      strings[strings_index] = malloc(index + 1);
      strcpy(strings[strings_index], buf);
      index = 0;
      ++(strings_index);
      if (strings_index == strings_len) {
        strings_len += 100;
        char **new_strings = malloc(sizeof(char *) * strings_len);
        if (new_strings == NULL) {
          fprintf(stderr, "memory allocation error\n");
          exit_code = 1;
          goto stop_mmap;
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
        goto stop_mmap;
      }
    }
  }

  if (index != 0) {
    buf[index] = '\0';
    strings[strings_index] = malloc(index + 1);
    strcpy(strings[strings_index], buf);
    ++strings_index;
  }

stop_mmap:
  free(buf);

  if (exit_code) {
    for (size_t i = 0; i < strings_index; ++i)
      free(strings[i]);
    free(strings);
    return NULL;
  }

  *strings_cnt = strings_index;
  return strings;
}
