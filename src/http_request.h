/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef XND_HTTP_REQUEST_H
#define XND_HTTP_REQUEST_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <curl/curl.h>

#include "strings.h"

extern const char *const XND_HTTP_REQUEST_GET;
extern const char *const XND_HTTP_REQUEST_HEAD;
extern const char *const XND_HTTP_REQUEST_POST;
extern const char *const XND_HTTP_REQUEST_PUT;
extern const char *const XND_HTTP_REQUEST_CONNECT;
extern const char *const XND_HTTP_REQUEST_OPTIONS;
extern const char *const XND_HTTP_REQUEST_TRACE;
extern const char *const XND_HTTP_REQUEST_PATCH;

/**
 * \brief Callback for writing received data.
 *
 * \details Parameters:
 * 1. (char *) The pointer to the delivered data. It points to the beginning of
 * the data received from the transfer.
 * 2. (size_t) The size of each data element. In this case, it is always 1
 * because the size parameter is fixed at 1 for this callback.
 * 3. (size_t) The number of data elements pointed to by the first parameter.
 * This parameter indicates the size of the data received in this particular
 * callback.
 * 4. (void *) The pointer to user-defined data passed to the callback
 * function.
 */
typedef size_t (*xnd_http_request_cb_t) (char *, size_t, size_t, void *);

/**
 * \brief HTTP request.
 */
typedef struct xnd_http_request_t {
	const char            *method;  /** HTTP request method. */
	xnd_string_t          *url;     /** URL. */
	xnd_string_t          *queries; /** Query parameters. */
	struct curl_slist     *headers; /** List of HTTP header records. */
	CURL                  *curl;    /** curl instance. */
	xnd_http_request_cb_t  cb;      /** Write callback. */
} xnd_http_request_t;

/**
 * \brief Initiates HTTP request module.
 */
extern void
xnd_http_request_init(void);

/**
 * \brief Cleans up HTTP request module.
 */
extern void
xnd_http_request_cleanup(void);

/**
 * \brief Creates new HTTP request.
 * \param baseurl The base URL of the HTTP request.
 * \return NULL on failure.
 */
extern xnd_http_request_t *
xnd_http_request_new(const char *method, const char *baseurl);

/**
 * \brief Destroys HTTP request.
 * \param req The HTTP request to destroy.
 */
extern void
xnd_http_request_destroy(xnd_http_request_t *req);

/**
 * \brief Adds path to the existing URL of the HTTP request.
 * \param path The path to be concatenated to the existing request URL. Do need
 * to provide any '/' as it is managed inside.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_path(xnd_http_request_t *req, const char *path);

/**
 * \brief Adds a query parameter to the URL of the HTTP request.
 * \param key The key of the query parameter to add.
 * \param value The value of the query parameter to add.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_query(xnd_http_request_t *req, const char *key,
                       const char *value);

/**
 * \brief Add Basic Authentication for the request.
 * \param req The HTTP request.
 * \param user The username.
 * \param pass The password.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_basic_auth(xnd_http_request_t *req, const char *user,
                            const char *pass);

/**
 * \brief Adds a record to the HTTP request headers.
 * \param req The HTTP request.
 * \param key The key of the HTTP request header to add.
 * \param value The value of the HTTP request header to add.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_header(xnd_http_request_t *req, const char *key,
                        const char *value);

/**
 * \brief Sets the payload of the HTTP request.
 * \param req The HTTP request.
 * \param payload The umm.. payload?
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_payload(xnd_http_request_t *req, const char *payload);

/**
 * \brief Sets the write callback for retrieved response.
 * \param req The HTTP request.
 * \param cb The callback function for writing received data.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_callback(xnd_http_request_t *req, xnd_http_request_cb_t cb);

/**
 * \brief Sends HTTP request with user-defined data passed to write cb
 * function, if provided, though.
 * \param req The HTTP request.
 * \param data The user-defined data to be passed to the write cb function.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_http_request_send_with_data(xnd_http_request_t *req, void *data);

/**
 * \brief Sends HTTP request.
 * \param Req The HTTP request.
 * \return 0 on success, -1 otherwise.
 */
#define xnd_http_request_send(Req) xnd_http_request_send_with_data(Req, NULL);

/**
 * \brief The default callback of Xendit HTTP request. This callback expects
 * `data` to be of type `xnd_string_t`.
 * \param ptr The delivered data.
 * \param size The size of each data element.
 * \param nmemb The number of data elements pointed to by `ptr`.
 * \param data Pointer to user-defined data, expects `xnd_string_t *`.
 * \return
 */
extern size_t
xnd_http_request_default_callback(char *ptr, size_t size, size_t nmemb,
                                  void *data);

#ifdef DEBUG
/**
 * \brief Dumps the details of HTTP request for debugging purposes.
 * \param req The http request.
 */
extern void
xnd_http_request_dump(xnd_http_request_t *req, FILE *output);
#endif

#ifdef __cplusplus
}
#endif

#endif
