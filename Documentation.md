# API documentation

## C library
Found in the `C` directory. Development happens on the `stdc` branch.

### Compiling

#### UNIX systems
Execute `make` in the `C` directory. This builds both the test tool and the library (calling `make testtool` has the same effect).  
You can also run `make library` to only create a dynamic library; this file does not have an extension and you may need to give it an extension (often this is `.so` for Linux and `.dylib` for Mac OS X).

There are two special targets: `libdarwin` (creates a .dylib file) and `libunix` (creates a .so file).  
(I'm not exactly happy with this setup, but it has to work for the moment.)

*Note:* the test tool dynamically links `libdbticket` without extension; if you rename it, most likely the test tool will not run.

The `gcc` compiler may add references to libgcc. If this is a problem for you, you can switch the compiler to `clang` in the Makefile (lines 1 & 4).

#### Windows/Visual Studio
The `win32` directory contains a Visual Studio project file that builds a DLL file for Win32. The project was created with Visual Studio 2015, although the file should work with previous versions as well. (As the time of writing this, I don't have any other VS versions at hand to check.)

References to new files are made on an irregular basis, i.e. some things may be broken unless those files are added.

### Using the C API
There are two API calls for checking ticket files:

* `(float) checkPDFFile(const char *)`
* `(float) checkPDFFile(FILE *)`

Both are found inside `db-ticket.h`, which is the public header.

A return value of 0 or higher indicates a successful execution; other values indicate an error. (See the next section.)  
The value represents how many scores the file scored. The maximum value is defined in `kkMaximumScore`.

If you feel adventurous or only need a subset of the functions, see the functions in `modules.h`.  
They are pretty straightforward; nevertheless they are seen as internal, so you should know what you're doing if you use them.

For extracting travel data from the document, please use this call (from db-ticket.h):

* int readDataFromTicket(FILE *file, struct travel_information *target)`

All neccessary types, structures and enums can be found inside traveldata.h.

### Return values
Error codes for `checkPDFFile` are:

* -1: File error. See `errno` for details.
* -2: File is either too large or too small to be a valid ticket file.
