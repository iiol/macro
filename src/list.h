#ifndef _MACRO_LIST_H
#define _MACRO_LIST_H

#include <stdbool.h>

#include "includes.h"
#include "misc.h"

struct external_funcs {
	void (*destroy_node)(void *entry);
	int (*cmp_nodes)(void *a, void *b);
	void (*copy_node)(void *to, void *from);
};

struct list_meta {
	struct list_node *head, *tail;
	int count;
	size_t ent_size;
	struct external_funcs efuncs;
};

struct list_node {
	struct list_meta *meta;
	struct list_node *prev, *next;
	uint8_t entry[0];
};

#define list_new(type) __list_new(sizeof (*type), NULL)

#define list_alloc_after(entry) __list_alloc_after(entry, sizeof (typeof (*entry)))
#define list_alloc_before(entry) __list_alloc_before(entry, sizeof (typeof (*entry)))
#define list_alloc_at_end(entry) __list_alloc_at_end(entry, sizeof (typeof (*entry)))
#define list_alloc_at_start(entry) __list_alloc_at_start(entry, sizeof (typeof (*entry)))

#define list_foreach(start, entry) for (entry = start; entry != NULL; entry = list_get_next(entry))
#define list_rofeach(start, entry) for (entry = start; entry != NULL; entry = list_get_prev(entry))
#define list_foreachi(start, entry, i) for (entry = start, i = 0; entry != NULL; entry = list_get_next(entry), ++i)
#define list_rofeachi(start, entry, i) for (entry = start, i = 0; entry != NULL; entry = list_get_prev(entry), ++i)

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

typedef enum list_pos {
	LIST_AFTER,
	LIST_BEFORE,
	LIST_AT_START,
	LIST_AT_END,
} list_pos;

inline static void* __list_new(size_t size, struct list_meta *meta);

inline static void* list_get_head(void *entry);
inline static void* list_get_tail(void *entry);
inline static void* list_get_prev(void *entry);
inline static void* list_get_next(void *entry);

inline static void* __list_alloc_after(void *entry, size_t size);
inline static void* __list_alloc_before(void *entry, size_t size);
inline static void* __list_alloc_at_end(void *entry, size_t size);
inline static void* __list_alloc_at_start(void *entry, size_t size);

inline static void* list_free(void *entry);
inline static void* list_free_range(void *entry, int from, int count);
inline static void  list_free_full(void *entry);

inline static void* list_dup(void *list);
inline static void* list_clone(void *list);

inline static void* list_reverse(void *list);
inline static void* list_merge(void *a, void *b, list_pos pos);

inline static void* list_destroy(void *entry);
inline static void* list_destroy_range(void *entry, int from, int count);
inline static void  list_destroy_full(void *entry);

inline static void  list_setfunc_destroy(void (*destroy_node)(void *entry));
inline static void  list_setfunc_cmp(int (*cmp_nodes)(void *a, void *b));
inline static void  list_setfunc_copy(void (*copy_node)(void *to, void *from));

inline static void  list_setlfunc_destroy(void *entry, void (*destroy_node)(void *entry));
inline static void  list_setlfunc_cmp(void *entry, int (*cmp_nodes)(void *a, void *b));
inline static void  list_setlfunc_copy(void *entry, void (*copy_node)(void *to, void *from));

#endif // _MACRO_LIST_H
