rem assuming sqlite3ext.h is in ../sqlite_3_38 - adjust as needed
gcc -I..\sqlite_3_38 -c match.c 
g++ -std=c++11 -shared -o match.dll match.o getMatch.cpp

rem uncomment to play with bare cmd line version
rem g++ -DSTANDALONE=1 -std=c++11 -o grepO.exe getMatch.cpp
