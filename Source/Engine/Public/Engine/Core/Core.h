#ifndef __volucris_core_h__
#define __volucris_core_h__

#include <Engine/Core/Assert.h>
#include <Engine/Core/Logging.h>

#define _V_CONCAT_IMPL(x, y) x##y

#define _V_CONCAT(x, y) _V_CONCAT_IMPL(x, y)

#define V_UNIQUE_NAME(prefix) _V_CONCAT(prefix, __LINE__)

#endif // !__volucris_core_h__
