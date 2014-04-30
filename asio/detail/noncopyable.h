#ifndef SUKEY_ASIO_DETAIL_NONCOPYALBE_H
#define SUKEY_ASIO_DETAIL_NONCOPYALBE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

namespace sukey{
namespace asio{
namespace detail{

class noncopyable
{
public:
	noncopyable(){}
	~noncopyable(){}
private:
	noncopyable(const noncopyable&);
	const noncopyable& operator=(const noncopyable&);
};

}
using sukey::asio::detail::noncopyable;
}
}

#endif
