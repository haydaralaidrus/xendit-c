/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "xendit_private.h"

void
xnd_sdk_init(void)
{ /** TODO: */ }

void
xnd_sdk_cleanup(void)
{ /** TODO: */ }

xnd_client_t *
xnd_client_new(const char *key)
{
	xnd_client_t *x;

	if (key == NULL || !key[0])
		return NULL;

	x = malloc(sizeof(xnd_client_t));
	if (x == NULL)
		return NULL;

	x->key = xnd_string_new(key);
	if (x->key == NULL) {
		free(x);
		return NULL;
	}

	return x;
}

void
xnd_client_destroy(xnd_client_t *x)
{
	if (x == NULL)
		return;

	xnd_string_destroy(&(x->key));
	free(x);
}
