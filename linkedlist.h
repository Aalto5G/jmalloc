#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stddef.h>

struct linked_list_node {
  struct linked_list_node *prev;
  struct linked_list_node *next;
};

struct linked_list_head {
  struct linked_list_node node;
};

static inline void linked_list_node_init(struct linked_list_node *node)
{
  node->prev = 0;
  node->next = 0;
}

static inline void linked_list_head_init(struct linked_list_head *head)
{
  head->node.prev = &head->node;
  head->node.next = &head->node;
}


static inline void linked_list_add_tail(
  struct linked_list_node *node, struct linked_list_head *head)
{
  node->next = &head->node;
  node->prev = head->node.prev;
  head->node.prev->next = node;
  head->node.prev = node;
}

static inline void linked_list_add_head(
  struct linked_list_node *node, struct linked_list_head *head)
{
  node->prev = &head->node;
  node->next = head->node.next;
  head->node.next->prev = node;
  head->node.next = node;
}

static inline void linked_list_add_before(
  struct linked_list_node *newnode, struct linked_list_node *oldnode)
{
  newnode->prev = oldnode->prev;
  newnode->next = oldnode;
  oldnode->prev->next = newnode;
  oldnode->prev = newnode;
}

static inline void linked_list_add_after(
  struct linked_list_node *newnode, struct linked_list_node *oldnode)
{
  newnode->prev = oldnode;
  newnode->next = oldnode->next;
  oldnode->next->prev = newnode;
  oldnode->next = newnode;
}

static inline int linked_list_is_empty(struct linked_list_head *head)
{
  return head->node.next == &head->node;
}

static inline void linked_list_delete(struct linked_list_node *node)
{
  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->prev = 0;
  node->next = 0;
}

#define LINKED_LIST_FOR_EACH(iter, head) \
  for (iter = (head)->node.next; iter != &(head)->node; iter = iter->next)

#define LINKED_LIST_FOR_EACH_REVERSE(iter, head) \
  for (iter = (head)->node.prev; iter != &(head)->node; iter = iter->prev)

#define LINKED_LIST_FOR_EACH_SAFE(iter, tmp, head) \
  for (iter = (head)->node.next, tmp = iter->next; \
       iter != &(head)->node; \
       iter = tmp, tmp = iter->next)

#define LINKED_LIST_FOR_EACH_REVERSE_SAFE(iter, tmp, head) \
  for (iter = (head)->node.prev, tmp = iter->prev; \
       iter != &(head)->node; \
       iter = tmp, tmp = iter->prev)

static inline size_t linked_list_size(struct linked_list_head *head)
{
  struct linked_list_node *node;
  size_t sz = 0;
  LINKED_LIST_FOR_EACH(node, head)
  {
    sz++;
  }
  return sz;
}

#endif
