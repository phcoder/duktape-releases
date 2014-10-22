/*
 *  Internal API calls which have (stack and other) semantics
 *  similar to the public API.
 */

#ifndef DUK_API_INTERNAL_H_INCLUDED
#define DUK_API_INTERNAL_H_INCLUDED

/* duk_push_sprintf constants */
#define DUK_PUSH_SPRINTF_INITIAL_SIZE  256
#define DUK_PUSH_SPRINTF_SANITY_LIMIT  (1*1024*1024*1024)

/* Flag ORed to err_code to indicate __FILE__ / __LINE__ is not
 * blamed as source of error for error fileName / lineNumber.
 */
#define DUK_ERRCODE_FLAG_NOBLAME_FILELINE  (1 << 24)

int duk_check_valstack_resize(duk_context *ctx, unsigned int min_new_size, int allow_shrink);
void duk_require_valstack_resize(duk_context *ctx, unsigned int min_new_size, int allow_shrink);

int duk_check_stack_raw(duk_context *ctx, unsigned int extra);
void duk_require_stack_raw(duk_context *ctx, unsigned int extra);

duk_tval *duk_get_tval(duk_context *ctx, int index);
duk_tval *duk_require_tval(duk_context *ctx, int index);
void duk_push_tval(duk_context *ctx, duk_tval *tv);

void duk_push_this_check_object_coercible(duk_context *ctx);   /* push the current 'this' binding; throw TypeError
                                                                * if binding is not object coercible (CheckObjectCoercible).
                                                                */
duk_hobject *duk_push_this_coercible_to_object(duk_context *ctx);       /* duk_push_this() + CheckObjectCoercible() + duk_to_object() */
duk_hstring *duk_push_this_coercible_to_string(duk_context *ctx);       /* duk_push_this() + CheckObjectCoercible() + duk_to_string() */

void duk_push_u32(duk_context *ctx, duk_uint32_t val);

/* internal helper for looking up a tagged type */
#define  DUK_GETTAGGED_FLAG_ALLOW_NULL  (1 << 24)
#define  DUK_GETTAGGED_FLAG_CHECK_CLASS (1 << 25)
#define  DUK_GETTAGGED_CLASS_SHIFT      16

duk_heaphdr *duk_get_tagged_heaphdr_raw(duk_context *ctx, int index, duk_int_t flags_and_tag);

duk_hstring *duk_get_hstring(duk_context *ctx, int index);
duk_hobject *duk_get_hobject(duk_context *ctx, int index);
duk_hbuffer *duk_get_hbuffer(duk_context *ctx, int index);
duk_hthread *duk_get_hthread(duk_context *ctx, int index);
duk_hcompiledfunction *duk_get_hcompiledfunction(duk_context *ctx, int index);
duk_hnativefunction *duk_get_hnativefunction(duk_context *ctx, int index);

#define duk_get_hobject_with_class(ctx,index,classnum) \
	((duk_hobject *) duk_get_tagged_heaphdr_raw((ctx), (index), \
		DUK_TAG_OBJECT | DUK_GETTAGGED_FLAG_ALLOW_NULL | \
		DUK_GETTAGGED_FLAG_CHECK_CLASS | ((classnum) << DUK_GETTAGGED_CLASS_SHIFT)))

/* XXX: add specific getters for e.g. thread; duk_get_hobject_with_flags()
 * could be the underlying primitive?
 */

duk_hstring *duk_to_hstring(duk_context *ctx, int index);
int duk_to_int_clamped_raw(duk_context *ctx, int index, int minval, int maxval, int *out_clamped);  /* out_clamped=NULL, RangeError if outside range */
int duk_to_int_clamped(duk_context *ctx, int index, int minval, int maxval);
int duk_to_int_check_range(duk_context *ctx, int index, int minval, int maxval);

duk_hstring *duk_require_hstring(duk_context *ctx, int index);
duk_hobject *duk_require_hobject(duk_context *ctx, int index);
duk_hbuffer *duk_require_hbuffer(duk_context *ctx, int index);
duk_hthread *duk_require_hthread(duk_context *ctx, int index);
duk_hcompiledfunction *duk_require_hcompiledfunction(duk_context *ctx, int index);
duk_hnativefunction *duk_require_hnativefunction(duk_context *ctx, int index);

#define duk_require_hobject_with_class(ctx,index,classnum) \
	((duk_hobject *) duk_get_tagged_heaphdr_raw((ctx), (index), \
		DUK_TAG_OBJECT | \
		DUK_GETTAGGED_FLAG_CHECK_CLASS | ((classnum) << DUK_GETTAGGED_CLASS_SHIFT)))

void duk_push_unused(duk_context *ctx);
void duk_push_hstring(duk_context *ctx, duk_hstring *h);
void duk_push_hstring_stridx(duk_context *ctx, int stridx);
void duk_push_hobject(duk_context *ctx, duk_hobject *h);
void duk_push_hbuffer(duk_context *ctx, duk_hbuffer *h);
void duk_push_builtin(duk_context *ctx, int builtin_idx);
int duk_push_object_helper(duk_context *ctx, int hobject_flags_and_class, int prototype_bidx);
int duk_push_object_helper_proto(duk_context *ctx, int hobject_flags_and_class, duk_hobject *proto);
int duk_push_object_internal(duk_context *ctx);
int duk_push_compiledfunction(duk_context *ctx);
void duk_push_c_function_nospecial(duk_context *ctx, duk_c_function func, int nargs);
void duk_push_c_function_noconstruct_nospecial(duk_context *ctx, duk_c_function func, int nargs);

int duk_get_prop_stridx(duk_context *ctx, int obj_index, unsigned int stridx);     /* [] -> [val] */
int duk_put_prop_stridx(duk_context *ctx, int obj_index, unsigned int stridx);     /* [val] -> [] */
int duk_del_prop_stridx(duk_context *ctx, int obj_index, unsigned int stridx);     /* [] -> [] */
int duk_has_prop_stridx(duk_context *ctx, int obj_index, unsigned int stridx);     /* [] -> [] */

int duk_get_prop_stridx_boolean(duk_context *ctx, int obj_index, duk_small_int_t stridx, int *out_has_prop);  /* [] -> [] */

void duk_def_prop(duk_context *ctx, int obj_index, int desc_flags);  /* [key val] -> [] */
void duk_def_prop_index(duk_context *ctx, int obj_index, unsigned int arr_index, int desc_flags);  /* [val] -> [] */
void duk_def_prop_stridx(duk_context *ctx, int obj_index, unsigned int stridx, int desc_flags);  /* [val] -> [] */
void duk_def_prop_stridx_builtin(duk_context *ctx, int obj_index, unsigned int stridx, unsigned int builtin_idx, int desc_flags);  /* [] -> [] */

void duk_def_prop_stridx_thrower(duk_context *ctx, int obj_index, unsigned int stridx, int desc_flags);  /* [] -> [] */

#endif  /* DUK_API_INTERNAL_H_INCLUDED */

