#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) return NULL;
    Node *head = NULL, *prev = NULL;
    for (int i = 1; i <= n; i++) {
        Node *node = malloc(sizeof(Node));
        node->id = i;
        node->next = NULL;
        if (head == NULL)
            head = node;
        else
            prev->next = node;
        prev = node;
    }
    if (prev != NULL)
        prev->next = head;
    return head;
}

void free_list(Node* head) {
    if (head == NULL) return;
    Node *cur = head->next;
    while (cur != head) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    free(head);
}