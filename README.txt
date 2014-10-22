=======
Duktape
=======

Duktape is a small and portable Ecmascript E5/E5.1 implementation.  It is
intended to be easily embeddable into C programs, with a C API similar in
spirit to Lua's.

The goal is to support the full E5 feature set like Unicode strings and
regular expressions.  Other feature highlights include:

  * Custom types (like pointers and buffers) for C integration

  * Reference counting and mark-and-sweep garbage collection
    (with finalizer support)

  * Co-operative threads, a.k.a. coroutines

  * Tail call support

You can browse Duktape programmer's API and other documentation at::

  http://www.duktape.org/

Building and integrating Duktape into your project is very straightforward.
See Makefile.example for an example::

  $ cd <dist_root>
  $ make -f Makefile.example
  [...]
  $ ./hello
  Hello world!
  2+3=5

To build an example command line tool, use the following::

  $ cd <dist_root>
  $ make -f Makefile.cmdline
  [...]
  $ ./duk
  ((o) Duktape
  duk> print('Hello world!');
  Hello world!
  = undefined

Duktape is (C) Sami Vaarala and licensed under the MIT license
(see ``LICENSE.txt``).  MurmurHash2 is used internally; it is also
under the MIT license.

Have fun!

-- 
Sami Vaarala
sami.vaarala@iki.fi

