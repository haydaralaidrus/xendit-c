/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>

#include "http_request.h"

const char *const XND_HTTP_REQUEST_GET     = "GET";
const char *const XND_HTTP_REQUEST_HEAD    = "HEAD";
const char *const XND_HTTP_REQUEST_POST    = "POST";
const char *const XND_HTTP_REQUEST_PUT     = "PUT";
const char *const XND_HTTP_REQUEST_CONNECT = "CONNECT";
const char *const XND_HTTP_REQUEST_OPTIONS = "OPTIONS";
const char *const XND_HTTP_REQUEST_TRACE   = "TRACE";
const char *const XND_HTTP_REQUEST_PATCH   = "PATCH";

void
xnd_http_request_init(void)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

void
xnd_http_request_cleanup(void)
{
	curl_global_cleanup();
}

xnd_http_request_t *
xnd_http_request_new(const char *method, const char *baseurl)
{
	xnd_http_request_t *req;

	if (baseurl == NULL || !baseurl[0])
		return NULL;

	req = malloc(sizeof(xnd_http_request_t));
	if (req == NULL)
		return NULL;

	req->url = xnd_string_new(baseurl);
	if (req->url == NULL) {
		free(req);
		return NULL;
	}

	req->curl = curl_easy_init();
	if (req->curl == NULL) {
		xnd_string_destroy(&(req->url));
		free(req);
		return NULL;
	}

	curl_easy_setopt(req->curl, CURLOPT_CUSTOMREQUEST, method);
	req->method  = method;
	req->queries = NULL;
	req->headers = NULL;
	req->cb      = NULL;

	return req;
}

void
xnd_http_request_destroy(xnd_http_request_t *req)
{
	if (req == NULL)
		return;

	if (req->queries != NULL)
		xnd_string_destroy(&(req->queries));
	if (req->headers != NULL)
		curl_slist_free_all(req->headers);

	curl_easy_cleanup(req->curl);
	xnd_string_destroy(&(req->url));
	free(req);
}

int
xnd_http_request_path(xnd_http_request_t *req, const char *path)
{
	if (req == NULL || path == NULL || !path[0])
		return -1;

	if (req->url->data[req->url->size - 1] != '/')
		xnd_string_append(&(req->url), '/');

	xnd_string_insert(&(req->url), path + (path[0] == '/'), req->url->size);

	return 0;
}

int
xnd_http_request_query(xnd_http_request_t *req, const char *key,
                       const char *value)
{
	if (req == NULL || key == NULL || !key[0])
		return -1;

	if (req->queries == NULL) {
		req->queries = xnd_string_new(NULL);
		if (req->queries == NULL)
			return -1;

		xnd_string_append(&(req->queries), '?');
	} else {
		xnd_string_append(&(req->queries), '&');
	}

	xnd_string_insert(&(req->queries), key, req->queries->size);
	xnd_string_append(&(req->queries), '=');
	xnd_string_insert(&(req->queries), value, req->queries->size);

	return 0;
}

int
xnd_http_request_basic_auth(xnd_http_request_t *req, const char *user,
                            const char *pass)
{
	xnd_string_t *cred;

	if (req == NULL || user == NULL || !user[0])
		return -1;

	cred = xnd_string_new(user);
	if (cred == NULL)
		return -1;

	xnd_string_append(&cred, ':');

	if (pass != NULL && pass[0])
		xnd_string_insert(&cred, pass, cred->size);

	curl_easy_setopt(req->curl, CURLOPT_USERPWD, cred->data);
	xnd_string_zeroize(&cred); /** Do not exposed in memory after it is no
	                               longer needed. At least we do our part. */
	xnd_string_destroy(&cred);

	return 0;
}

int
xnd_http_request_header(xnd_http_request_t *req, const char *key,
                        const char *value)
{
	xnd_string_t *header;

	if (req == NULL || key == NULL || !key[0] || value == NULL || !value[0])
		return -1;

	header = xnd_string_new(key);
	if (header == NULL)
		return -1;

	xnd_string_insert(&header, ": ", header->size);
	xnd_string_insert(&header, value, header->size);
	req->headers = curl_slist_append(req->headers, header->data);
	xnd_string_destroy(&header);

	return 0;
}

int
xnd_http_request_payload(xnd_http_request_t *req, const char *payload)
{
	if (req == NULL || payload == NULL || !payload[0])
		return -1;

	curl_easy_setopt(req->curl, CURLOPT_POSTFIELDS, payload);

	return 0;
}

int
xnd_http_request_callback(xnd_http_request_t *req, xnd_http_request_cb_t cb)
{
	if (req == NULL)
		return -1;

	req->cb = cb;

	return 0;
}

int
xnd_http_request_send_with_data(xnd_http_request_t *req, void *data)
{
	CURLcode res;
	xnd_string_t *fullurl;

	if (req == NULL)
		return -1;

	fullurl = xnd_string_new(req->url->data);
	if (fullurl == NULL)
		return -1;

	if (req->queries != NULL)
		xnd_string_insert(&fullurl, req->queries->data, fullurl->size);

	if (req->headers != NULL)
		curl_easy_setopt(req->curl, CURLOPT_HTTPHEADER, req->headers);

	if (req->cb != NULL) {
		curl_easy_setopt(req->curl, CURLOPT_WRITEFUNCTION, req->cb);
		if (data != NULL)
			curl_easy_setopt(req->curl, CURLOPT_WRITEDATA, data);
	}

	curl_easy_setopt(req->curl, CURLOPT_URL, fullurl->data);
	res = curl_easy_perform(req->curl);
	xnd_string_destroy(&fullurl);

	if (res != CURLE_OK)
		return -1;

	return 0;
}

size_t
xnd_http_request_default_callback(char *ptr, size_t size, size_t nmemb,
                                  void *data)
{
	xnd_string_t **response; /** memory management handled outside */
	size_t realsize = size * nmemb;

	response = (xnd_string_t **) data;
	xnd_string_sized_insert(response, ptr, (*response)->size, realsize);

	return realsize;
}

#ifdef DEBUG
void
xnd_http_request_dump(xnd_http_request_t *req, FILE *output)
{
	FILE *out;

	if (req == NULL)
		return;

	if (output == NULL)
		out = stdout;

	fprintf(out, "\n\n%s ", req->method);
	fprintf(out, "%s", req->url->data);

	if (req->queries != NULL)
		fprintf(out, "%s\n", req->queries->data);
	else
		fprintf(out, "\n");

	fprintf(out, "Header\n");
	if (req->headers != NULL)
		for (struct curl_slist *i = req->headers; i != NULL; i = i->next)
			fprintf(out, "    %s\n", i->data);
}
#endif
