#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *Queue = malloc(sizeof(struct list_head));

    if (Queue == NULL)
        return NULL;

    INIT_LIST_HEAD(Queue);


    return Queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    struct list_head *temp;

    if (l == NULL) {
        return;
    }

    if (list_empty(l)) {
        // printf("l is already release");
        free(l);
        return;
    }

    if (list_is_singular(l)) {
        element_t *node = container_of(l->next, typeof(element_t), list);
        q_release_element(node);
        free(l);

        return;
    }

    for (temp = l->next; temp != l;) {
        element_t *node = container_of(temp, typeof(element_t), list);
        temp = temp->next;
        q_release_element(node);
    }

    free(l);
    return;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *Node = malloc(sizeof(element_t));
    char *c = malloc(strlen(s) + 1);



    if (Node == NULL || c == NULL)
        return false;

    memcpy(c, s, strlen(s) + 1);
    Node->value = c;
    INIT_LIST_HEAD(&Node->list);
    list_add(&Node->list, head);
    /*
    if (head->next == head || head->prev == head) {
        Node->value = c;
        head->next = &Node->list;
        head->prev = &Node->list;
        Node->list.prev = head;
        Node->list.next = head;
    } else {
        Node->value = c;
        Node->list.next = head->next;
        Node->list.prev = head;
        head->next->prev = &Node->list;
        head->next = &Node->list;
    }
    */


    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *Node = malloc(sizeof(element_t));

    char *c = malloc(strlen(s) + 1);
    // struct list_head *temp;

    if (Node == NULL || c == NULL)
        return false;

    memcpy(c, s, strlen(s) + 1);

    Node->value = c;
    INIT_LIST_HEAD(&Node->list);
    list_add_tail(&Node->list, head);
    /*
    if (head->next == head || head->prev == head) {
        Node->value = c;
        head->next = &Node->list;
        head->prev = &Node->list;
        Node->list.prev = head;
        Node->list.next = head;
    } else {
        Node->value = c;
        Node->list.next = head;
        Node->list.prev = head->prev;

        head->prev->next = &Node->list;
        head->prev = &Node->list;
    }
    */


    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }



    struct list_head *node = head->next;
    element_t *element;
    element = list_entry(node, typeof(element_t), list);



    if (sp) {
        memcpy(sp, element->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }



    list_del(node);
    /*
    if (strlen(element->value) <=0 || bufsize -1 <=0){
        *sp='\0';
    }

    else if (strlen(element->value) <= bufsize - 1)
        //sp = element->value;
        memcpy(sp, element->value, strlen(element->value)+1);
    else if(strlen(element->value) > bufsize - 1){
        memcpy(sp, element->value, bufsize-1);
        //sp = element->value;
        *(sp + bufsize-1) = '\0';
    }
    */
    // element->value = sp;
    /*
    sp=node->value;

    head->next = head->next->next;
    head->next->prev = head;

    node->list.next = &node->list;
    node->list.prev = &node->list;
    */
    return element;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }



    struct list_head *node = head->prev;
    element_t *element;
    element = list_entry(node, typeof(element_t), list);



    if (sp) {
        memcpy(sp, element->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }



    list_del(node);
    return element;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;


    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    if (head == NULL || list_empty(head))
        return false;
    else if (list_is_singular(head)) {
        q_remove_head(head, NULL, 100);
        return true;
    }


    struct list_head **indir = &head->next, *prev = NULL;
    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next) {
        prev = *indir;
        indir = &(*indir)->next;
    }
    struct list_head *next = (*indir)->next;
    // free(*indir);
    prev->next = next;



    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    if (list_is_singular(head) || list_empty(head))
        return;
    struct list_head *li;
    struct list_head *temp;

    temp = head->prev;
    head->prev = head->next;
    head->next = temp;

    list_for_each (li, head) {
        temp = li->prev;
        li->prev = li->next;
        li->next = temp;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head)
{
    element_t *begin[100], *end[100], *L, *R;
    element_t pivot;
    int i = 0;

    begin[0] = list_first_entry(head, element_t, list);
    end[0] = list_last_entry(head, element_t, list);

    while (i >= 0) {
        L = begin[i];
        R = end[i];

        if (L != R && &begin[i]->list != head) {
            pivot = *begin[i];
            if (i == 100 - 1) {
                return;
            }

            while (L != R) {
                while (*(R->value) >= *(pivot.value) && L != R)
                    R = list_entry(R->list.prev, element_t, list);
                if (L != R) {
                    L->value = R->value;
                    L = list_entry(L->list.next, element_t, list);
                }

                while (L->value <= pivot.value && L != R)
                    L = list_entry(L->list.next, element_t, list);
                if (L != R) {
                    R->value = L->value;
                    R = list_entry(R->list.prev, element_t, list);
                }
            }
            L->value = pivot.value;
            begin[i + 1] = list_entry(L->list.next, element_t, list);
            end[i + 1] = end[i];
            end[i++] = L;
        } else
            i--;
    }
}
