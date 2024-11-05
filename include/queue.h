/*
 * A simple implementation of queue using linked list.
 */

#ifndef _queue_h_
#define _queue_h_


typedef struct _queue_elem {
    struct _queue_elem *next;
} queue_elem_t;

typedef struct queue {
    queue_elem_t *head;
    queue_elem_t *tail;
} queue_t;

static inline void init_queue(queue_t *q)
{
    q->head = q->tail = NULL;
}

static inline int is_empty(queue_t *q)
{
    return (q->head == NULL);
}

static inline void enqueue(queue_t *q, queue_elem_t *node)
{
    node->next = NULL;
    if (is_empty(q)) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
}

static inline queue_elem_t *dequeue(queue_t *q)
{
    queue_elem_t *node = q->head;
    if (node) {
        q->head = node->next;
        if (q->head == NULL) {
            q->tail = NULL;
        }
    }
    return node;
}

#define queue_head(que) ((que)->head)
#define queue_tail(que) ((que)->tail)

#define foreach_queue(elem, type, que) \
            for(elem = ((type)((que) ? (que)->head : NULL)); \
                elem != NULL; \
                elem = ((type)(((queue_elem_t*)(elem))->next)))

#endif
