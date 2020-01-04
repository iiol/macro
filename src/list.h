#ifndef _MACRO_LIST_H
#define _MACRO_LIST_H

#include "includes.h"

struct list_meta {
	struct list_node *head, *tail;
};

struct list_node {
	struct list_meta *meta;
	struct list_node *prev, *next;
};


#define xassert(var)							\
({									\
	typeof (var) __var = var;					\
									\
	if (!__var) {							\
		_int_logit_full(stderr, "[A] ", "%s", #var);		\
		exit(1);						\
	}								\
									\
	__var;								\
})

#define list_init(p)							\
({									\
	typeof (p) __p;							\
									\
	__p = xmalloc(sizeof (typeof (*__p)));				\
	memset(&__p->_list, 0, sizeof (struct list_node));		\
									\
	__p->_list.meta = xmalloc(sizeof (struct list_meta));		\
	__p->_list.meta->head = &__p->_list;				\
	__p->_list.meta->tail = &__p->_list;				\
	__p->_list.prev = NULL;						\
	__p->_list.next = NULL;						\
									\
	if (strcmp("NULL", #p) != 0)					\
		p = __p;						\
									\
	__p								\
})

#define list_alloc_at_end(p)						\
({									\
	typeof (p) __newp, __p = p;					\
									\
	if (__p) {							\
		__newp = xmalloc(sizeof (typeof (*__p)));		\
		__newp->_list.meta = __p->_list.meta;			\
		__newp->_list.prev = __p->_list.meta->tail;		\
		__newp->_list.next = NULL;				\
		__newp->_list.meta->tail = &(__newp->_list);		\
		if (__newp->_list.prev != NULL)				\
			__newp->_list.prev->next = &(__newp->_list);	\
	}								\
	else								\
 		list_init(__newp);					\
									\
	__newp;								\
})

#define list_delete(p)							\
({									\
	typeof (p) __head = NULL, __p = xassert(p);			\
	unsigned int __offst = offsetof(typeof (*__p), _list);		\
									\
	if (__p->_list.prev == NULL)					\
		__p->_list.meta->head = __p->_list.next;		\
	else								\
		__p->_list.prev->next = __p->_list.next;		\
									\
	if (__p->_list.next == NULL)					\
		__p->_list.meta->tail = __p->_list.prev;		\
	else								\
		__p->_list.next->prev = __p->_list.prev;		\
									\
	if (__p->_list.meta->head != NULL)				\
		__head = (void*)((int8_t*)__p->_list.meta->head - __offst); \
									\
	if (__p->_list.prev == NULL && __p->_list.next == NULL)		\
		free(__p->_list.meta);					\
									\
	free(__p);							\
									\
	__head;								\
})

#define list_get_prev(p)						\
({									\
	typeof (p) __ret = NULL, __p = xassert(p);			\
	unsigned int __offst = offsetof(typeof (*__p), _list);		\
									\
	if (__p->_list.prev != NULL)					\
		__ret = (void*)((int8_t*)__p->_list.prev - __offst);	\
									\
	__ret;								\
})

#define list_get_next(p)						\
({									\
	typeof (p) __ret = NULL, __p = xassert(p);			\
	unsigned int __offst = offsetof(typeof (*__p), _list);		\
									\
	if (__p->_list.next != NULL)					\
		__ret = (void*)((int8_t*)__p->_list.next - __offst);	\
									\
	__ret;								\
})

#define list_get_head(p)						\
({									\
	typeof (p) __ret = NULL, __p = xassert(p);			\
	unsigned int __offst = offsetof(typeof (*__p), _list);		\
									\
	if (__p->_list.meta->head != NULL)				\
		__ret = (void*)((int8_t*)__p->_list.meta->head - __offst); \
									\
	__ret;								\
})

#define list_get_tail(p)						\
({									\
	typeof (p) __ret = NULL, __p = xassert(p);			\
	unsigned int __offst = offsetof(typeof (*__p), _list);		\
									\
	if (__p->_list.meta->tail != NULL)				\
		__ret = (void*)((int8_t*)__p->_list.meta->tail - __offst); \
									\
	__ret;								\
})

#define list_find(p, var, val)						\
({									\
	typeof (p) __entry;						\
									\
	list_foreach (p, __entry)					\
		if (__entry->var == (val))				\
			break;						\
									\
	__entry;							\
})

#define list_foreach(head, entry) for (entry = head; entry != NULL; entry = list_get_next(entry))

#undef xassert

#endif // _MACRO_LIST_H
