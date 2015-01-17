# db-ticket
db-ticket is a little library to check whether a given PDF file is a valid online ticket issued by [Deutsche Bahn](http://bahn.de/).

Please note that this library can't read the actual information on the ticket.

# Usage
You basically have four options. The first step, of course, is to download the repository. Then you can do the following:
* Run `make` or `make testtool` to create the `testtool` program. It accepts one parameter, the path to a file. This tool will test all modules on the given file.
* Run `make dynamic` to create a dynamic library `libdbticket.so` (you may need to rename this on some platforms).
* Run `make static` to create a static library `libdbticket.a`.
* Import the code into your project and use the functions that way. (Please note that the licence also applies in this case.)

## Simple API usage
In most cases, you'll only need one function call, which is `int checkPDFFile(const char *)` inside `db-ticket.h`.

A return value of 0 indicates a successful execution; other values indicate an error. You can (and should) check errno in this case.

If you feel adventurous or only need a subset of the functions, see the functions in `modules.h`. They are pretty straightforward; nevertheless they are seen as internal, so you should know what you're doing if you use them.

# Licence
The code is licenced under a MIT licence (see LICENSE.md for details).