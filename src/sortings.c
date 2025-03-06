#include "../include/sortings.h"

#define MAX_STR_LEN 1000

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

int asc(const char *first, const char *second) { return strcmp(first, second); }

int des(const char *first, const char *second) { return strcmp(second, first); }

void sort_bubble(char **arr, size_t size,
                 int (*comparator)(const char *, const char *)) {
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size - 1; ++j) {
      if (comparator(arr[j], arr[j + 1]) > 0) {
        char *ptr = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = ptr;
      }
    }
  }
}

void sort_insertion(char **arr, size_t size,
                    int (*comparator)(const char *, const char *)) {

  for (size_t i = 1; i < size; ++i) {
    char *current_string = arr[i];
    for (long long j = (long long)(i - 1); j >= 0; --j) {
      if (comparator(arr[j], current_string) >= 0) {
        arr[j + 1] = arr[j];
        if (j == 0) {
          arr[0] = current_string;
        }
      } else {
        arr[j + 1] = current_string;
        break;
      }
    }
  }
}

void sort_quick(char **arr, size_t size,
                int (*comparator)(const char *, const char *)) {

  long long left = 0;
  long long right = (long long)(size - 1);
  long long cur_left = left;
  long long cur_right = right;
  char *pivot = arr[(long long)(size - 1) / 2];

  while (cur_left <= cur_right) {

    while (comparator(arr[cur_left], pivot) < 0) {
      ++cur_left;
    }

    while (comparator(arr[cur_right], pivot) > 0) {
      --cur_right;
    }

    if (cur_left <= cur_right) {
      char *swap = arr[cur_right];
      arr[cur_right--] = arr[cur_left];
      arr[cur_left++] = swap;
    }
  }

  if (cur_right > left) {
    sort_quick(arr, cur_right + 1, comparator);
  }

  if (cur_left < right) {
    sort_quick(arr + cur_left, right - cur_left + 1, comparator);
  }
}

void sort_merge(char **arr, size_t size,
                int (*comparator)(const char *, const char *)) {
  if (arr < arr + size - 1) {
    size_t right = size - 1;
    size_t mid = right / 2;

    sort_merge(arr, mid + 1, comparator);
    sort_merge(arr + mid + 1, size - mid - 1, comparator);

    size_t length_left = mid + 1;
    size_t length_right = right - mid;
    char **left_part = malloc(length_left * sizeof(char *));
    char **right_part = malloc(length_right * sizeof(char *));

    for (size_t i = 0; i < MAX(length_left, length_right); ++i) {
      if (i < length_left)
        left_part[i] = arr[i];
      if (i < length_right)
        right_part[i] = arr[mid + i + 1];
    }

    size_t arr_index = 0;
    size_t left_part_index = 0;
    size_t right_part_index = 0;

    while (left_part_index < length_left && right_part_index < length_right) {
      if (comparator(left_part[left_part_index], right_part[right_part_index]) <
          0)
        arr[arr_index++] = left_part[left_part_index++];
      else
        arr[arr_index++] = right_part[right_part_index++];
    }

    while (left_part_index < length_left) {
      arr[arr_index++] = left_part[left_part_index++];
    }

    while (right_part_index < length_right) {
      arr[arr_index++] = right_part[right_part_index++];
    }

    free(left_part);
    free(right_part);
  }
}

int get_symb(char *string, size_t index) {
  if (index < strlen(string))
    return string[index];
  return 0;
}

void reverse(char **arr, size_t size) {
  for (size_t i = 0; i < size / 2; ++i) {
    char *swap = arr[size - 1 - i];
    arr[size - 1 - i] = arr[i];
    arr[i] = swap;
  }
}

void sort_radix(char **arr, size_t size,
                int (*comparator)(const char *, const char *)) {
  const int range = 256;

  size_t max_len = 0;
  for (size_t i = 0; i < size; ++i)
    max_len = MAX(max_len, strlen(arr[i]));
  if (max_len == 0)
    return;

  for (long long index = (long long)(max_len - 1); index >= 0; --index) {

    int symb_cnt[256] = {0};
    char **step = malloc(size * sizeof(char *));

    for (size_t j = 0; j < size; ++j)
      ++symb_cnt[get_symb(arr[j], index)];

    for (int j = 1; j < range; ++j)
      symb_cnt[j] += symb_cnt[j - 1];

    for (long long j = (long long)(size - 1); j >= 0; --j) {
      int symb = get_symb(arr[j], index);
      step[symb_cnt[symb] - 1] = arr[j];
      --symb_cnt[symb];
    }

    for (size_t i = 0; i < size; ++i)
      arr[i] = step[i];

    free(step);
  }
  if (comparator == des)
    reverse(arr, size);
}
