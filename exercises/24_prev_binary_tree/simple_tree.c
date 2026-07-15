#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    node->tree_node = tree_node;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
        return;
    }
    q->rear->next = node;
    q->rear = node;
}

TreeNode* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    TreeNode *tree_node = temp->tree_node;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size <= 0 || level_order[0] == INT_MIN) return NULL;

    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = root->right = NULL;

    Queue *q = create_queue();
    enqueue(q, root);

    int i = 1;
    while (!is_empty(q) && i < size) {
        TreeNode *current = dequeue(q);

        if (i < size && level_order[i] != INT_MIN) {
            TreeNode *left = (TreeNode*)malloc(sizeof(TreeNode));
            left->val = level_order[i];
            left->left = left->right = NULL;
            current->left = left;
            enqueue(q, left);
        }
        i++;

        if (i < size && level_order[i] != INT_MIN) {
            TreeNode *right = (TreeNode*)malloc(sizeof(TreeNode));
            right->val = level_order[i];
            right->left = right->right = NULL;
            current->right = right;
            enqueue(q, right);
        }
        i++;
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) return;
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

// Stack implementation for iterative preorder
typedef struct StackNode {
    TreeNode *tree_node;
    struct StackNode *next;
} StackNode;

static void push_stack(StackNode **top, TreeNode *tree_node) {
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    node->tree_node = tree_node;
    node->next = *top;
    *top = node;
}

static TreeNode* pop_stack(StackNode **top) {
    if (*top == NULL) return NULL;
    StackNode *temp = *top;
    TreeNode *tree_node = temp->tree_node;
    *top = (*top)->next;
    free(temp);
    return tree_node;
}

static int is_stack_empty(StackNode *top) {
    return top == NULL;
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) return;
    StackNode *stack = NULL;
    push_stack(&stack, root);

    while (!is_stack_empty(stack)) {
        TreeNode *current = pop_stack(&stack);
        printf("%d ", current->val);

        if (current->right != NULL)
            push_stack(&stack, current->right);
        if (current->left != NULL)
            push_stack(&stack, current->left);
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}