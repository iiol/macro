#include "list.h"

inline static void*
__list_new(size_t size, struct list_meta *meta)
{
	uint8_t *p;
	struct list_node *hdr;

	hdr = malloc(size + sizeof (struct list_node));
	p = (uint8_t*)hdr + sizeof (struct list_node);

	if (meta == NULL) {
		hdr->meta = malloc(sizeof (struct list_meta));
		hdr->meta->head = hdr;
		hdr->meta->tail = hdr;
		hdr->meta->node_size = size;
	}
	else
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

	assert(entry && "Argument is NULL");

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

	assert(entry && "Argument is NULL");

	p = entry;
	hdr = (struct list_node*)(p - sizeof (struct list_node));
	meta = hdr->meta;

	return (uint8_t*)meta->tail + sizeof (struct list_node);
}

inline static void*
list_get_prev(void *entry)
{
	uint8_t *p;
	struct list_node *hdr;

	assert(entry && "Argument is NULL");

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

	assert(entry && "Argument is NULL");

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
__list_alloc_next(void *entry, size_t size)
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
__list_alloc_prev(void *entry, size_t size)
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

	return __list_alloc_next(entry, size);

}

inline static void*
__list_alloc_at_start(void *entry, size_t size)
{
	if (entry == NULL)
		return __list_new(size, NULL);

	entry = list_get_head(entry);

	return __list_alloc_prev(entry, size);
}

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

	if (hdr->meta->head != NULL)
		p = (uint8_t*)hdr->meta->head + sizeof (struct list_node);

	if (hdr->prev == NULL && hdr->next == NULL)
		free(meta);

	free(hdr);

	return p;
}

inline static void
list_destroy(void *entry)
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
