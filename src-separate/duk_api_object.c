/*
 *  Object handling: property access and other support functions.
 */

#include "duk_internal.h"

/*
 *  Property handling
 *
 *  The API exposes only the most common property handling functions.
 *  The caller can invoke Ecmascript built-ins for full control (e.g.
 *  defineProperty, getOwnPropertyDescriptor).
 */

duk_bool_t duk_get_prop(duk_context *ctx, duk_idx_t obj_index) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_tval *tv_obj;
	duk_tval *tv_key;
	duk_bool_t rc;

	DUK_ASSERT(ctx != NULL);

	/* Note: copying tv_obj and tv_key to locals to shield against a valstack
	 * resize is not necessary for a property get right now.
	 */

	tv_obj = duk_require_tval(ctx, obj_index);
	tv_key = duk_require_tval(ctx, -1);

	rc = duk_hobject_getprop(thr, tv_obj, tv_key);
	DUK_ASSERT(rc == 0 || rc == 1);
	/* a value is left on stack regardless of rc */

	duk_remove(ctx, -2);  /* remove key */
	return rc;  /* 1 if property found, 0 otherwise */
}

duk_bool_t duk_get_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT(key != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_string(ctx, key);
	return duk_get_prop(ctx, obj_index);
}

duk_bool_t duk_get_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
	DUK_ASSERT(ctx != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_number(ctx, (double) arr_index);
	return duk_get_prop(ctx, obj_index);
}

duk_bool_t duk_get_prop_stridx(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx) {
	duk_hthread *thr = (duk_hthread *) ctx;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_hstring(ctx, thr->strs[stridx]);
	return duk_get_prop(ctx, obj_index);
}

duk_bool_t duk_get_prop_stridx_boolean(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx, duk_bool_t *out_has_prop) {
	duk_bool_t rc;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);

	rc = duk_get_prop_stridx(ctx, obj_index, stridx);
	if (out_has_prop) {
		*out_has_prop = rc;
	}
	rc = duk_to_boolean(ctx, -1);
	DUK_ASSERT(rc == 0 || rc == 1);
	duk_pop(ctx);
	return rc;
}

duk_bool_t duk_put_prop(duk_context *ctx, duk_idx_t obj_index) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_tval *tv_obj;
	duk_tval *tv_key;
	duk_tval *tv_val;
	duk_small_int_t throw_flag;
	duk_bool_t rc;

	DUK_ASSERT(ctx != NULL);

	/* Note: copying tv_obj and tv_key to locals to shield against a valstack
	 * resize is not necessary for a property put right now (putprop protects
	 * against it internally).
	 */

	tv_obj = duk_require_tval(ctx, obj_index);
	tv_key = duk_require_tval(ctx, -2);
	tv_val = duk_require_tval(ctx, -1);
	throw_flag = duk_is_strict_call(ctx);  /* FIXME */

	rc = duk_hobject_putprop(thr, tv_obj, tv_key, tv_val, throw_flag);
	DUK_ASSERT(rc == 0 || rc == 1);

	duk_pop_2(ctx);  /* remove key and value */
	return rc;  /* 1 if property found, 0 otherwise */
}

duk_bool_t duk_put_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT(key != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_string(ctx, key);
	duk_swap_top(ctx, -2);  /* [val key] -> [key val] */
	return duk_put_prop(ctx, obj_index);
}

duk_bool_t duk_put_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
	DUK_ASSERT(ctx != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_number(ctx, (double) arr_index);
	duk_swap_top(ctx, -2);  /* [val key] -> [key val] */
	return duk_put_prop(ctx, obj_index);
}

duk_bool_t duk_put_prop_stridx(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx) {
	duk_hthread *thr = (duk_hthread *) ctx;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_hstring(ctx, thr->strs[stridx]);
	duk_swap_top(ctx, -2);  /* [val key] -> [key val] */
	return duk_put_prop(ctx, obj_index);
}

