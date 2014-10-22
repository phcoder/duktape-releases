/*
 *  Shared error message strings
 *
 *  To minimize code footprint, try to share error messages inside Duktape
 *  code.
 */

#include "duk_internal.h"

/* Mostly API related */
const char *duk_str_internal_error = "internal error";
const char *duk_str_invalid_count = "invalid count";
const char *duk_str_invalid_call_args = "invalid call args";
const char *duk_str_not_constructable = "not constructable";
const char *duk_str_not_callable = "not callable";
const char *duk_str_not_extensible = "not extensible";
const char *duk_str_not_writable = "not writable";
const char *duk_str_not_configurable = "not configurable";

const char *duk_str_invalid_index = "invalid index";
const char *duk_str_push_beyond_alloc_stack = "attempt to push beyond currently allocated stack";
const char *duk_str_src_stack_not_enough = "source stack does not contain enough elements";
const char *duk_str_not_undefined = "not undefined";
const char *duk_str_not_null = "not null";
const char *duk_str_not_boolean = "not boolean";
const char *duk_str_not_number = "not number";
const char *duk_str_not_string = "not string";
const char *duk_str_not_pointer = "not pointer";
const char *duk_str_not_buffer = "not buffer";
const char *duk_str_not_object = "not object";
const char *duk_str_unexpected_type = "unexpected type";
const char *duk_str_not_thread = "not thread";
const char *duk_str_not_compiledfunction = "not compiledfunction";
const char *duk_str_not_nativefunction = "not nativefunction";
const char *duk_str_not_c_function = "not c function";
const char *duk_str_defaultvalue_coerce_failed = "[[DefaultValue]] coerce failed";
const char *duk_str_number_outside_range = "number outside range";
const char *duk_str_not_object_coercible = "not object coercible";
const char *duk_str_string_too_long = "string too long";
const char *duk_str_buffer_too_long = "buffer too long";
const char *duk_str_sprintf_too_long = "sprintf message too long";
const char *duk_str_object_alloc_failed = "object alloc failed";
const char *duk_str_thread_alloc_failed = "thread alloc failed";
const char *duk_str_func_alloc_failed = "func alloc failed";
const char *duk_str_buffer_alloc_failed = "buffer alloc failed";
const char *duk_str_pop_too_many = "attempt to pop too many entries";

/* JSON */
const char *duk_str_fmt_ptr = "%p";
const char *duk_str_invalid_json = "invalid json";
const char *duk_str_invalid_number = "invalid number";
const char *duk_str_jsondec_reclimit = "json decode recursion limit";
const char *duk_str_jsonenc_reclimit = "json encode recursion limit";
const char *duk_str_cyclic_input = "cyclic input";

/* Object property access */
const char *duk_str_proxy_revoked = "proxy revoked";
const char *duk_str_object_resize_failed = "object resize failed";
const char *duk_str_invalid_base = "invalid base value";
const char *duk_str_strict_caller_read = "attempt to read strict 'caller'";
const char *duk_str_proxy_rejected = "proxy rejected";
const char *duk_str_invalid_array_length = "invalid array length";
const char *duk_str_array_length_write_failed = "array length write failed";
const char *duk_str_array_length_not_writable = "array length non-writable";
const char *duk_str_setter_undefined = "setter undefined";
const char *duk_str_redefine_virt_prop = "attempt to redefine virtual property";
const char *duk_str_invalid_descriptor = "invalid descriptor";
const char *duk_str_property_is_virtual = "property is virtual";

/* Compiler */
const char *duk_str_parse_error = "parse error";
const char *duk_str_duplicate_label = "duplicate label";
const char *duk_str_invalid_label = "invalid label";
const char *duk_str_invalid_array_literal = "invalid array literal";
const char *duk_str_invalid_object_literal = "invalid object literal";
const char *duk_str_invalid_var_declaration = "invalid variable declaration";
const char *duk_str_cannot_delete_identifier = "cannot delete identifier";
const char *duk_str_invalid_expression = "invalid expression";
const char *duk_str_invalid_lvalue = "invalid lvalue";
const char *duk_str_expected_identifier = "expected identifier";
const char *duk_str_empty_expr_not_allowed = "empty expression not allowed";
const char *duk_str_invalid_for = "invalid for statement";
const char *duk_str_invalid_switch = "invalid switch statement";
const char *duk_str_invalid_break_cont_label = "invalid break/continue label";
const char *duk_str_invalid_return = "invalid return";
const char *duk_str_invalid_try = "invalid try";
const char *duk_str_with_in_strict_mode = "with in strict mode";
const char *duk_str_func_stmt_not_allowed = "function statement not allowed";
const char *duk_str_unterminated_stmt = "unterminated statement";
const char *duk_str_invalid_arg_name = "invalid argument name";
const char *duk_str_invalid_func_name = "invalid function name";
const char *duk_str_invalid_getset_name = "invalid getter/setter name";
const char *duk_str_func_name_required = "function name required";

/* Executor */
const char *duk_str_internal_error_exec_longjmp = "internal error in bytecode executor longjmp handler";

/* Limits */
const char *duk_str_valstack_limit = "valstack limit";
const char *duk_str_object_property_limit = "object property limit";
const char *duk_str_prototype_chain_limit = "prototype chain limit";
const char *duk_str_bound_chain_limit = "function call bound chain limit";
const char *duk_str_c_callstack_limit = "C call stack depth limit";
const char *duk_str_compiler_recursion_limit = "compiler recursion limit";
const char *duk_str_bytecode_limit = "bytecode limit";
const char *duk_str_reg_limit = "register limit";
const char *duk_str_temp_limit = "temp limit";
const char *duk_str_const_limit = "const limit";
const char *duk_str_func_limit = "function limit";
