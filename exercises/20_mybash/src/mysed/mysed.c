#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    // 找到第一个 '/' 的位置（跳过 's'）
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char *p = cmd + 2;  // 跳过 "s/"
    
    // 查找 old_str 的结束位置（下一个 '/'）
    const char *old_end = strchr(p, '/');
    if (!old_end) {
        return -1;
    }
    
    // 提取 old_str
    size_t old_len = old_end - p;
    *old_str = (char*)malloc(old_len + 1);
    if (!*old_str) {
        return -1;
    }
    strncpy(*old_str, p, old_len);
    (*old_str)[old_len] = '\0';
    
    p = old_end + 1;  // 跳过 '/'
    
    // 查找 new_str 的结束位置（下一个 '/'）
    const char *new_end = strchr(p, '/');
    if (!new_end) {
        // 如果最后一个 '/' 不存在，一直取到末尾
        size_t new_len = strlen(p);
        *new_str = (char*)malloc(new_len + 1);
        if (!*new_str) {
            free(*old_str);
            *old_str = NULL;
            return -1;
        }
        strncpy(*new_str, p, new_len);
        (*new_str)[new_len] = '\0';
    } else {
        size_t new_len = new_end - p;
        *new_str = (char*)malloc(new_len + 1);
        if (!*new_str) {
            free(*old_str);
            *old_str = NULL;
            return -1;
        }
        strncpy(*new_str, p, new_len);
        (*new_str)[new_len] = '\0';
    }

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    char *pos = strstr(str, old);
    if (pos == NULL) {
        return;  // 未找到
    }

    // 计算新字符串长度
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t tail_len = strlen(pos + old_len);
    
    // 检查缓冲区是否足够
    char *temp = NULL;
    
    // 如果 new 比 old 长，需要移动后面的内容
    if (new_len > old_len) {
        // 先检查是否会溢出
        if (strlen(str) + (new_len - old_len) >= 1024) {
            // 如果超出缓冲区，需要动态分配
            temp = (char*)malloc(strlen(str) + (new_len - old_len) + 1);
            if (!temp) return;
            // 拷贝 pos 之前的部分
            size_t prefix_len = pos - str;
            strncpy(temp, str, prefix_len);
            temp[prefix_len] = '\0';
            // 拼接 new
            strcat(temp, new);
            // 拼接剩余部分
            strcat(temp, pos + old_len);
            strcpy(str, temp);
            free(temp);
            return;
        }
        // 移动后面的字符为新字符串腾出空间
        memmove(pos + new_len, pos + old_len, tail_len + 1);
    } else if (new_len < old_len) {
        // 移动后面的字符向前覆盖
        memmove(pos + new_len, pos + old_len, tail_len + 1);
    }
    
    // 拷贝 new 字符串到 pos 位置
    memcpy(pos, new, new_len);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}