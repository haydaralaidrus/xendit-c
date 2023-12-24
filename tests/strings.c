#include <stdlib.h>
#include <string.h>

#include "strings.h"

static int
test_xnd_string_new_destroy(void)
{
	xnd_string_t *s;

	/** test empty string */
	s = xnd_string_new("");
	if (s == NULL)
		return 0;
	if (s->size != 0UL)
		return 0;
	if (s->capacity != 15UL)
		return 0;
	xnd_string_destroy(&s);
	if (s != NULL)
		return 0;

	/** test initial capacity */
	s = xnd_string_new("abcdefghijklmno");
	if (s == NULL)
		return 0;
	if (s->size != 15UL)
		return 0;
	if (s->capacity != 15UL)
		return 0;
	xnd_string_destroy(&s);
	if (s != NULL)
		return 0;

	/** test exceeding initial capacity */
	s = xnd_string_new("abcdefghijklmnopqrst");
	if (s == NULL)
		return 0;
	if (s->size != strlen("abcdefghijklmnopqrst"))
		return 0;
	if (s->capacity != strlen("abcdefghijklmnopqrst"))
		return 0;
	xnd_string_destroy(&s);
	if (s != NULL)
		return 0;

	return 1;
}

static int
test_xnd_string_insert(void)
{
	xnd_string_t *s;

	/** test concatenate */
	s = xnd_string_new("abcde");
	if (s == NULL)
		return 0;
	if (xnd_string_insert(&s, "fghij", s->size) != 0)
		return 0;
	if (strcmp(s->data, "abcdefghij") != 0)
		return 0;
	xnd_string_destroy(&s);

	/** test prepending string */
	s = xnd_string_new("abcde");
	if (s == NULL)
		return 0;
	if (xnd_string_insert(&s, "fghij", 0UL) != 0)
		return 0;
	if (strcmp(s->data, "fghijabcde") != 0)
		return 0;
	xnd_string_destroy(&s);

	/** test string string in the middle */
	s = xnd_string_new("abcdef");
	if (s == NULL)
		return 0;
	if (xnd_string_insert(&s, "xyz", 2UL) != 0)
		return 0;
	if (strcmp(s->data, "abxyzcdef") != 0)
		return 0;
	xnd_string_destroy(&s);

	/** test exceed current capacity */
	s = xnd_string_new("abcde");
	if (s == NULL)
		return 0;
	if (xnd_string_insert(&s, "fghijklmnopqrstuvwxy", s->size) != 0)
		return 0;
	if (s->size != 25UL)
		return 0;
	if (s->capacity != 30UL)
		return 0;
	xnd_string_destroy(&s);

	/** test exceed next growth capacity */
	s = xnd_string_new("abcde");
	if (s == NULL)
		return 0;
	if (xnd_string_insert(&s, "fghijklmnopqrstuvwxyz123456789", s->size) != 0)
		return 0;
	if (s->size != 35UL)
		return 0;
	if (s->capacity != 35UL)
		return 0;
	/** then, test insert another string */
	if (xnd_string_insert(&s, "xyz", s->size) != 0)
		return 0;
	if (s->size != 38UL)
		return 0;
	if (s->capacity != 70UL)
		return 0;
	/** then, test inserting out-of-bound */
	if (xnd_string_insert(&s, "xyz", s->size + 1) != -1)
		return 0;
	if (strcmp(s->data, "abcdefghijklmnopqrstuvwxyz123456789xyz") != 0)
		return 0;
	if (s->size != 38UL)
		return 0;
	if (s->capacity != 70UL)
		return 0;
	xnd_string_destroy(&s);


	return 1;
}

static int
test_xnd_string_append(void)
{
	xnd_string_t *s;

	s = xnd_string_new("abcdefghijklmn");
	if (s == NULL)
		return 0;

	/** test appending without exceeding current capacity */
	xnd_string_append(&s, 'o');
	if (strcmp(s->data, "abcdefghijklmno") != 0)
		return 0;
	if (s->size != 15UL)
		return 0;
	if (s->capacity != 15UL)
		return 0;

	/** test exceeding current capacity */
	xnd_string_append(&s, 'p');
	if (strcmp(s->data, "abcdefghijklmnop") != 0)
		return 0;
	if (s->size != 16UL)
		return 0;
	if (s->capacity != 30UL)
		return 0;
	xnd_string_destroy(&s);

	return 1;
}

static int
test_xnd_string_clear(void)
{
	xnd_string_t *s;

	s = xnd_string_new("abcdefghijklomnopqrstuvwxyz123");
	if (s == NULL)
		return 0;

	/** test clearing and compare */
	xnd_string_clear(&s);
	if (strcmp(s->data, "") != 0)
		return 0;
	if (s->size != 0UL)
		return 0;
	if (s->capacity != 30UL)
		return 0;

	/** test insert after clearing */
	xnd_string_insert(&s, "abcde", 0UL);
	if (strcmp(s->data, "abcde") != 0)
		return 0;
	if (s->size != 5UL)
		return 0;
	if (s->capacity != 30UL)
		return 0;
	xnd_string_destroy(&s);

	return 1;
}

static int
test_xnd_string_zeroize(void)
{
	xnd_string_t *s;

	s = xnd_string_new("abcde");
	if (s == NULL)
		return 0;

	/** test zeroizing the whole capacity */
	xnd_string_zeroize(&s);
	for (size_t i = 0UL; i < s->capacity; ++i)
		if (s->data[i] != '\0')
			return 0;
	xnd_string_destroy(&s);

	return 1;
}

int
main(void)
{
	if (! test_xnd_string_new_destroy())
		exit(EXIT_FAILURE);
	if (! test_xnd_string_insert())
		exit(EXIT_FAILURE);
	if (! test_xnd_string_append())
		exit(EXIT_FAILURE);
	if (! test_xnd_string_clear())
		exit(EXIT_FAILURE);
	if (! test_xnd_string_zeroize())
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}
