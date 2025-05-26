#ifndef __volucris_volucris_h__
#define __volucris_volucris_h__

#include <string_view>
#include "Core/logging.h"
#include "assert.h"

VOLUCRIS_DEFINE_LOG(Engine)

namespace volucris
{
	static_assert(sizeof(int) == 4, "Error: 'int' is not 4 bytes on this platform!");
	static_assert(sizeof(size_t) == 8, "Error: 'size_t' is not 8 bytes on this platform!");
}

#endif // !__volucris_volucris_h__
