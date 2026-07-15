#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

void init_queue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

bool is_empty(Queue *q) {
    return q->count == 0;
}

bool is_full(Queue *q) {
    return q->count == MAX_PEOPLE;
}

void enqueue(Queue *q, int id) {
    if (is_full(q)) return;
    q->data[q->tail].id = id;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
}

int dequeue(Queue *q) {
    if (is_empty(q)) return -1;
    int id = q->data[q->head].id;
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return id;
}

int main() {
    Queue q;
    int total_people = 50;
    int report_interval = 5;

    init_queue(&q);
    for (int i = 1; i <= total_people; i++) {
        enqueue(&q, i);
    }

    while (q.count > 1) {
        for (int i = 1; i < report_interval; i++) {
            int id = dequeue(&q);
            enqueue(&q, id);
        }
        int eliminated = dequeue(&q);
        printf("淘汰: %d\n", eliminated);
    }

    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}