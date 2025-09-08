#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 循环数组队列（牺牲一格法）：front 指向队头元素，rear 指向下一个入队位置
typedef struct {
    int *a;
    int cap;    // 实际数组容量（可用容量 = cap - 1）
    int front;  // 队头元素下标
    int rear;   // 下一个入队位置
} CirQueue;

static bool cq_init(CirQueue* q, int capacity) {
    if (!q || capacity < 2) return false; // 至少 2，因牺牲一格
    q->a = (int*)malloc(sizeof(int) * capacity);
    if (!q->a) return false;
    q->cap = capacity;
    q->front = q->rear = 0;
    return true;
}

static void cq_destroy(CirQueue* q) {
    if (!q) return;
    free(q->a);
    q->a = NULL; q->cap = q->front = q->rear = 0;
}

static inline bool cq_is_empty(const CirQueue* q) {
    return q->front == q->rear;
}

static inline bool cq_is_full(const CirQueue* q) {
    return (q->rear + 1) % q->cap == q->front;
}

static inline int cq_size(const CirQueue* q) {
    return (q->rear - q->front + q->cap) % q->cap;
}

static bool cq_enqueue(CirQueue* q, int x) {
    if (cq_is_full(q)) return false;
    q->a[q->rear] = x;
    q->rear = (q->rear + 1) % q->cap;
    return true;
}

static bool cq_dequeue(CirQueue* q, int* out) {
    if (cq_is_empty(q)) return false;
    if (out) *out = q->a[q->front];
    q->front = (q->front + 1) % q->cap;
    return true;
}

static bool cq_front(const CirQueue* q, int* out) {
    if (cq_is_empty(q)) return false;
    if (out) *out = q->a[q->front];
    return true;
}

static void cq_print(const CirQueue* q, const char* tag) {
    printf("%s size=%d (cap=%d, usable=%d): ", tag, cq_size(q), q->cap, q->cap - 1);
    for (int i = 0, idx = q->front; i < cq_size(q); ++i) {
        printf("%d ", q->a[idx]);
        idx = (idx + 1) % q->cap;
    }
    printf("\n");
}

int main(void) {
    CirQueue q;
    if (!cq_init(&q, 6)) { // cap=6 -> usable=5
        printf("init failed\n");
        return 1;
    }

    // 入队 1..4
    for (int i = 1; i <= 4; ++i) {
        bool ok = cq_enqueue(&q, i);
        printf("enqueue %d -> %s\n", i, ok ? "OK" : "FULL");
    }
    cq_print(&q, "after enqueue 1..4");

    // 出队 2 个，演示队头前进
    for (int k = 0; k < 2; ++k) {
        int x; bool ok = cq_dequeue(&q, &x);
        printf("dequeue -> %s", ok ? "OK " : "EMPTY");
        if (ok) printf("(%d)", x);
        printf("\n");
    }
    cq_print(&q, "after dequeue x2");

    // 入队 5,6,7，触发索引环绕
    int more[] = {5,6,7};
    for (int i = 0; i < 3; ++i) {
        bool ok = cq_enqueue(&q, more[i]);
        printf("enqueue %d -> %s\n", more[i], ok ? "OK" : "FULL");
    }
    cq_print(&q, "after enqueue 5,6,7 (wrap) ");

    // 查看队头
    int f; if (cq_front(&q, &f)) printf("front=%d\n", f);

    // 全部出队
    printf("dequeue all: ");
    int x;
    while (cq_dequeue(&q, &x)) {
        printf("%d ", x);
    }
    printf("\n");
    cq_print(&q, "after dequeue all");

    cq_destroy(&q);
    return 0;
}
