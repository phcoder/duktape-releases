/*
 *  Math built-ins
 */

#include "duk_internal.h"

/*
 *  Use static helpers which can work with math.h functions matching
 *  the following signatures. This is not portable if any of these math
 *  functions is actually a macro.
 *
 *  Typing here is intentionally 'double' because that's what the standard
 *  library APIs use.
 */

typedef double (*duk__one_arg_func)(double);
typedef double (*duk__two_arg_func)(double, double);

static int duk__math_minmax(duk_context *ctx, double initial, duk__two_arg_func min_max) {
	duk_int_t n = duk_get_top(ctx);
	duk_int_t i;
	double res = initial;
	double t;

	/*
	 *  Note: fmax() does not match the E5 semantics.  E5 requires
	 *  that if -any- input to Math.max() is a NaN, the result is a
	 *  NaN.  fmax() will return a NaN only if -both- inputs are NaN.
	 *  Same applies to fmin().
	 *
	 *  Note: every input value must be coerced with ToNumber(), even
	 *  if we know the result will be a NaN anyway: ToNumber() may have
	 *  side effects for which even order of evaluation matters.
	 */

	for (i = 0; i < n; i++) {
		t = duk_to_number(ctx, i);
		if (DUK_FPCLASSIFY(t) == DUK_FP_NAN || DUK_FPCLASSIFY(res) == DUK_FP_NAN) {
			/* Note: not normalized, but duk_push_number() will normalize */
			res = DUK_DOUBLE_NAN;
		} else {
			res = min_max(res, t);
		}
	}

	duk_push_number(ctx, res);
	return 1;
}

static double duk__fmin_fixed(double x, double y) {
	/* fmin() with args -0 and +0 is not guaranteed to return
	 * -0 as Ecmascript requires.
	 */
	if (x == 0 && y == 0) {
		/* XXX: what's the safest way of creating a negative zero? */
		if (DUK_SIGNBIT(x) != 0 || DUK_SIGNBIT(y) != 0) {
			return -0.0;
		} else {
			return +0.0;
		}
	}
#ifdef DUK_USE_MATH_FMIN
	return DUK_FMIN(x, y);
#else
	return (x < y ? x : y);
#endif
}

static double duk__fmax_fixed(double x, double y) {
	/* fmax() with args -0 and +0 is not guaranteed to return
	 * +0 as Ecmascript requires.
	 */
	if (x == 0 && y == 0) {
		if (DUK_SIGNBIT(x) == 0 || DUK_SIGNBIT(y) == 0) {
			return +0.0;
		} else {
			return -0.0;
		}
	}
#ifdef DUK_USE_MATH_FMAX
	return DUK_FMAX(x, y);
#else
	return (x > y ? x : y);
#endif
}

static double duk__round_fixed(double x) {
	/* Numbers half-way between integers must be rounded towards +Infinity,
	 * e.g. -3.5 must be rounded to -3 (not -4).  When rounded to zero, zero
	 * sign must be set appropriately.  E5.1 Section 15.8.2.15.
	 *
	 * Note that ANSI C round() is "round to nearest integer, away from zero",
	 * which is incorrect for negative values.  Here we make do with floor().
	 */

	int c = DUK_FPCLASSIFY(x);
	if (c == DUK_FP_NAN || c == DUK_FP_INFINITE || c == DUK_FP_ZERO) {
		return x;
	}

	/*
	 *  x is finite and non-zero
	 *
	 *  -1.6 -> floor(-1.1) -> -2
	 *  -1.5 -> floor(-1.0) -> -1  (towards +Inf)
	 *  -1.4 -> floor(-0.9) -> -1
	 *  -0.5 -> -0.0               (special case)
	 *  -0.1 -> -0.0               (special case)
	 *  +0.1 -> +0.0               (special case)
	 *  +0.5 -> floor(+1.0) -> 1   (towards +Inf)
	 *  +1.4 -> floor(+1.9) -> 1
	 *  +1.5 -> floor(+2.0) -> 2   (towards +Inf)
	 *  +1.6 -> floor(+2.1) -> 2
	 */

	if (x >= -0.5 && x < 0.5) {
		/* +0.5 is handled by floor, this is on purpose */
		if (x < 0.0) {
			return -0.0;
		} else {
			return +0.0;
		}
	}

	return DUK_FLOOR(x + 0.5);
}

