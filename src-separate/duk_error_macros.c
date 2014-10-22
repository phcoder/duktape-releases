/*
 *  Error, fatal, and panic handling.
 */

#include "duk_internal.h"

#define DUK__ERRFMT_BUFSIZE  256  /* size for formatting buffers */

#ifdef DUK_USE_VERBOSE_ERRORS

#ifdef DUK_USE_VARIADIC_MACROS
void duk_err_handle_error(const char *filename, int line, duk_hthread *thr, int code, const char *fmt, ...) {
	va_list ap;
	char msg[DUK__ERRFMT_BUFSIZE];
	va_start(ap, fmt);
	(void) DUK_VSNPRINTF(msg, sizeof(msg), fmt, ap);
	msg[sizeof(msg) - 1] = (char) 0;
	duk_err_create_and_throw(thr, code, msg, filename, line);
	va_end(ap);  /* dead code, but ensures portability (see Linux man page notes) */
}
#else  /* DUK_USE_VARIADIC_MACROS */
const char *duk_err_file_stash = NULL;
int duk_err_line_stash = 0;

DUK_NORETURN(static void duk__handle_error(const char *filename, int line, duk_hthread *thr, int code, const char *fmt, va_list ap));

static void duk__handle_error(const char *filename, int line, duk_hthread *thr, int code, const char *fmt, va_list ap) {
	char msg[DUK__ERRFMT_BUFSIZE];
	(void) DUK_VSNPRINTF(msg, sizeof(msg), fmt, ap);
	msg[sizeof(msg) - 1] = (char) 0;
	duk_err_create_and_throw(thr, code, msg, filename, line);
}

void duk_err_handle_error(const char *filename, int line, duk_hthread *thr, int code, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	duk__handle_error(filename, line, thr, code, fmt, ap);
	va_end(ap);  /* dead code */
}

void duk_err_handle_error_stash(duk_hthread *thr, int code, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	duk__handle_error(duk_err_file_stash, duk_err_line_stash, thr, code, fmt, ap);
	va_end(ap);  /* dead code */
}
#endif  /* DUK_USE_VARIADIC_MACROS */

#else  /* DUK_USE_VERBOSE_ERRORS */

#ifdef DUK_USE_VARIADIC_MACROS
void duk_err_handle_error(duk_hthread *thr, int code) {
	duk_err_create_and_throw(thr, code);
}

#else  /* DUK_USE_VARIADIC_MACROS */
void duk_err_handle_error_nonverbose1(duk_hthread *thr, int code, const char *fmt, ...) {
	duk_err_create_and_throw(thr, code);
}

void duk_err_handle_error_nonverbose2(const char *filename, int line, duk_hthread *thr, int code, const char *fmt, ...) {
	duk_err_create_and_throw(thr, code);
}
#endif  /* DUK_USE_VARIADIC_MACROS */

#endif  /* DUK_USE_VERBOSE_ERRORS */

/*
 *  Default fatal error handler
 */

void duk_default_fatal_handler(duk_context *ctx, int code, const char *msg) {
	DUK_UNREF(ctx);
#ifdef DUK_USE_FILE_IO
	DUK_FPRINTF(DUK_STDERR, "FATAL %d: %s\n", code, msg ? msg : "null");
	DUK_FFLUSH(DUK_STDERR);
#else
	/* omit print */
#endif
	DUK_DPRINT("default fatal handler called, code %d -> calling DUK_PANIC()", code);
	DUK_PANIC(code, msg);
	DUK_UNREACHABLE();
}

/*
 *  Default panic handler
 */

#if !defined(DUK_USE_PANIC_HANDLER)
void duk_default_panic_handler(int code, const char *msg) {
#ifdef DUK_USE_FILE_IO
	DUK_FPRINTF(DUK_STDERR, "PANIC %d: %s ("
#if defined(DUK_USE_PANIC_ABORT)
	            "calling abort"
#elif defined(DUK_USE_PANIC_EXIT)
	            "calling exit"
#elif defined(DUK_USE_PANIC_SEGFAULT)
	            "segfaulting on purpose"
#else
#error no DUK_USE_PANIC_xxx macro defined
#endif	
	            ")\n", code, msg ? msg : "null");
	DUK_FFLUSH(DUK_STDERR);
#else
	/* omit print */
#endif

#if defined(DUK_USE_PANIC_ABORT)
	DUK_ABORT();
#elif defined(DUK_USE_PANIC_EXIT)
	DUK_EXIT(-1);
#elif defined(DUK_USE_PANIC_SEGFAULT)
	/* exit() afterwards to satisfy "noreturn" */
	DUK_CAUSE_SEGFAULT();
	DUK_EXIT(-1);
#else
#error no DUK_USE_PANIC_xxx macro defined
#endif

	DUK_UNREACHABLE();
}
#endif  /* !DUK_USE_PANIC_HANDLER */

#undef DUK__ERRFMT_BUFSIZE
