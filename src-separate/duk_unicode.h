/*
 *  Unicode helpers
 */

#ifndef DUK_UNICODE_H_INCLUDED
#define DUK_UNICODE_H_INCLUDED

#define  DUK_UNICODE_MAX_XUTF8_LENGTH   7   /* up to 36 bit codepoints */
#define  DUK_UNICODE_MAX_CESU8_LENGTH   6   /* all codepoints up to U+10FFFF */

#define  DUK_UNICODE_CP_ZWNJ                   0x200c  /* zero-width non-joiner */
#define  DUK_UNICODE_CP_ZWJ                    0x200d  /* zero-width joiner */
#define  DUK_UNICODE_CP_REPLACEMENT_CHARACTER  0xfffd  /* http://en.wikipedia.org/wiki/Replacement_character#Replacement_character */

#ifdef DUK_USE_SOURCE_NONBMP
/*
 *  Automatically generated by extract_chars.py, do not edit!
 */

extern char duk_unicode_identifier_start_noascii[797];
#else
/*
 *  Automatically generated by extract_chars.py, do not edit!
 */

extern char duk_unicode_identifier_start_noascii_bmponly[614];
#endif

#ifdef DUK_USE_SOURCE_NONBMP
/*
 *  Automatically generated by extract_chars.py, do not edit!
 */

extern char duk_unicode_identifier_part_minus_identifier_start_noascii[397];
#else
/*
 *  Automatically generated by extract_chars.py, do not edit!
 */

extern char duk_unicode_identifier_part_minus_identifier_start_noascii_bmponly[348];
#endif

/*
 *  Automatically generated by extract_caseconv.py, do not edit!
 */

extern char duk_unicode_caseconv_uc[1288];
extern char duk_unicode_caseconv_lc[616];

/*
 *  Extern
 */

/* duk_unicode_support.c */
extern duk_u8 duk_unicode_xutf8_markers[7];
extern duk_u16 duk_unicode_re_ranges_digit[2];
extern duk_u16 duk_unicode_re_ranges_white[22];
extern duk_u16 duk_unicode_re_ranges_wordchar[8];
extern duk_u16 duk_unicode_re_ranges_not_digit[4];
extern duk_u16 duk_unicode_re_ranges_not_white[24];
extern duk_u16 duk_unicode_re_ranges_not_wordchar[10];

/*
 *  Prototypes
 */

int duk_unicode_get_xutf8_length(duk_u32 x);
size_t duk_unicode_encode_xutf8(duk_u32 x, duk_u8 *out);
size_t duk_unicode_encode_cesu8(duk_u32 x, duk_u8 *out);
int duk_unicode_xutf8_get_u32(duk_hthread *thr, duk_u8 **ptr, duk_u8 *ptr_start, duk_u8 *ptr_end, duk_u32 *out_cp);
duk_u32 duk_unicode_xutf8_get_u32_checked(duk_hthread *thr, duk_u8 **ptr, duk_u8 *ptr_start, duk_u8 *ptr_end);
duk_u32 duk_unicode_unvalidated_utf8_length(duk_u8 *data, duk_u32 blen);
int duk_unicode_is_whitespace(int x);
int duk_unicode_is_line_terminator(int x);
int duk_unicode_is_identifier_start(int x);
int duk_unicode_is_identifier_part(int x);
void duk_unicode_case_convert_string(duk_hthread *thr, int uppercase);
int duk_unicode_re_canonicalize_char(duk_hthread *thr, int x);
int duk_unicode_re_is_wordchar(int x);

#endif  /* DUK_UNICODE_H_INCLUDED */

