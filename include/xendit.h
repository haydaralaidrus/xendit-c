/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2023 Haydar Alaidrus
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file or at https://opensource.org/licenses/MIT.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef XND_XENDIT_H
#define XND_XENDIT_H 1

#ifdef __cplusplus
extern "C" {
#endif

#define XND_BASEURL "https://api.xendit.co"

/**
 * \brief Xendit client opaque object.
 */
typedef struct xnd_client_t xnd_client_t;

/**
 * \brief Sets up the environment for Xendit SDK.
 */
extern void
xnd_sdk_init(void);

/**
 * \brief Cleans up the Xendit SDK environment.
 */
extern void
xnd_sdk_cleanup(void);

/**
 * \brief Creates new Xendit client.
 * \param key The secret API key.
 * \return NULL on failure.
 */
extern xnd_client_t *
xnd_client_new(const char *key);

/**
 * \brief Destroys Xendit client.
 * \param x The Xendit client to destroy.
 */
extern void
xnd_client_destroy(xnd_client_t *x);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Balances
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define XND_ENDPOINT_BALANCE (XND_BASEURL "/balance")

/**
 * \brief Xendit balance object.
 */
typedef struct xnd_balance_t {
	double balance;
} xnd_balance_t;

/**
 * \brief Retrieves the balance of your cash and pending balance.
 * \param x The Xendit client.
 * \param for_user_id The XenPlatform sub-account ID for the transaction.
 * \param account_type The balance type, "CASH", "HOLDING", or "TAX"
 * \param currency The currency filter.
 * \param response The retrieved balance information.
 * \return 0 on success, -1 otherwise.
 */
extern int
xnd_balance(const xnd_client_t *x, const char *for_user_id,
            const char *account_type, const char *currency,
            xnd_balance_t *response);

#ifdef __cplusplus
}
#endif

#endif
