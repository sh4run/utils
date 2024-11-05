/*
 * hashtable.h
 * An implementation of hash table.
 */

#ifndef _hashtable_h_
#define _hashtable_h_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

typedef struct {
    queue_elem_t  elem;
} hash_head_t;

typedef int (*hash_cb_t)(void *key);
typedef int (*compare_cb_t)(hash_head_t *node, void *key);

typedef struct {
    hash_cb_t     hash_cb;
    compare_cb_t  compare_cb;
    int           depth;
    queue_t       table[];
} hashtable_t;

static inline void hashtable_free(hashtable_t *t)
{
    free(t);
}

static inline hashtable_t *hashtable_init(int depth, 
                                          hash_cb_t hash_cb, 
                                          compare_cb_t compare_cb)
{
    hashtable_t *t;
    t = (hashtable_t*)malloc(sizeof(hashtable_t) + depth * sizeof(queue_t));
    if (t) {
        t->depth = depth;
        t->hash_cb = hash_cb;
        t->compare_cb = compare_cb;
        memset(t->table, 0, depth * sizeof(queue_t));
    }
    return t;
}

static inline void hashtable_add(hashtable_t* t, void *node, void *key)
{
    queue_elem_t *elem = (queue_elem_t*)node;
    assert(t && t->hash_cb && elem);

    int hash = t->hash_cb(key);
    assert(hash < t->depth);

    enqueue(&t->table[hash], elem);
}

static inline hash_head_t *hashtable_search(hashtable_t *t, void *key) 
{
    hash_head_t *elem;
    int hash = t->hash_cb(key);
    assert(hash < t->depth);

    foreach_queue(elem, hash_head_t*, &t->table[hash]) {
        if (!t->compare_cb(elem, key)) {
            return elem;
        }
    }
    return NULL;
}


/*
 *  DJB2 string hash
 */
static inline int string_hash(void *key)
{
    unsigned char *str = (unsigned char *)key;
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (int)(hash % 256);
}

#endif