static double duk__pow_fixed(double x, double y) {
	/* The ANSI C pow() semantics differ from Ecmascript.
	 *
	 * E.g. when x==1 and y is +/- infinite, the Ecmascript required
	 * result is NaN, while at least Linux pow() returns 1.
	 */

	int cx, cy, sx;

	DUK_UNREF(cx);
	DUK_UNREF(sx);
	cy = DUK_FPCLASSIFY(y);

	if (cy == DUK_FP_NAN) {
		goto ret_nan;
	}
	if (DUK_FABS(x) == 1.0 && cy == DUK_FP_INFINITE) {
		goto ret_nan;
	}
#if defined(DUK_USE_POW_NETBSD_WORKAROUND)
	/* See test-bug-netbsd-math-pow.js: NetBSD 6.0 on x86 (at least) does not
	 * correctly handle some cases where x=+/-0.  Specific fixes to these
	 * here.
	 */
	cx = DUK_FPCLASSIFY(x);
	if (cx == DUK_FP_ZERO && y < 0.0) {
		sx = DUK_SIGNBIT(x);
		if (sx == 0) {
			/* Math.pow(+0,y) should be Infinity when y<0.  NetBSD pow()
			 * returns -Infinity instead when y is <0 and finite.  The
			 * if-clause also catches y == -Infinity (which works even
			 * without the fix).
			 */
			return DUK_DOUBLE_INFINITY;
		} else {
			/* Math.pow(-0,y) where y<0 should be:
			 *   - -Infinity if y<0 and an odd integer
			 *   - Infinity otherwise
			 * NetBSD pow() returns -Infinity for all finite y<0.  The
			 * if-clause also catches y == -Infinity (which works even
			 * without the fix).
			 */

			/* fmod() return value has same sign as input (negative) so
			 * the result here will be in the range ]-2,0], 1 indicates
			 * odd.  If x is -Infinity, NaN is returned and the odd check
			 * always concludes "not odd" which results in desired outcome.
			 */
			double tmp = DUK_FMOD(y, 2);
			if (tmp == -1.0) {
				return -DUK_DOUBLE_INFINITY;
			} else {
				/* Not odd, or y == -Infinity */
				return DUK_DOUBLE_INFINITY;
			}
		}
	}
#endif
	return DUK_POW(x, y);

 ret_nan:
	return DUK_DOUBLE_NAN;
}

/* order must match constants in genbuiltins.py */
static const duk__one_arg_func duk__one_arg_funcs[] = {
	DUK_FABS,
	DUK_ACOS,
	DUK_ASIN,
	DUK_ATAN,
	DUK_CEIL,
	DUK_COS,
	DUK_EXP,
	DUK_FLOOR,
	DUK_LOG,
	duk__round_fixed,
	DUK_SIN,
	DUK_SQRT,
	DUK_TAN
};

/* order must match constants in genbuiltins.py */
static const duk__two_arg_func duk__two_arg_funcs[] = {
	DUK_ATAN2,
	duk__pow_fixed
};

duk_ret_t duk_bi_math_object_onearg_shared(duk_context *ctx) {
	duk_small_int_t fun_idx = duk_get_magic(ctx);
	duk__one_arg_func fun;

	DUK_ASSERT(fun_idx >= 0);
	DUK_ASSERT(fun_idx < (duk_small_int_t) (sizeof(duk__one_arg_funcs) / sizeof(duk__one_arg_func)));
	fun = duk__one_arg_funcs[fun_idx];
	/* FIXME: double typing here: double or duk_double_t? */
	duk_push_number(ctx, fun((double) duk_to_number(ctx, 0)));
	return 1;
}

duk_ret_t duk_bi_math_object_twoarg_shared(duk_context *ctx) {
	duk_small_int_t fun_idx = duk_get_magic(ctx);
	duk__two_arg_func fun;

	DUK_ASSERT(fun_idx >= 0);
	DUK_ASSERT(fun_idx < (duk_small_int_t) (sizeof(duk__two_arg_funcs) / sizeof(duk__two_arg_func)));
	fun = duk__two_arg_funcs[fun_idx];
	/* FIXME: double typing here: double or duk_double_t? */
	duk_push_number(ctx, fun((double) duk_to_number(ctx, 0), (double) duk_to_number(ctx, 1)));
	return 1;
}

duk_ret_t duk_bi_math_object_max(duk_context *ctx) {
	return duk__math_minmax(ctx, -DUK_DOUBLE_INFINITY, duk__fmax_fixed);
}

duk_ret_t duk_bi_math_object_min(duk_context *ctx) {
	return duk__math_minmax(ctx, DUK_DOUBLE_INFINITY, duk__fmin_fixed);
}

duk_ret_t duk_bi_math_object_random(duk_context *ctx) {
	duk_push_number(ctx, (duk_double_t) duk_util_tinyrandom_get_double((duk_hthread *) ctx));
	return 1;
}
