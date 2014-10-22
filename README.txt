=======
Duktape
=======

Duktape is a small and portable Ecmascript E5/E5.1 implementation.  It is
intended to be easily embeddable into C programs, with a C API similar in
spirit to Lua's.  Duktape supports the full E5 feature set such as errors
(with tracebacks), Unicode strings, and regular expressions.  Other feature
highlights include:

  * Custom types (like pointers and buffers) for C integration

  * Reference counting and mark-and-sweep garbage collection
    (with finalizer support)

  * Co-operative threads, a.k.a. coroutines

  * Tail call support

You can browse Duktape programmer's API and other documentation at::

  http://www.duktape.org/

In particular, you should read the getting started section::

  http://duktape.org/guide.html#gettingstarted

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

There are further examples in the ``examples/`` directory.  Although
Duktape itself is widely portable, some of the examples are Linux only.
For instance the ``eventloop`` example illustrates how ``setTimeout()``
and other standard timer functions could be implemented on Unix/Linux.

The ``polyfills/`` directory provides a few replacement suggestions for
non-standard Javascript functions provided by other implementations.

This distributable contains Duktape version 0.11.0, created from git
commit 621b545c474dd7a3def7aefed0557b1a825a4578 (v0.10.0-827-g621b545).

Duktape is copyrighted by its authors (see ``AUTHORS.txt``) and licensed
under the MIT license (see ``LICENSE.txt``).  MurmurHash2 is used internally;
it is also under the MIT license.

Have fun!

Sami Vaarala (sami.vaarala@iki.fi)
