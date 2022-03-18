## Introduction

Having been unable to find an 
[sqlite extension](https://sqlite.org/loadext.html)
 to implement the 
 [MATCH](https://sqlite.org/lang_expr.html#the_like_glob_regexp_match_and_extract_operators) operator, I've had a go myself.

It's probably buggy: in particular I've no idea 
whether it's UTF8 compliant or not. If it isn't
I don't know what to do about it. Consequently
*don't use it in production or when connected to
the internet*.

## Compilation

Edit and use `compile.sh` or `compile.bat` as appropriate.

There's a bit of a fandango since we need a clean gcc 
compilation for the `sqlite3` shell whilst calling out
to C++ `<regex>` functions.

### Dependencies

`sqlite3ext.h` from one of the options at [sqlite.org/download.html](https://sqlite.org/download.html)

## Usage

The extension replicates `grep -o` 
    
    select 'string' MATCH 'n.*';

will return 'ng', just as 

    echo string | grep -o "n.*"

will.

### Use with sqlite3 shell

do

    .load path/to/match

at the prompt or in `sqliterc`

### Use with C++

do something like

    sqlite3 *s3Ptr = ...
    int rc = sqlite3_create_function(s3Ptr, 
                                    "match", 
                                    2, 
                                    SQLITE_UTF8, 
                                    0, 
                                    matchFunc, 
                                    0, 
                                    0);

in your source. You can compile this as C++.

## Notes

This could perhaps be implemented by hacking on 
[regexp.c](https://www.sqlite.org/src/file?name=ext/misc/regexp.c&ci=trunk).

However, I've arranged for `matchFunc` to call 
`int getMatch(const char *pattern, const char *input, char *res)`
which uses the C++ `<regex>` library to do the heavy lifting.

All being well `*res` contains the matching text.

I look forward to seeing a more rigorous and 
economical implementation.
