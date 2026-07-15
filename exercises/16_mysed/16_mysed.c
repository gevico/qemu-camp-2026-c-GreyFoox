#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    // 跳过 "s/"
    const char* p = cmd + 2;
    
    // 找到第一个 '/' 作为 old_str 的结束
    const char* old_end = strchr(p, '/');
    if (old_end == NULL) {
        return -1;
    }
    
    // 提取 old_str
    size_t old_len = old_end - p;
    *old_str = (char*)malloc(old_len + 1);
    if (*old_str == NULL) {
        return -1;
    }
    strncpy(*old_str, p, old_len);
    (*old_str)[old_len] = '\0';
    
    // 跳过 '/'
    p = old_end + 1;
    
    // 找到第二个 '/' 作为 new_str 的结束
    const char* new_end = strchr(p, '/');
    if (new_end == NULL) {
        free(*old_str);
        *old_str = NULL;
        return -1;
    }
    
    // 提取 new_str
    size_t new_len = new_end - p;
    *new_str = (char*)malloc(new_len + 1);
    if (*new_str == NULL) {
        free(*old_str);
        *old_str = NULL;
        return -1;
    }
    strncpy(*new_str, p, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char* pos = strstr(str, old);
    if (pos == NULL) {
        return;
    }
    
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t tail_len = strlen(pos + old_len);
    
    // 向后移动剩余部分为新字符串腾出空间
    memmove(pos + new_len, pos + old_len, tail_len + 1);
    // 复制新字符串
    memcpy(pos, new, new_len);
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}