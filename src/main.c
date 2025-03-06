#include "../include/file_read.h"
#include "../include/sortings.h"

size_t my_atoi(const char *arg, int *exit_code) {
  size_t cnt = 0;
  size_t length = strlen(arg);
  for (size_t i = 0; i < length; ++i) {
    switch (arg[i]) {
    case '1':
      cnt *= 10;
      cnt += 1;
      break;
    case '2':
      cnt *= 10;
      cnt += 2;
      break;
    case '3':
      cnt *= 10;
      cnt += 3;
      break;
    case '4':
      cnt *= 10;
      cnt += 4;
      break;
    case '5':
      cnt *= 10;
      cnt += 5;
      break;
    case '6':
      cnt *= 10;
      cnt += 6;
      break;
    case '7':
      cnt *= 10;
      cnt += 7;
      break;
    case '8':
      cnt *= 10;
      cnt += 8;
      break;
    case '9':
      cnt *= 10;
      cnt += 9;
      break;
    case '0':
      cnt *= 10;
      break;
    default:
      fprintf(stderr,
              "Invalid format of parameter for number of sorted strings\n");
      *exit_code = -1;
      return 0;
      break;
    }
  }
  return cnt;
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr, "Incorrect number of parameters\n");
    return -1;
  }

  int (*comparator)(const char *, const char *);

  if (strcmp("asc", argv[4]) == 0)
    comparator = asc;
  else if (strcmp("des", argv[4]) == 0)
    comparator = des;
  else {
    fprintf(stderr, "%s is not a comparator\n", argv[4]);
    return -1;
  }

  int exit_code = 0;
  size_t strings_cnt = 0;
  char **strings = read_file(argv[2], &strings_cnt);
  if (strings == NULL) {
    exit_code = -1;
    goto end;
  }

  size_t cnt = my_atoi(argv[1], &exit_code);
  if (exit_code)
    goto end;
  if (cnt > strings_cnt) {
    fprintf(stderr, "the parameter for the number of sorted strings exceeds "
                    "the number of strings in the file\n");
    goto end;
  }

  if (strcmp(argv[3], "bubble") == 0)
    sort_bubble(strings, cnt, comparator);
  else if (strcmp(argv[3], "insertion") == 0)
    sort_insertion(strings, cnt, comparator);
  else if (strcmp(argv[3], "merge") == 0)
    sort_merge(strings, cnt, comparator);
  else if (strcmp(argv[3], "quick") == 0)
    sort_quick(strings, cnt, comparator);
  else if (strcmp(argv[3], "radix") == 0)
    sort_radix(strings, cnt, comparator);
  else {
    fprintf(stderr, "unknown sorting algorithm: %s\n", argv[3]);
    exit_code = -1;
    goto end;
  }

  for (size_t i = 0; i < strings_cnt; ++i) {
    printf("%s\n", strings[i]);
  }

end:
  for (size_t i = 0; i < cnt; ++i)
    free(strings[i]);
  free(strings);
  return exit_code;
}
