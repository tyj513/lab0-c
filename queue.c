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
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
