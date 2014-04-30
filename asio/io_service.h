#ifndef SUKEY_ASIO_IO_SERVICE_H
#define SUKEY_ASIO_IO_SERVICE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "config.h"
#include "detail\noncopyable.h"

namespace sukey{
namespace asio{

	class io_service:private noncopyable
	{
		SUKEY_ASIO_DECL io_service();
	};

}
}

#endif