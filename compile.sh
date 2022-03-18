# assuming sqlite3ext.h is in ../sqlite_3_38 - adjust as needed
gcc -fPIC -I../sqlite_3_38 -c match.c 
g++ -fPIC -std=c++11 -shared -o match.so match.o getMatch.cpp
rm match.o

# uncomment to play with bare cmd line version
# g++ -DSTANDALONE=1 -std=c++11 -o grepO getMatch.cpp
