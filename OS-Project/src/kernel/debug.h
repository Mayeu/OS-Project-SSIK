
#ifndef DEBUG_H
#define DEBUG_H

/* Only for debug purposes:
 * Your OS should work when NDEBUG is defined.
 */

#ifdef NDEBUG
#define kdebug_putc(n)
#define kdebug_print(n)
#define kdebug_println(n)
#define kdebug_printint(n)
#define kdebug_assert(n)

#else
void            kdebug_putc(char c);
void            kdebug_print(const char *text);
void            kdebug_println(const char *text);
void            kdebug_printint(int value);
void            kdebug_assert_at(int assertion, const char *file, int line);
#define kdebug_assert(a) kdebug_assert_at(a,__FILE__,__LINE__)
#endif

#endif
