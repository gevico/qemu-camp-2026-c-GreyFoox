#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
    0, 1, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
};

// 方向: 上, 下, 左, 右
int dir_row[] = {-1, 1, 0, 0};
int dir_col[] = {0, 0, -1, 1};

typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    Point data[MAX_ROW * MAX_COL];
    int top;
} Stack;

void push(Stack *s, int row, int col) {
    s->data[s->top].row = row;
    s->data[s->top].col = col;
    s->top++;
}

Point pop(Stack *s) {
    s->top--;
    return s->data[s->top];
}

bool is_empty(Stack *s) {
    return s->top == 0;
}

int main(void) {
    int visited[MAX_ROW][MAX_COL] = {0};
    Point prev[MAX_ROW][MAX_COL];  // 记录每个点的前驱
    
    Stack stack;
    stack.top = 0;
    
    push(&stack, 0, 0);
    visited[0][0] = 1;
    prev[0][0] = (Point){-1, -1};
    
    bool found = false;
    
    while (!is_empty(&stack)) {
        Point cur = pop(&stack);
        
        if (cur.row == MAX_ROW - 1 && cur.col == MAX_COL - 1) {
            found = true;
            break;
        }
        
        for (int i = 0; i < 4; i++) {
            int nr = cur.row + dir_row[i];
            int nc = cur.col + dir_col[i];
            
            if (nr >= 0 && nr < MAX_ROW && nc >= 0 && nc < MAX_COL &&
                maze[nr][nc] == 0 && !visited[nr][nc]) {
                push(&stack, nr, nc);
                visited[nr][nc] = 1;
                prev[nr][nc] = cur;
            }
        }
    }
    
    if (found) {
        // 从终点回溯到起点，收集路径
        Point path[MAX_ROW * MAX_COL];
        int path_len = 0;
        Point cur = {MAX_ROW - 1, MAX_COL - 1};
        
        while (cur.row != -1) {
            path[path_len++] = cur;
            cur = prev[cur.row][cur.col];
        }
        
        // 打印路径 (从终点到起点)
        for (int i = 0; i < path_len; i++) {
            printf("(%d, %d)\n", path[i].row, path[i].col);
        }
    } else {
        printf("No path!\n");
    }
    
    return 0;
}