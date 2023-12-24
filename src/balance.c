/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "strings.h"
#include "http_request.h"
#include "xendit_private.h"

int
xnd_balance(const xnd_client_t *x, const char *for_user_id,
            const char *account_type, const char *currency,
            xnd_balance_t *response)
{
	(void) response; /** HACK: temporarily diffuse compiler warning */

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

	xnd_string_destroy(&res);
	xnd_http_request_destroy(req);

	return 0;
}
