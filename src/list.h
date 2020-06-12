#ifndef _MACRO_LIST_H
#define _MACRO_LIST_H

#include "includes.h"
#include "misc.h"

struct list_meta {
	struct list_node *head, *tail;
	int count;
};

struct list_node {
	struct list_meta *meta;
	struct list_node *prev, *next;
};


#define list_new(type) __list_new(sizeof (*type), NULL)
#define list_alloc_next(entry) __list_alloc_next(entry, sizeof (typeof (*entry)))
#define list_alloc_prev(entry) __list_alloc_prev(entry, sizeof (typeof (*entry)))
#define list_alloc_at_end(entry) __list_alloc_at_end(entry, sizeof (typeof (*entry)))
#define list_alloc_at_start(entry) __list_alloc_at_start(entry, sizeof (typeof (*entry)))
#define list_foreach(start, entry) for (entry = start; entry != NULL; entry = list_get_next(entry))

#define list_search_by_elem(entry, elem, value)				\
({									\
	typeof (entry) __p;						\
									\
	for (__p = entry; __p != NULL; __p = list_get_next(__p))	\
		if (__p->elem == (value))				\
			break;						\
									\
	__p;								\
})

#define list_search_by_str(entry, elem, str)				\
({									\
	typeof (entry) __p;						\
									\
	for (__p = entry; __p != NULL; __p = list_get_next(__p))	\
		if (!strcmp(__p->elem, str))				\
			break;						\
									\
	__p;								\
})

inline static void* __list_new(size_t size, struct list_meta *meta);
inline static void* list_get_head(void *entry);
inline static void* list_get_tail(void *entry);
inline static void* list_get_prev(void *entry);
inline static void* list_get_next(void *entry);
inline static void* __list_alloc_next(void *entry, size_t size);
inline static void* __list_alloc_prev(void *entry, size_t size);
inline static void* __list_alloc_at_end(void *entry, size_t size);
inline static void* __list_alloc_at_start(void *entry, size_t size);
inline static void* list_free(void *entry);
inline static void  list_destroy(void *entry);

#endif // _MACRO_LIST_H
