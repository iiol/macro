#include "list.h"

#define __list_extern_destroy(entry)			\
do {							\
	struct list_meta *meta;				\
							\
	meta = __list_get_meta(entry);			\
							\
	if (meta && meta->efuncs.destroy_node)		\
		meta->efuncs.destroy_node(entry);	\
	else if (list_extern_funcs.destroy_node)	\
		list_extern_funcs.destroy_node(entry);	\
} while (0)

#define __list_extern_cmp_nodes(a, b)					\
do {									\
	if (__list_get_meta(a).size == __list_get_meta(b).size) {	\
		struct list_meta *meta;					\
									\
		meta = __list_get_meta(a);				\
									\
		if (meta && meta->efuncs.cmp_nodes)			\
			meta->efuncs.cmp_nodes(a, b);			\
		else if (list_extern_funcs.cmp_nodes)			\
			list_extern_funcs.cmp_nodes(a, b);		\
	}								\
} while (0)

#define __list_extern_copy_node(to, from)		\
do {							\
	struct list_meta *meta;				\
							\
	meta = __list_get_meta(from);			\
							\
	if (meta && meta->efuncs.copy_node)		\
		meta->efuncs.copy_node(to, from);	\
	else if (list_extern_funcs.copy_node)		\
		list_extern_funcs.copy_node(to, from);	\
} while (0)

#define __list_get_node(entry) ((struct list_node*)(entry ? (uint8_t*)entry - sizeof (struct list_node) : NULL))
#define __list_get_meta(entry) ((struct list_meta*)(entry ? __list_get_node(entry)->meta : NULL))

static struct external_funcs list_extern_funcs;

inline static void*
__list_new(size_t size, struct list_meta *meta)
{
	uint8_t *p;
	struct list_node *hdr;

	hdr = malloc(size + sizeof (struct list_node));
	p = (uint8_t*)hdr + sizeof (struct list_node);

	if (meta == NULL) {
		meta = malloc(sizeof (struct list_meta));
		meta->head = hdr;
		meta->tail = hdr;
		meta->count = 1;
		meta->ent_size = size;
	}
	else
		++meta->count;

	hdr->meta = meta;
	hdr->prev = NULL;
	hdr->next = NULL;

	return(p);
}

inline static void*
__list_empty(size_t size)
{
	uint8_t *p;
	struct list_node *hdr;

	hdr = malloc(size + sizeof (struct list_node));
	p = (uint8_t*)hdr + sizeof (struct list_node);

	hdr->meta = malloc(sizeof (struct list_meta));
	hdr->meta->head = hdr;
	hdr->meta->tail = hdr;
	hdr->meta->count = 0;
	hdr->meta->ent_size = size;
	hdr->prev = NULL;
	hdr->next = NULL;

	return(p);
}

inline static void*
list_get_head(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;
	struct list_meta *meta;

	if (!entry)
		return(NULL);

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	return((uint8_t*)meta->head + sizeof (struct list_node));
}

inline static void*
list_get_tail(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;
	struct list_meta *meta;

	if (!entry)
		return(NULL);

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	return((uint8_t*)meta->tail + sizeof (struct list_node));
}

inline static int
list_get_count(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	if (!entry)
		return(0);

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));

	return(hdr->meta->count);
}

inline static void*
list_get_prev(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	if (!entry)
		return(NULL);

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));

	hdr = hdr->prev;
	if (hdr == NULL)
		p = NULL;
	else
		p = (uint8_t*)hdr + sizeof (struct list_node);

	return(p);
}

inline static void*
list_get_next(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	if (!entry)
		return(NULL);

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));

	hdr = hdr->next;
	if (hdr == NULL)
		p = NULL;
	else
		p = (uint8_t*)hdr + sizeof (struct list_node);

	return(p);
}

inline static void*
__list_alloc_after(void *entry, size_t size)
{
	uint8_t *new_p, *p;
	struct list_node *new_hdr, *hdr;
	struct list_meta *meta;

	if (__list_get_meta(entry)->count == 0) {
		__list_get_meta(entry)->count = 1;
		return(entry);
	}

	if (entry == NULL)
		return(__list_new(size, NULL));

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;
	new_p = __list_new(size, meta);
	new_hdr = (struct list_node*)(new_p - sizeof (struct list_node));

	if (hdr->next == NULL)
		meta->tail = new_hdr;
	else
		hdr->next->prev = new_hdr;

	new_hdr->meta = meta;
	new_hdr->next = hdr->next;
	new_hdr->prev = hdr;
	hdr->next = new_hdr;

	return(new_p);
}

