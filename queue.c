#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue_info {
    struct list_head head;
    int size;
};

/* Create an empty queue */
struct list_head *q_new()
{
    struct queue_info *q = malloc(sizeof(struct queue_info));
    if (!q)
        return NULL;
    INIT_LIST_HEAD(&q->head);
    q->size = 0;
    return &q->head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct queue_info *q = container_of(head, struct queue_info, head);
    struct list_head *pos, *tmp;
    list_for_each_safe (pos, tmp, head) {
        element_t *entry = list_entry(pos, element_t, list);
        free(entry->value);
        free(entry);
    }
    free(q);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    struct queue_info *q = container_of(head, struct queue_info, head);
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;

    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }

    list_add(&new->list, head);
    q->size++;
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    struct queue_info *q = container_of(head, struct queue_info, head);
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;

    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }

    list_add_tail(&new->list, head);
    q->size++;
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    struct queue_info *q = container_of(head, struct queue_info, head);
    if (!head || list_empty(head))
        return NULL;

    element_t *item = list_first_entry(head, element_t, list);
    if (sp && bufsize > 0) {
        strncpy(sp, item->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(&item->list);
    q->size--;
    return item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    struct queue_info *q = container_of(head, struct queue_info, head);
    if (!head || list_empty(head))
        return NULL;

    element_t *item = list_last_entry(head, element_t, list);
    if (sp && bufsize > 0) {
        strncpy(sp, item->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(&item->list);
    q->size--;
    return item;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    const struct queue_info *q = container_of(head, struct queue_info, head);
    return q->size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    struct queue_info *q = container_of(head, struct queue_info, head);
    if (!head || list_empty(head))
        return false;

    struct list_head *slow = head->next;
    struct list_head *fast = head->next;

    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    element_t *mid = list_entry(slow, element_t, list);
    list_del(slow);
    free(mid->value);
    free(mid);
    q->size--;
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return false;
    }

    element_t *curr = NULL, *next = NULL;
    bool check = false;
    list_for_each_entry_safe (curr, next, head, list) {
        if (&next->list != head && !strcmp(curr->value, next->value)) {
            list_del_init(&curr->list);
            q_release_element(curr);
            check = true;
        } else if (check) {
            list_del_init(&curr->list);
            q_release_element(curr);
            check = false;
        }
    }
    return true;
}


/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *node;
    list_for_each (node, head) {
        if (node->next == head)
            break;
        list_move(node, node->next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *current = head, *prev = head->prev, *next = NULL;
    do {
        next = current->next;
        current->next = prev;
        current->prev = next;
        prev = current;
        current = next;
    } while (current != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || k <= 1)
        return;

    struct list_head *current = head->next, *prev = head;

    while (current != head) {
        struct list_head *start = prev->next;
        int count = 0;  // Move count declaration inside the loop
        while (count < k && current != head) {
            current = current->next;
            count++;
        }
        if (count == k) {
            struct list_head *end = current->prev;
            struct list_head *p = start, *n, *tmp;
            while (p != current) {
                n = p->next;
                tmp = p->prev;
                p->prev = p->next;
                p->next = tmp;
                p = n;
            }
            prev->next = end;
            end->prev = prev;
            start->next = current;
            current->prev = start;
            prev = start;
        }
    }
}


/* Sort elements of queue in ascending/descending order */
struct list_head *merge(struct list_head *left,
                        struct list_head *right,
                        bool descend)
{
    struct list_head dummy;
    INIT_LIST_HEAD(&dummy);
    struct list_head *tail = &dummy;

    while (left && right) {
        const element_t *l = list_entry(left, element_t, list);
        const element_t *r = list_entry(right, element_t, list);
        if ((strcmp(l->value, r->value) <= 0) ^ descend) {
            tail->next = left;
            left->prev = tail;
            left = left->next;
        } else {
            tail->next = right;
            right->prev = tail;
            right = right->next;
        }
        tail = tail->next;
    }

    tail->next = left ? left : right;
    if (tail->next)
        tail->next->prev = tail;

    return dummy.next;
}

struct list_head *merge_sort(struct list_head *head, bool descend)
{
    if (!head || !head->next)
        return head;

    struct list_head *slow = head, *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct list_head *mid = slow->next;
    slow->next = NULL;

    struct list_head *left = merge_sort(head, descend);
    struct list_head *right = merge_sort(mid, descend);

    return merge(left, right, descend);
}

void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *first = head->next;
    struct list_head *last = head->prev;

    last->next = NULL;
    first->prev = NULL;

    struct list_head *sorted = merge_sort(first, descend);

    head->next = sorted;
    sorted->prev = head;

    struct list_head *tail = sorted;
    while (tail->next)
        tail = tail->next;
    tail->next = head;
    head->prev = tail;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur = head->prev;
    const element_t *cur_e = list_entry(cur, element_t, list);
    while (cur != head) {
        struct list_head *prev = cur->prev;
        if (prev == head)
            break;
        element_t *prev_e = list_entry(prev, element_t, list);
        if (strcmp(prev_e->value, cur_e->value) > 0) {
            list_del(prev);
            free(prev_e->value);
            free(prev_e);
            struct queue_info *q = container_of(head, struct queue_info, head);
            q->size--;
        } else {
            cur = prev;
            cur_e = prev_e;
        }
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur = head->prev;
    const element_t *cur_e = list_entry(cur, element_t, list);
    while (cur != head) {
        struct list_head *prev = cur->prev;
        if (prev == head)
            break;
        element_t *prev_e = list_entry(prev, element_t, list);
        if (strcmp(prev_e->value, cur_e->value) < 0) {
            list_del(prev);
            free(prev_e->value);
            free(prev_e);
            struct queue_info *q = container_of(head, struct queue_info, head);
            q->size--;
        } else {
            cur = prev;
            cur_e = prev_e;
        }
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
