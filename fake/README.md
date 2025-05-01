PDCurses for fake console
============================

This directory contains PDCurses source code files specific to ... nothing!

In this "port", pdcurses does not draw anything GUI. It print "opcodes" instead.

When it want to move cursor, it will print "gotoyx 2, 5" to console, etc.

Queries like key events or screen width are "reply" from stdin (noecho).

It is designed to be used by automation scripts.

Modified from [../wincon](../wincon)


Building
--------

- Choose the appropriate makefile for your compiler:

        Makefile      - GCC (MinGW or Cygnus) [Yes]
        Makefile.bcc  - Borland C++ [Not up to date]
        Makefile.vc   - Microsoft Visual C++ [Not up to date]
        Makefile.wcc  - Watcom [Not up to date]

- Optionally, you can build in a different directory than the platform
  directory by setting PDCURSES_SRCDIR to point to the directory where
  you unpacked PDCurses, and changing to your target directory:

        set PDCURSES_SRCDIR=c:\pdcurses

- Build it:

        make -f makefilename

  (For Watcom, use "wmake" instead of "make"; for MSVC, "nmake"; for
  MinGW, "mingw32-make".) You'll get the library (pdcurses.lib or .a,
  depending on your compiler) and a lot of object files.

  You can also give the optional parameter "WIDE=Y", to build the
  library with wide-character (Unicode) support:

        wmake -f Makefile.wcc WIDE=Y

  When built this way, the library is not compatible with Windows 9x,
  unless you also link with the Microsoft Layer for Unicode (not
  tested).

  Another option, "UTF8=Y", makes PDCurses ignore the system locale, and
  treat all narrow-character strings as UTF-8. This option has no effect
  unless WIDE=Y is also set. Use it to get around the poor support for
  UTF-8 in the Windows console:

        make -f Makefile.bcc WIDE=Y UTF8=Y

  You can also use the optional parameter "DLL=Y" with Visual C++,
  MinGW or Cygwin, to build the library as a DLL:

        nmake -f Makefile.vc WIDE=Y DLL=Y

  When you build the library as a Windows DLL, you must always define
  PDC_DLL_BUILD when linking against it. (Or, if you only want to use
  the DLL, you could add this definition to your curses.h.)

  Add the target "demos" to build the sample programs.

- If your build stops with errors about PCONSOLE_SCREEN_BUFFER_INFOEX,
  add the parameter "INFOEX=N" to your make command line and try again.
  (This will happen with older compile environments.)


Acknowledgements
----------------

[Windows console port](../wincon) was originally provided by Chris Szurgot
<szurgot@itribe.net>
