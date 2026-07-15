#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    // 找到 '?' 的位置，获取查询参数部分
    const char *query_start = strchr(url, '?');
    if (query_start == NULL) {
        printf("URL中未找到查询参数\n");
        err = -1;
        goto exit;
    }

    // 跳到 '?' 之后
    query_start++;

    // 复制查询字符串以便使用 strtok 修改
    char *query_str = strdup(query_start);
    if (query_str == NULL) {
        perror("strdup");
        err = -1;
        goto exit;
    }

    // 使用 '&' 分割每个参数
    char *token;
    char *saveptr;
    token = strtok_r(query_str, "&", &saveptr);
    while (token != NULL) {
        // 使用 '=' 分割 key 和 value
        char *key = token;
        char *value = strchr(token, '=');
        if (value != NULL) {
            *value = '\0';  // 将 '=' 替换为字符串结束符
            value++;
            printf("key = %s, value = %s\n", key, value);
        }
        token = strtok_r(NULL, "&", &saveptr);
    }

    free(query_str);

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}