# API documentation

## C library
Found in the `C` directory. Development happens on the `stdc` branch.

#### Compiling
You basically have four options. The first step, of course, is to download the repository. Then you can do the following:
* Run `make` or `make testtool` to create the `testtool` program. It accepts one parameter, the path to a file. This tool will test all modules on the given file.
* Run `make dynamic` to create a dynamic library `libdbticket.so` (you may need to rename this on some platforms).
* Import the code into your project and use the functions that way. (Please note that the licence also applies in this case.)

#### Using the C API
There are two API calls:

* `(float) checkPDFFile(const char *)`
* `(float) checkPDFFile(FILE *)`

Both are found inside `db-ticket.h`, which is the public header.

A return value of 0 or higher indicates a successful execution; other values indicate an error. (See the next section.)  
The value represents how many scores the file scored. The maximum value is defined in `kkMaximumScore`.

If you feel adventurous or only need a subset of the functions, see the functions in `modules.h`. They are pretty straightforward; nevertheless they are seen as internal, so you should know what you're doing if you use them.

### Return values
Error codes for `checkPDFFile` are:

* -1: File error. See `errno` for details.
* -2: File is either too large or too small to be a valid ticket file.

## C# .NET library
This branch is not being actively developed. If you want to check it out – the remains can be found in the `netfx` branch.