inline static void*
__list_alloc_before(void *entry, size_t size)
{
	uint8_t *new_p, *p;
	struct list_node *new_hdr, *hdr;
	struct list_meta *meta;

	if (__list_get_meta(entry)->count == 0) {
		__list_get_meta(entry)->count = 1;
		return(entry);
	}

	if (entry == NULL)
		return(__list_new(size, NULL));

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;
	new_p = __list_new(size, meta);
	new_hdr = (struct list_node*)(new_p - sizeof (struct list_node));

	if (hdr->prev == NULL)
		meta->head = new_hdr;
	else
		hdr->prev->next = new_hdr;

	new_hdr->meta = meta;
	new_hdr->prev = hdr->prev;
	new_hdr->next = hdr;
	hdr->prev = new_hdr;

	return(new_p);
}

inline static void*
__list_alloc_at_end(void *entry, size_t size)
{
	if (entry == NULL)
		return(__list_new(size, NULL));

	if (__list_get_meta(entry)->count == 0) {
		__list_get_meta(entry)->count = 1;
		return(entry);
	}

	entry = list_get_tail(entry);

	return(__list_alloc_after(entry, size));

}

inline static void*
__list_alloc_at_start(void *entry, size_t size)
{
	if (entry == NULL)
		return(__list_new(size, NULL));

	if (__list_get_meta(entry)->count == 0) {
		__list_get_meta(entry)->count = 1;
		return(entry);
	}

	entry = list_get_head(entry);

	return(__list_alloc_before(entry, size));
}

/*
 * @desc		list_free -- free list entry
 * @arg[in]		entry     -- pointer to entry
 * @ret			pointer to head of list or NULL if deleted last entry
 */
inline static void*
list_free(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;
	struct list_meta *meta;

	if (entry == NULL)
		return(NULL);

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	if (hdr->prev == NULL)
		meta->head = hdr->next;
	else
		hdr->prev->next = hdr->next;

	if (hdr->next == NULL)
		meta->tail = hdr->prev;
	else
		hdr->next->prev = hdr->prev;

	p = NULL;
	--meta->count;

	if (hdr->meta->head != NULL)
		p = (uint8_t*)hdr->meta->head + sizeof (struct list_node);

	if (hdr->prev == NULL && hdr->next == NULL)
		free(meta);

	free(hdr);

	return(p);
}

/*
 * @desc		list_free_range -- free count entries from <fromth> element from head
 * @arg[in]		entry           -- pointer to any entry in list
 * @ret			pointer to head of list or NULL if deleted last entry
 */
inline static void*
list_free_range(void *entry, int from, int count)
{
	int i;
	void *next, *ret;

	for (i = 0; entry != NULL; ++i, entry = next) {
		if (i < from || i >= from + count)
			continue;

		next = list_get_next(entry);
		ret = list_free(entry);
	}

	return(ret);
}

/*
 * @desc		list_free_full -- free full list
 * @arg[in]		entry          -- pointer to any entry in list
 * @ret			void
 * TODO: rewrite
 */
inline static void
list_free_full(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;
	struct list_meta *meta;

	if (entry == NULL)
		return;

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	for (hdr = meta->head; hdr != NULL;) {
		p = (uint8_t*)hdr + sizeof (struct list_node);
		hdr = hdr->next;
		list_free(p);
	}
}

/*
 * @desc		list_destroy -- destroy entry from list
 * @arg[in]		entry        -- pointer to entry in list
 * @ret			void
 */
inline static void*
list_destroy(void *entry)
{
	__list_extern_destroy(entry);

	return(list_free(entry));
}

/*
 * @desc		list_destroy_range -- destroy count entries from <fromth> element from head
 * @arg[in]		entry              -- pointer to any entry in list
 * @ret			pointer to head of list or NULL if deleted last entry
 */
inline static void*
list_destroy_range(void *entry, int from, int count)
{
	int i;
	void *next, *ret;

	for (i = 0; entry != NULL; ++i, entry = next) {
		if (i < from || i >= from + count)
			continue;

		next = list_get_next(entry);
		__list_extern_destroy(entry);
		ret = list_free(entry);
	}

	return(ret);
}

/*
 * @desc		list_destroy_full -- destroy full list
 * @arg[in]		entry             -- pointer to any entry in list
 * @ret			void
 */
