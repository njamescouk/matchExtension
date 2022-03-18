#include <cstdio>
#include <string>
#include <string.h>
#include <regex>
#include "match.h"

char *progname = 0;
const char *szVersion = "1.0";

// return 0 on pattern error, otherwise 1
extern "C" int getMatch(const char *pattern, const char *input, char *res)
{
    res[0] = 0;

    if (pattern == 0 || input == 0)
        return 1;

    if (strlen(pattern) == 0 || strlen(input) == 0)
        return 1;

    std::string prefix;
    std::string matched;
    std::string suffix;
    std::cmatch mr;
    std::regex rx;
    bool reRes = false;
    try 
    {
        rx = std::regex (pattern);
        reRes = regex_search(input, mr, rx);
    }
    catch (std::regex_error rErr)
    {
        return 0;
    }

    if (reRes)
    {
        prefix = mr.format("$`");
        matched = mr.format("$&");
        suffix = mr.format("$'");

        if (!matched.empty())
            strcpy(res, matched.c_str());
    }

    return 1;
}

#if STANDALONE==1
void usage(FILE *fp);
 
int main (int argc, char *argv[])
{
    progname = argv[0];

    if (argc != 3)
    {
        usage(stderr);
        return __LINE__;
    }

    char *pattern = argv[1];
    char *str = argv[2];
    char *res = (char *)calloc(strlen(str) + 4, sizeof(char));

    if (getMatch (pattern, str, res) == 0)
    {
        fprintf(stderr, "bad pattern %s\n", pattern);
    }
    else
    {
        printf("%s\n", res);
    }

    return 0;
}
 
void usage(FILE *fp)
{
    fprintf(fp, "usage %s pattern string\n", progname);
    fprintf(fp, "prints match (if any) of pattern in string\n");
}

#endif // STANDALONE
