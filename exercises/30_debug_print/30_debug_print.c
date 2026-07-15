#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#define DEBUG_LEVEL 2

#define DEBUG_PRINT(fmt, ...) \
    do { \
        if (DEBUG_LEVEL >= 2) { \
            printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
        } else if (DEBUG_LEVEL >= 1) { \
            printf("DEBUG: " fmt "\n", ##__VA_ARGS__); \
        } \
    } while (0)

//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding
// padding

// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}