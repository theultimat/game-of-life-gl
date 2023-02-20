#ifndef GOL_ASSERT_H
#define GOL_ASSERT_H

#include <stdnoreturn.h>


#define GOL_ASSERT(x) (void)((x) || (gol_assert_fail(#x, __FILE__, __LINE__, \
    __func__), 0))

#define GOL_ASSERT_MSG(x, fmt, ...) (void)((x) || (gol_assert_fail_msg(#x, __FILE__, \
    __LINE__, __func__, fmt, ##__VA_ARGS__), 0))

#define GOL_CHECK_GL() gol_check_gl(__FILE__, __LINE__, __func__)


// Don't call these directly - use the macros above instead.
noreturn void gol_assert_fail(const char *exp, const char *file, int line, const char *func);
noreturn void gol_assert_fail_msg(const char *exp, const char *file, int line,
    const char *func, const char *fmt, ...) __attribute__((format(printf, 5, 6)));

void gol_check_gl(const char *file, int line, const char *func);

#endif
