# db-ticket
db-ticket is a little library to inspect digital ticket PDF files issued by [Deutsche Bahn](http://bahn.de/).

## Languages and platforms
The library is written in standard C (C99), using only libc; therefore it could and should run on many platforms.

Information about compilation and usage of the API can be found in `Documentation.md`.

## Testing
Rail tickets are expensive, and I can only do so many trips per year. This means that my collection of rail tickets is rather limited. If you happen to have more of them, please test them, and report if there are any unreadable tickets. (Unreadable means an API test score of 50% or less, tested on an officially issued DB ticket.)  
If you find bugs, or want to submit own code, open an issue or send me a pull request.

## Licence
The code is licenced under a MIT licence (see LICENSE.md for details).

## Acknowledgements
This library uses code from the [miniz library](https://code.google.com/p/miniz/) for stream inflation. The code was published under the public domain.
