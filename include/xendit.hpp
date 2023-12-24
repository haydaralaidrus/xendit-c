#ifndef XND_XENDIT_HPP
#define XND_XENDIT_HPP 1

#include <string>
#include <utility>
#include <xendit/xendit.h>

namespace xnd {

class client {
private:

	bool error_;
	xnd_client_t *client_;

public:

	client(const char *key);
	client(const std::string &key);
	~client();
	client(const client &other) = delete;
	client(client &&other) noexcept;

	client &
	operator=(const client &rhs) = delete;
	client &
	operator=(client &&rhs) noexcept;

	bool
	has_error(void) const;

};

inline
client::client(const char *key)
	: error_ { false }
	, client_ { xnd_client_new(key) }
{
	if (client_ == nullptr)
		error_ = true;
}

inline
client::client(const std::string &key)
	: client(key.c_str())
{}

inline
client::~client()
{
	xnd_client_destroy(client_);
}

inline client::client(client &&other) noexcept
	: error_ { std::exchange(other.error_, false) }
	, client_{ std::exchange(other.client_, nullptr) }
{}

inline client &
client::operator=(client &&rhs) noexcept
{
	if (this != &rhs) {
		error_ = std::exchange(rhs.error_, false);
		client_ = std::exchange(rhs.client_, nullptr);
	}

	return *this;
}

bool
client::has_error(void) const
{
	return error_;
}

}

#endif
