#ifndef __volucris_assert_h__
#define __volucris_assert_h__

#include "logging.h"

#ifdef _DEBUG
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK()
#endif // _DEBUG

#ifdef _DEBUG
#define check(exp) if (!(exp)){ DEBUG_BREAK();}
#define checkf(exp, category, msg, ...)  if (!(exp)){ V_LOG_ERROR(category, msg, ##__VA_ARGS__); DEBUG_BREAK();}
#define checkq(exp, category, msg, ...) checkf(exp, category, msg, ##__VA_ARGS__);
#else
#define checkf(exp, category, msg, ...) if (!(exp)) { V_LOG_ERROR(category, msg, ##__VA_ARGS__);}
#define check(exp)
#define checkq(exp, category, msg, ...) if (!(exp)) { V_LOG_ERROR(category, msg, ##__VA_ARGS__); std::exit(-1); }
#endif


#endif // !__volucris_assert_h__
