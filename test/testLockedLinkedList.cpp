#include "linkedList.h"
#include <catch.hpp>

TEST_CASE("Insert into linked list, search and erase nodes") {
  list::LinkedList<int> lList;
  for (int i = 0; i < 1000; ++i) {
    lList.insertNode(i);
  }
  for (int i = 0; i < 1000; ++i) {
    REQUIRE(lList.searchNode(i) == true);
  }
  lList.insertNode(1000);

  REQUIRE(lList.searchNode(1000) == true);
  REQUIRE(lList.searchNode(2100) == false);
  REQUIRE(lList.searchNode(1) == true);
  lList.eraseNode(1);
  lList.eraseNode(2);
  lList.eraseNode(2222);
  lList.eraseNode(-20);
  lList.eraseNode(333);

  REQUIRE(lList.searchNode(1000) == true);
  REQUIRE(lList.searchNode(1) == false);
  REQUIRE(lList.searchNode(2) == false);
  REQUIRE(lList.searchNode(333) == false);
}

TEST_CASE("Insert in linked list for different threads ") {
  list::LinkedList<int> lList;
  int nThreads = 3;
  std::vector<std::thread> threads(nThreads);

  threads[0] = std::thread([&lList]() {
    for (int j = 0; j < 5; ++j) {
      lList.insertNode(j);
    }
  });
  threads[1] = std::thread([&lList]() {
    for (int j = 5; j < 10; ++j) {
      lList.insertNode(j);
    }
  });
  threads[2] = std::thread([&lList]() {
    for (int j = 10; j < 15; ++j) {
      lList.insertNode(j);
    }
  });
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  REQUIRE(lList.searchNode(0) == true);
  REQUIRE(lList.searchNode(1) == true);
  REQUIRE(lList.searchNode(2) == true);
  REQUIRE(lList.searchNode(3) == true);
  REQUIRE(lList.searchNode(4) == true);
  REQUIRE(lList.searchNode(5) == true);
  REQUIRE(lList.searchNode(6) == true);
  REQUIRE(lList.searchNode(7) == true);
  REQUIRE(lList.searchNode(8) == true);
  REQUIRE(lList.searchNode(9) == true);
  REQUIRE(lList.searchNode(10) == true);
  REQUIRE(lList.searchNode(11) == true);
  REQUIRE(lList.searchNode(12) == true);
  REQUIRE(lList.searchNode(13) == true);
  REQUIRE(lList.searchNode(14) == true);
}

TEST_CASE("Erase in linked list for different threads ") {
  list::LinkedList<int> lList;
  int nThreads = 3;
  std::vector<std::thread> threads(nThreads);

  for (int i = 0; i < 15; ++i) {
    lList.insertNode(i);
  }

  threads[0] = std::thread([&lList]() {
    for (int j = 0; j < 5; ++j) {
      lList.eraseNode(j);
    }
  });
  threads[1] = std::thread([&lList]() {
    for (int j = 5; j < 10; ++j) {
      lList.eraseNode(j);
    }
  });
  threads[2] = std::thread([&lList]() {
    for (int j = 10; j < 15; ++j) {
      lList.eraseNode(j);
    }
  });
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  REQUIRE(lList.searchNode(0) == false);
  REQUIRE(lList.searchNode(1) == false);
  REQUIRE(lList.searchNode(2) == false);
  REQUIRE(lList.searchNode(3) == false);
  REQUIRE(lList.searchNode(4) == false);
  REQUIRE(lList.searchNode(5) == false);
  REQUIRE(lList.searchNode(6) == false);
  REQUIRE(lList.searchNode(7) == false);
  REQUIRE(lList.searchNode(8) == false);
  REQUIRE(lList.searchNode(9) == false);
  REQUIRE(lList.searchNode(10) == false);
  REQUIRE(lList.searchNode(11) == false);
  REQUIRE(lList.searchNode(12) == false);
  REQUIRE(lList.searchNode(13) == false);
  REQUIRE(lList.searchNode(14) == false);
}

TEST_CASE("Search in linked list for different threads ") {
  list::LinkedList<int> lList;
  int nThreads = 3;
  std::vector<std::thread> threads(nThreads);

  for (int i = 0; i < 15; ++i) {
    lList.insertNode(i);
  }

  threads[0] = std::thread([&lList]() {
    for (int j = 0; j < 5; ++j) {
      REQUIRE(lList.searchNode(j) == true);
    }
  });
  threads[1] = std::thread([&lList]() {
    for (int j = 5; j < 10; ++j) {
      REQUIRE(lList.searchNode(j) == true);
    }
  });
  threads[2] = std::thread([&lList]() {
    for (int j = 10; j < 15; ++j) {
      REQUIRE(lList.searchNode(j) == true);
    }
  });
  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}
