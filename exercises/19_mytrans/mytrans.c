// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    char *start = str;
    char *end;

    // 去除前导空白
    while (isspace((unsigned char)*start))
        start++;

    if (*start == '\0') {
        *str = '\0';
        return;
    }

    // 去除尾部空白
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
        end--;

    // 将 trimmed 内容移到原字符串开头
    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

    while (fgets(line, sizeof(line), file)) {
        trim(line);

        if (line[0] == '\0')
            continue;

        if (line[0] == '#') {
            // 如果之前有未保存的词条，先保存
            if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
                hash_table_insert(table, current_word, current_translation);
                (*dict_count)++;
            }

            // 新单词
            strncpy(current_word, line + 1, sizeof(current_word) - 1);
            current_word[sizeof(current_word) - 1] = '\0';
            current_translation[0] = '\0';
            in_entry = 1;
        } else if (line[0] != '\0' && strncmp(line, "Trans:", 6) == 0) {
            // 翻译行
            const char *trans_text = line + 6;
            if (current_translation[0] != '\0') {
                strncat(current_translation, "@", sizeof(current_translation) - strlen(current_translation) - 1);
            }
            strncat(current_translation, trans_text, sizeof(current_translation) - strlen(current_translation) - 1);
        }
    }

    // 保存最后一个词条
    if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
        hash_table_insert(table, current_word, current_translation);
        (*dict_count)++;
    }

  fclose(file);
  return 0;
}