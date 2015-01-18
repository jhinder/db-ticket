# db-ticket
db-ticket is a little library to check whether a given PDF file is a valid online ticket issued by [Deutsche Bahn](http://bahn.de/).

Please note that this library can't read the actual information on the ticket.

## Languages and platforms
This library exists in two languages so far:
* C (standard C99), for any UNIX system,
* C#, for any .NET 4.0 (or higher) project,

The C library should also work for iOS, but as I have no resources for iOS development, I can't test that.

Documentation on API usage and sample code for all languages and platforms can be found in `Documentation.md`.

## Testing
Rail tickets are expensive, and I can only do so many trips per year. This means that my collection of rail tickets is rather limited. If you happen to have more of them, please test them, and report if there are any unreadable tickets. (Unreadable means an API test score of 50% or less, tested on an officially issued DB ticket.)  
If you find bugs, or want to submit own code, open an issue or send me a pull request.

## Licence
The code is licenced under a MIT licence (see LICENSE.md for details).