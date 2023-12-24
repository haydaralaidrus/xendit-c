/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef XND_STRINGS_H
#define XND_STRINGS_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * \brief Dynamic string.
 */
typedef struct xnd_string_t {
	size_t  size;     /** Size of the string stored not including NTB. */
	size_t  capacity; /** Max size of the string stored not including NTB. */
	char    data[0];  /** Pointer to the string stored. */
} xnd_string_t;

/**
 * \brief Creates a new dynamic string.
 * \param str The initial string to be contained.
 * \return NULL on failure.
 */
extern xnd_string_t *
xnd_string_new(const char *str);

/**
 * \brief Destroys dynamic string.
 * \param s The dynamic string to destroy.
 */
extern void
xnd_string_destroy(xnd_string_t **s);

/**
 * \brief Inserts string at the specified index no more than provided size.
 * \param s The dynamic string to insert.
 * \param str The string to be inserted.
 * \param index The index at which the string should be inserted. If index is
 * equal to current size, the string is appended.
 * \param size The maximum number of bytes to be inserted.
 * \return 0 on successful insertion, -1 otherwise.
 */
extern int
xnd_string_sized_insert(xnd_string_t **s, const char *str, const size_t index,
                        size_t size);

/**
 * \brief Appends a single character to the end of the dynamic string.
 * \param s The dynamic string to append.
 * \param c The character to be appended.
 * \return 0 on successful appending, -1 otherwise.
 */
extern int
xnd_string_append(xnd_string_t **s, const char c);

/**
 * \brief Clears the contents of the dynamic string.
 * \param s The dynamic string to clear.
 * \return 0 on successful clearance, -1 otherwise.
 */
extern int
xnd_string_clear(xnd_string_t **s);

/**
 * \brief Zeroizes the contents of the dynamic string. This function zeroes out
 * not only the current size of the string but also the entire capacity of the
 * dynamic string, effectively zeroizing its contents.
 * \param s The dynamic string to be zeroed out.
 * \return 0 on successful zeroizing, -1 otherwise.
 */
extern int
xnd_string_zeroize(xnd_string_t **s);

/**
 * \brief Inserts string at the specified index.
 * \param S The dynamic string to insert.
 * \param Str The string to be inserted.
 * \param Index The index at which the string should be inserted. If index is
 * equal to current size, the string is appended.
 * \return 0 on successful insertion, -1 otherwise.
 */
#define xnd_string_insert(S, Str, I) \
        xnd_string_sized_insert(S, Str, I, __SIZE_MAX__)

#ifdef __cplusplus
}
#endif

#endif
