Implementation of locked ordered linked list. Concurrency is implemented using mutex on each
node. There are three functions implemented for this data structure, insert, search and
erase of node. Each of these functions have similar implementation regarding concurrency,
locks are hold on the nodes function is actually interested in and function unlocks each
node as it moves on to the next node.

CMake is used for project build. For building tests for testLockedLinkedList.cpp,
Catch2 repo from GitHub (https://github.com/catchorg/Catch2)
is pulled in local project linkedList. Doxygen is used for project documentation.

To build project and project documentation, go to root directory:
$ cd root
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make doc

To run tests:
$ cd test
$ ./tests

To check valgrind: valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./tests
To check coverage: gcov-10 testSkipList.cpp.gcno
