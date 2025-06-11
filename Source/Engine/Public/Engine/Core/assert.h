#ifndef __volucris_assert_h__
#define __volucris_assert_h__

#include "logging.h"

#ifdef _DEBUG
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK()
#endif // _DEBUG

#ifdef _DEBUG
#define v_check(exp) if (!(exp)){ DEBUG_BREAK();}
#define v_checkf(exp, category, msg, ...)  if (!(exp)){ V_LOG_ERROR(category, msg, ##__VA_ARGS__); DEBUG_BREAK();}
#define v_checkq(exp, category, msg, ...) v_checkf(exp, category, msg, ##__VA_ARGS__);
#else
#define v_checkf(exp, category, msg, ...) if (!(exp)) { V_LOG_ERROR(category, msg, ##__VA_ARGS__);}
#define v_check(exp)
#define v_checkq(exp, category, msg, ...) if (!(exp)) { V_LOG_ERROR(category, msg, ##__VA_ARGS__); std::exit(-1); }
#endif


#endif // !__volucris_assert_h__
