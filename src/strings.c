/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <string.h>

#include "strings.h"

/** The dynamic string's initial capacity. */
#define XND_STRING_INITIAL_CAPACITY (15UL)

/** The dynamic string's growth factor. */
#define XND_STRING_GROWTH (2UL)

/** Resizes the capacity of a dynamic string according to the hinted new
    size. */
static int
xnd_string_resize(xnd_string_t **s, size_t hint_size);

xnd_string_t *
xnd_string_new(const char *str)
{
	xnd_string_t *s;
	size_t size, capacity;

	if (str == NULL)
		size = 0UL;
	else
		size = strlen(str);

	if (size <= XND_STRING_INITIAL_CAPACITY)
		capacity = XND_STRING_INITIAL_CAPACITY;
	else
		capacity = size;

	/** HACK: temporary struct hack. */
	s = malloc(sizeof(xnd_string_t) + (sizeof(char) * (capacity + 1UL)));
	if (s == NULL)
		return NULL;

	memcpy(s->data, str, size);
	s->data[size] = '\0';
	s->size = size;
	s->capacity = capacity;

	return s;
}

void
xnd_string_destroy(xnd_string_t **s)
{
	if (s == NULL || *s == NULL)
		return;

	free(*s);
	*s = NULL;
}

int
xnd_string_insert(xnd_string_t **s, const char *str, const size_t index)
{
	size_t len, newsz;

	if (s == NULL || *s == NULL)
		return -1;

	if (str == NULL || !str[0])
		return 0; /** empty string, return immediately */

	if (index > (*s)->size)
		return -1; /** out of bound */

	len = strlen(str);
	newsz = (*s)->size + len;

	if (xnd_string_resize(s, newsz) == -1)
		return -1;

	memmove(
		(*s)->data + index + len,
		(*s)->data + index,
		((*s)->size - index) + 1
	);

	memcpy((*s)->data + index, str, len);

	(*s)->size = newsz;

	return 0;
}

int
xnd_string_sized_insert(xnd_string_t **s, const char *str, const size_t index,
                        size_t size)
{
	size_t len, newsz;

	if (s == NULL || *s == NULL)
		return -1;

	if (str == NULL || !str[0])
		return 0; /** empty string, return immediately */

	if (index > (*s)->size)
		return -1; /** out of bound */

	len = strlen(str);
	if (len > size)
		len = size;

	newsz = (*s)->size + len;

	if (xnd_string_resize(s, newsz) == -1)
		return -1;

	memmove(
		(*s)->data + index + len,
		(*s)->data + index,
		((*s)->size - index) + 1
	);

	memcpy((*s)->data + index, str, len);

	(*s)->size = newsz;

	return 0;
}

int
xnd_string_append(xnd_string_t **s, const char c)
{
	if (s == NULL || *s == NULL)
		return -1;

	if (xnd_string_resize(s, (*s)->size + 1) == -1)
		return -1;

	(*s)->data[(*s)->size] = c;
	(*s)->data[(*s)->size + 1] = '\0';
	++((*s)->size);

	return 0;
}

int
xnd_string_clear(xnd_string_t **s)
{
	if (s == NULL || *s == NULL)
		return -1;

	(*s)->data[0] = '\0';
	(*s)->size = 0UL;

	return 0;
}

int
xnd_string_zeroize(xnd_string_t **s)
{
	if (s == NULL || *s == NULL)
		return -1;

	memset((*s)->data, 0, (*s)->capacity);
	(*s)->size = 0UL;

	return 0;
}

static int
xnd_string_resize(xnd_string_t **s, size_t hint_size)
{
	xnd_string_t *tmp;
	size_t newcap;

	if (hint_size <= (*s)->capacity)
		return 0; /** No resizing needed */

	if (hint_size > ((*s)->capacity * XND_STRING_GROWTH))
		newcap = hint_size;
	else
		newcap = (*s)->capacity * XND_STRING_GROWTH;

	/** HACK: temporary struct hack. */
	tmp = realloc(*s, sizeof(xnd_string_t) + (sizeof(char) * (newcap + 1UL)));
	if (tmp == NULL)
		return -1;

	*s = tmp;
	(*s)->capacity = newcap;

	return 0;
}