inline static void
list_destroy_full(void *entry)
{
	int i;
	void *next;

	for (i = 0, entry = list_get_head(entry); entry != NULL; ++i, entry = next) {
		next = list_get_next(entry);
		__list_extern_destroy(entry);
		list_free(entry);
	}
}

inline static void*
list_dup(void *list)
{
	struct list_meta *meta, *nmeta;
	struct list_node *nlist = NULL;
	size_t size;
	void *entry;

	if (!list)
		return(NULL);

	meta = __list_get_meta(list);
	size = meta->ent_size;

	list_foreach(list_get_head(list), entry) {
		nlist = list_alloc_at_end(nlist);
		memcpy(nlist, entry, size);
	}

	if (!nlist)
		return(NULL);

	nmeta = __list_get_meta(nlist);
	nmeta->ent_size = meta->ent_size;

	return(list_get_head(nlist));
}

inline static void*
list_clone(void *list)
{
	struct list_meta *meta, *nmeta;
	struct list_node *nlist = NULL;
	void *entry;

	if (!list)
		return(NULL);

	meta = __list_get_meta(list);

	list_foreach(list_get_head(list), entry) {
		nlist = list_alloc_at_end(nlist);
		__list_extern_copy_node(nlist, entry);
	}

	if (!nlist)
		return(NULL);

	nmeta = __list_get_meta(nlist);
	nmeta->ent_size = meta->ent_size;

	return(list_get_head(nlist));
}

inline static void*
list_reverse(void *list)
{
	void *entry;
	struct list_meta *meta;

	if (list == NULL)
		return(NULL);

	meta = __list_get_meta(list);
	SWAP(meta->head, meta->tail);

	list_foreach (list_get_head(list), entry) {
		struct list_node *node;

		node = __list_get_node(entry);
		SWAP(node->next, node->prev);
	}

	return(list_get_head(list));
}

inline static void*
list_merge(void *a, void *b, list_pos pos)
{
	size_t size;
	void *entry;

	if (!a || !b)
		return(list_get_head(!a ? b : a));

	if (pos == LIST_AFTER && !list_get_next(a))
		pos = LIST_AT_END;
	else if (pos == LIST_BEFORE) {
		void *e;

		e = list_get_prev(b);
		pos = LIST_AT_START;

		if (e) {
			pos = LIST_AFTER;
			b = e;
		}
	}

	if (__list_get_meta(a)->ent_size != __list_get_meta(b)->ent_size)
		return(NULL);

	size = __list_get_meta(a)->ent_size;

	switch (pos) {
	case LIST_AT_START:
		list_rofeach (list_get_tail(b), entry) {
			a = list_alloc_at_start(a);
			memcpy(a, entry, size);
		}
		break;

	case LIST_AT_END:
		list_foreach (list_get_head(b), entry) {
			a = list_alloc_at_end(a);
			memcpy(a, entry, size);
		}
		break;

	case LIST_AFTER:
		list_foreach (list_get_head(b), entry) {
			a = list_alloc_after(a);
			memcpy(a, entry, size);
		}
		break;

	// LIST_BEFORE unused
	case LIST_BEFORE:
	default:
		break;
	}

	list_free_full(b);

	return(list_get_head(a));
}

inline static void
list_setfunc_destroy(void (*destroy_node)(void *entry))
{
	list_extern_funcs.destroy_node = destroy_node;
}

inline static void
list_setlfunc_destroy(void *entry, void (*destroy_node)(void *entry))
{
	struct list_meta *meta;

	meta = __list_get_meta(entry);
	if (meta)
		meta->efuncs.destroy_node = destroy_node;
}

inline static void
list_setfunc_cmp(int (*cmp_nodes)(void *a, void *b))
{
	list_extern_funcs.cmp_nodes = cmp_nodes;
}

inline static void
list_setlfunc_cmp(void *entry, int (*cmp_nodes)(void *a, void *b))
{
	struct list_meta *meta;

	meta = __list_get_meta(entry);
	if (meta)
		meta->efuncs.cmp_nodes = cmp_nodes;
}

inline static void
list_setfunc_copy(void (*copy_node)(void *to, void *from))
{
	list_extern_funcs.copy_node = copy_node;
}

inline static void
list_setlfunc_copy(void *entry, void (*copy_node)(void *to, void *from))
{
	struct list_meta *meta;

	meta = __list_get_meta(entry);
	if (meta)
		meta->efuncs.copy_node = copy_node;
}
