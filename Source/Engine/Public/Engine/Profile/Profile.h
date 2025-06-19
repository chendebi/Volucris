#ifndef __volucris_profile_h__
#define __volucris_profile_h__

#include <Engine/Profile/ScopedTimer.h>
#include <Engine/Profile/ProfileManager.h>
#include <tracy/Tracy.hpp>

#define _V_CONCAT_IMPL(x, y) x##y

#define _V_CONCAT(x, y) _V_CONCAT_IMPL(x, y)

#define _V_UNIQUE_NAME(prefix) _V_CONCAT(prefix, __LINE__)

#define V_FUNCTION_NAME __FUNCSIG__

#define V_NAMED_PROFILE(name) ZoneScopedN(name);

#define V_SCOPED_PROFILE ZoneScoped;

//#define V_PROFILE_FUNC() V_PROFILE(V_FUNCTION_NAME, V_FUNCTION_NAME)

#endif // !__volucris_profile_h__
