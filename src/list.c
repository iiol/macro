#include "list.h"

#define __list_extern_destroy(entry)			\
do {							\
	if (list_extern_funcs.destroy_node)		\
		list_extern_funcs.destroy_node(entry);	\
} while (0)

static struct external_funcs {
	void (*destroy_node)(void *entry);
} list_extern_funcs;

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
	}
	else
		++meta->count;

	hdr->meta = meta;
	hdr->prev = NULL;
	hdr->next = NULL;

	return p;
}

inline static void*
list_get_head(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;
	struct list_meta *meta;

	if (!entry)
		return NULL;

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	return (uint8_t*)meta->head + sizeof (struct list_node);
}

inline static void*
list_get_tail(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;
	struct list_meta *meta;

	if (!entry)
		return NULL;

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	return (uint8_t*)meta->tail + sizeof (struct list_node);
}

inline static int
list_get_count(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	if (!entry)
		return 0;

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));

	return hdr->meta->count;
}

inline static void*
list_get_prev(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	if (!entry)
		return NULL;

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));

	hdr = hdr->prev;
	if (hdr == NULL)
		p = NULL;
	else
		p = (uint8_t*)hdr + sizeof (struct list_node);

	return p;
}

inline static void*
list_get_next(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	if (!entry)
		return NULL;

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));

	hdr = hdr->next;
	if (hdr == NULL)
		p = NULL;
	else
		p = (uint8_t*)hdr + sizeof (struct list_node);

	return p;
}

inline static void*
__list_alloc_after(void *entry, size_t size)
{
	uint8_t *new_p, *p;
	struct list_node *new_hdr, *hdr;
	struct list_meta *meta;

	if (entry == NULL)
		return __list_new(size, NULL);

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

	return new_p;
}

inline static void*
__list_alloc_before(void *entry, size_t size)
{
	uint8_t *new_p, *p;
	struct list_node *new_hdr, *hdr;
	struct list_meta *meta;

	if (entry == NULL)
		return __list_new(size, NULL);

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

	return new_p;
}

inline static void*
__list_alloc_at_end(void *entry, size_t size)
{
	if (entry == NULL)
		return __list_new(size, NULL);

	entry = list_get_tail(entry);

	return __list_alloc_after(entry, size);

}

inline static void*
__list_alloc_at_start(void *entry, size_t size)
{
	if (entry == NULL)
		return __list_new(size, NULL);

	entry = list_get_head(entry);

	return __list_alloc_before(entry, size);
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
		return NULL;

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

	return p;
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

	return ret;
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

inline static void*
list_destroy(void *entry)
{
	__list_extern_destroy(entry);

	return list_free(entry);
}

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

	return ret;
}

inline static void
list_destroy_full(void *entry)
{
	int i;
	void *next;

	for (i = 0; entry != NULL; ++i, entry = next) {
		next = list_get_next(entry);
		__list_extern_destroy(entry);
		list_free(entry);
	}
}

inline static void
list_setfunc_destroy(void (*destroy_node)(void *entry))
{
	list_extern_funcs.destroy_node = destroy_node;
}
