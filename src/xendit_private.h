/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef XND_XENDIT_PRIVATE_H
#define XND_XENDIT_PRIVATE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "strings.h"
#include "xendit.h"

struct xnd_client_t {
	xnd_string_t *key; /** The secret API key. */
};

#ifdef __cplusplus
}
#endif

#endif
