/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <json-c/json.h>

#include "strings.h"
#include "http_request.h"
#include "xendit_private.h"

/** Binds JSON string response to Xendit balance object. */
static int
xnd_balance_bind(const char *jsonstr, xnd_balance_t **balance);

int
xnd_balance(const xnd_client_t *x, const char *for_user_id,
            const char *account_type, const char *currency,
            xnd_balance_t *response)
{
	xnd_http_request_t *req;
	xnd_string_t *res;

	if (x == NULL)
		return -1;

	req = xnd_http_request_new(XND_HTTP_REQUEST_GET, XND_ENDPOINT_BALANCE);
	if (req == NULL)
		return -1;

	res = xnd_string_new(NULL);
	if (res == NULL) {
		xnd_http_request_destroy(req);
		return -1;
	}

	/** Headers */
	xnd_http_request_header(req, "Content-Type", "application/json");
	if (for_user_id != NULL && for_user_id[0])
		xnd_http_request_header(req, "for-user-id", for_user_id);

	/** Query params */
	if (account_type != NULL && account_type[0])
		xnd_http_request_query(req, "account_type", account_type);
	if (currency != NULL && currency[0])
		xnd_http_request_query(req, "currency", currency);

	/** Callback */
	xnd_http_request_callback(req, xnd_http_request_default_callback);

	/** Basic auth */
	xnd_http_request_basic_auth(req, x->key->data, NULL);

	/** Send request */
	xnd_http_request_send_with_data(req, (void *) &res);

	/** Bind JSON response */
	xnd_balance_bind(res->data, &response);

	xnd_string_destroy(&res);
	xnd_http_request_destroy(req);

	return 0;
}

static int
xnd_balance_bind(const char *jsonstr, xnd_balance_t **balance)
{
	json_object *root;
	json_object *balance_obj = NULL;

	if (jsonstr == NULL || !jsonstr[0])
		return -1;

	root = json_tokener_parse(jsonstr);
	json_object_object_get_ex(root, "balance", &balance_obj);
	(*balance)->balance = json_object_get_double(balance_obj);
	json_object_put(root);

	return 0;
}