duk_bool_t duk_del_prop(duk_context *ctx, duk_idx_t obj_index) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_tval *tv_obj;
	duk_tval *tv_key;
	duk_small_int_t throw_flag;
	duk_bool_t rc;

	DUK_ASSERT(ctx != NULL);

	/* Note: copying tv_obj and tv_key to locals to shield against a valstack
	 * resize is not necessary for a property delete right now.
	 */

	tv_obj = duk_require_tval(ctx, obj_index);
	tv_key = duk_require_tval(ctx, -1);
	throw_flag = duk_is_strict_call(ctx);  /* FIXME */

	rc = duk_hobject_delprop(thr, tv_obj, tv_key, throw_flag);
	DUK_ASSERT(rc == 0 || rc == 1);

	duk_pop(ctx);  /* remove key */
	return rc;
}

duk_bool_t duk_del_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT(key != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_string(ctx, key);
	return duk_del_prop(ctx, obj_index);
}

duk_bool_t duk_del_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
	DUK_ASSERT(ctx != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_number(ctx, (double) arr_index);
	return duk_del_prop(ctx, obj_index);
}

duk_bool_t duk_del_prop_stridx(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx) {
	duk_hthread *thr = (duk_hthread *) ctx;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_hstring(ctx, thr->strs[stridx]);
	return duk_del_prop(ctx, obj_index);
}

duk_bool_t duk_has_prop(duk_context *ctx, duk_idx_t obj_index) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_tval *tv_obj;
	duk_tval *tv_key;
	duk_bool_t rc;

	DUK_ASSERT(ctx != NULL);

	/* Note: copying tv_obj and tv_key to locals to shield against a valstack
	 * resize is not necessary for a property existence check right now.
	 */

	tv_obj = duk_require_tval(ctx, obj_index);
	tv_key = duk_require_tval(ctx, -1);

	rc = duk_hobject_hasprop(thr, tv_obj, tv_key);
	DUK_ASSERT(rc == 0 || rc == 1);

	duk_pop(ctx);  /* remove key */
	return rc;  /* 1 if property found, 0 otherwise */
}

duk_bool_t duk_has_prop_string(duk_context *ctx, duk_idx_t obj_index, const char *key) {
	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT(key != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_string(ctx, key);
	return duk_has_prop(ctx, obj_index);
}

duk_bool_t duk_has_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index) {
	DUK_ASSERT(ctx != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_number(ctx, (double) arr_index);
	return duk_has_prop(ctx, obj_index);
}

duk_bool_t duk_has_prop_stridx(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx) {
	duk_hthread *thr = (duk_hthread *) ctx;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	duk_push_hstring(ctx, thr->strs[stridx]);
	return duk_has_prop(ctx, obj_index);
}

/* Define own property without inheritance looks and such.  This differs from
 * [[DefineOwnProperty]] because special behaviors (like Array 'length') are
 * not invoked by this method.  The caller must be careful to invoke any such
 * behaviors if necessary.
 */
void duk_def_prop(duk_context *ctx, duk_idx_t obj_index, duk_small_uint_t desc_flags) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_hobject *obj;
	duk_hstring *key;

	DUK_ASSERT(ctx != NULL);

	obj = duk_require_hobject(ctx, obj_index);
	DUK_ASSERT(obj != NULL);
	key = duk_to_hstring(ctx, -2);
	DUK_ASSERT(key != NULL);
	DUK_ASSERT(duk_require_tval(ctx, -1) != NULL);

	duk_hobject_define_property_internal(thr, obj, key, desc_flags);

	duk_pop(ctx);  /* pop key */
}

void duk_def_prop_index(duk_context *ctx, duk_idx_t obj_index, duk_uarridx_t arr_index, duk_small_uint_t desc_flags) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_hobject *obj;

	DUK_ASSERT(ctx != NULL);

	obj = duk_require_hobject(ctx, obj_index);
	DUK_ASSERT(obj != NULL);

	duk_hobject_define_property_internal_arridx(thr, obj, arr_index, desc_flags);
	/* value popped by call */
}

void duk_def_prop_stridx(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx, duk_small_uint_t desc_flags) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_hobject *obj;
	duk_hstring *key;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);

	obj = duk_require_hobject(ctx, obj_index);
	DUK_ASSERT(obj != NULL);
	key = thr->strs[stridx];
	DUK_ASSERT(key != NULL);
	DUK_ASSERT(duk_require_tval(ctx, -1) != NULL);

	duk_hobject_define_property_internal(thr, obj, key, desc_flags);
	/* value popped by call */
}

