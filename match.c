#include "match.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#ifdef SQLITE_CORE
#include "sqlite3.h" 
#else
#include "sqlite3ext.h" 
SQLITE_EXTENSION_INIT1 
#endif

/*
    implement pattern MATCH input, equivalent to
    match(input, pattern)

    returns the matched portion of input specified by the 
    regular expression pattern
*/
void matchFunc(sqlite3_context *pCtx, int argc, sqlite3_value **argv)
{
    char *res = 0;
    int resLen = 0;
    const char *pattern = (const char *)sqlite3_value_text(argv[0]);
    const char *input   = (const char *)sqlite3_value_text(argv[1]);
    int byteCount = 0;
    const char *curChar = 0;

    if (sqlite3_value_type(argv[0])==SQLITE_NULL
        || sqlite3_value_type(argv[1])==SQLITE_NULL)
    {
        free (res);
        sqlite3_result_null(pCtx);
        return;
    }

    resLen = strlen(input) + 10;
    res = (char *)calloc(resLen, sizeof(char));

    if (getMatch(pattern, input, res) == 0)
    {
        char *buf = (char *)calloc(strlen(pattern)*2, sizeof(char));
        sprintf(buf, "bad pattern \"%s\"\n", pattern);
        sqlite3_result_error(pCtx, buf, -1);
        return;
    }
    else if (strlen(res) == 0)
    {
        free (res);
        sqlite3_result_null(pCtx);
    }
    else
    {
        sqlite3_result_text(pCtx, res, -1, free);
    }
}

/* SQLite invokes this routine once when it loads the extension.
** Create new functions, collating sequences, and virtual table
** modules here.  This is usually the only exported symbol in
** the shared library.
*/
/*
*/
#if !SQLITE_CORE
int sqlite3_match_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
)
{
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi)
  rc = sqlite3_create_function(db, "match", 2, SQLITE_UTF8, 0, matchFunc, 0, 0);
  return rc;
}
#endif  // !SQLITE_CORE