void duk_def_prop_stridx_builtin(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx, duk_small_int_t builtin_idx, duk_small_uint_t desc_flags) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_hobject *obj;
	duk_hstring *key;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT_DISABLE(stridx >= 0);
	DUK_ASSERT(stridx < DUK_HEAP_NUM_STRINGS);
	DUK_ASSERT_DISABLE(builtin_idx >= 0);
	DUK_ASSERT(builtin_idx < DUK_NUM_BUILTINS);

	obj = duk_require_hobject(ctx, obj_index);
	DUK_ASSERT(obj != NULL);
	key = thr->strs[stridx];
	DUK_ASSERT(key != NULL);

	duk_push_hobject(ctx, thr->builtins[builtin_idx]);
	duk_hobject_define_property_internal(thr, obj, key, desc_flags);
	/* value popped by call */
}

/* This is a rare property helper; it sets the global thrower (E5 Section 13.2.3)
 * setter/getter into an object property.  This is needed by the 'arguments'
 * object creation code, function instance creation code, and Function.prototype.bind().
 */

void duk_def_prop_stridx_thrower(duk_context *ctx, duk_idx_t obj_index, duk_small_int_t stridx, duk_small_uint_t desc_flags) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_hobject *obj = duk_require_hobject(ctx, obj_index);
	duk_hobject *thrower = thr->builtins[DUK_BIDX_TYPE_ERROR_THROWER];
	duk_hobject_define_accessor_internal(thr, obj, DUK_HTHREAD_GET_STRING(thr, stridx), thrower, thrower, desc_flags);
}

/*
 *  Object related
 *
 *  Note: seal() and freeze() are accessible through Ecmascript bindings,
 *  and are not exposed through the API.
 */

void duk_compact(duk_context *ctx, duk_idx_t obj_index) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_hobject *obj;

	DUK_ASSERT(ctx != NULL);

	obj = duk_get_hobject(ctx, obj_index);
	if (obj) {
		/* Note: this may fail, caller should protect the call if necessary */
		duk_hobject_compact_props(thr, obj);
	}
}

/* FIXME: the duk_hobject_enum.c stack APIs should be reworked */

void duk_enum(duk_context *ctx, duk_idx_t obj_index, duk_uint_t enum_flags) {
	DUK_ASSERT(ctx != NULL);

	duk_require_hobject(ctx, obj_index);
	duk_dup(ctx, obj_index);
	duk_hobject_enumerator_create(ctx, enum_flags);   /* [target] -> [enum] */
}

duk_bool_t duk_next(duk_context *ctx, duk_idx_t enum_index, duk_bool_t get_value) {
	DUK_ASSERT(ctx != NULL);

	duk_require_hobject(ctx, enum_index);
	duk_dup(ctx, enum_index);
	return duk_hobject_enumerator_next(ctx, get_value);
}

/*
 *  Helpers for writing multiple properties
 */

void duk_put_function_list(duk_context *ctx, duk_idx_t obj_index, const duk_function_list_entry *funcs) {
	const duk_function_list_entry *ent = funcs;

	DUK_ASSERT(ctx != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	if (ent != NULL) {
		while (ent->key != NULL) {
			duk_push_c_function(ctx, ent->value, ent->nargs);
			duk_put_prop_string(ctx, obj_index, ent->key);
			ent++;
		}
	}
}

void duk_put_number_list(duk_context *ctx, duk_idx_t obj_index, const duk_number_list_entry *numbers) {
	const duk_number_list_entry *ent = numbers;

	DUK_ASSERT(ctx != NULL);

	obj_index = duk_require_normalize_index(ctx, obj_index);
	if (ent != NULL) {
		while (ent->key != NULL) {
			duk_push_number(ctx, ent->value);
			duk_put_prop_string(ctx, obj_index, ent->key);
			ent++;
		}
	}
}

/*
 *  Shortcut for accessing global object properties
 */

duk_bool_t duk_get_global_string(duk_context *ctx, const char *key) {
	duk_hthread *thr = (duk_hthread *) ctx;
	duk_bool_t ret;

	DUK_ASSERT(ctx != NULL);
	DUK_ASSERT(thr->builtins[DUK_BIDX_GLOBAL] != NULL);

	/* XXX: direct implementation */

	duk_push_hobject(ctx, thr->builtins[DUK_BIDX_GLOBAL]);
	ret = duk_get_prop_string(ctx, -1, key);
	duk_remove(ctx, -2);
	return ret;
}
